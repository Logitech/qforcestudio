/*
 * QForceStudio - Explore Force Feedback on Linux.
 *
 * Copyright (C) 2014 Logitech Europe S.A.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * To Contact Logitech Europe S.A. please send an email to
 * devtechsupport@logitech.com or via mail at EPFL - Quartier de
 * l'Innovation Daniel Borel Innovation Center CH - 1015 Lausanne,
 * Attention Roland Bosa.
 *
 */

#include "QForceStudio.h"
#include "GameController.h"
#include "FakeGameController.h"
#include "ForceEffect.h"
#include <QDebug>

#ifdef Q_OS_LINUX
#include <inttypes.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#endif

GameController::GameController(QObject *parent) :
    QObject(parent),
    m_fd(-1),
    m_effectCount(0)
{
    int capSize = (FF_CNT + 8 * sizeof(unsigned long) - 1) / (8 * sizeof(unsigned long));
    m_deviceCapsByteCount = capSize * sizeof(unsigned long);
    m_deviceCaps = new unsigned long[capSize];
    memset(m_deviceCaps, 0, m_deviceCapsByteCount);
}

GameController::~GameController(void)
{
    SAFE_DELETE(m_deviceCaps);
}

bool GameController::open(const QString &filename)
{
    close();

    m_filename = filename;

#ifdef Q_OS_LINUX
    m_fd = ::open(filename.toLatin1(), O_RDWR);
    if (m_fd < 0)
    {
        //qDebug() << "Failed to open " << filename << " with error: " << errno;
        return false;
    }

    int driverVersion = -1;
    if (::ioctl(m_fd, EVIOCGVERSION, &driverVersion) < 0)
    {
        qDebug() << "Failed to get driver version with error: " << errno;
        close();
        return false;
    }
    qDebug() << filename << ": version:" << QString::number(driverVersion, 16);

    char nameBuffer[1024];
    if (::ioctl(m_fd, EVIOCGNAME(sizeof(nameBuffer)), nameBuffer) < 0)
    {
        qDebug() << "Failed to retrieve name of device with error: " << errno;
        close();
        return false;
    }
    m_name = QString::fromLatin1(nameBuffer);
    qDebug() << filename << ": name:" << m_name;

    int num = 0;
    if (::ioctl(m_fd, EVIOCGEFFECTS, &num) < 0)
    {
        qDebug() << "Failed to retrieve number of effects with error: " << errno;
        close();
        return false;
    }
    m_effectCount = num;
    if (m_effectCount <= 0)
    {
        qDebug() << "Not a force device (max effect count is " << m_effectCount << ").";
        close();
        return false;
    }
    qDebug() << filename << ": can handle " << m_effectCount << " effects.";

    struct input_id inputId;
    if (::ioctl(m_fd, EVIOCGID, &inputId) < 0)
    {
        qDebug() << "Failed to retrieve input id struct with error: " << errno;
        close();
        return false;
    }
    qDebug() << "Bus type of " << filename << ": " << inputId.bustype;

    if (inputId.bustype != BUS_VIRTUAL)
    {
        if (::ioctl(m_fd, EVIOCGPHYS(sizeof(nameBuffer)), nameBuffer) < 0)
        {
            qDebug() << "Failed to retrieve physical location of device with error: " << errno;
            close();
            return false;
        }
        qDebug() << "Physical location of " << filename << ": " << QString(nameBuffer);
    }

    if (::ioctl(m_fd, EVIOCGUNIQ(sizeof(nameBuffer)), nameBuffer) < 0)
    {
        qDebug() << "Failed to retrieve unique identifier with error: " << errno;
    }
    else
    {
        qDebug() << "Unique identifier of " << filename << ": " << QString(nameBuffer);
    }

    if (::ioctl(m_fd, EVIOCGPROP(sizeof(nameBuffer)), nameBuffer) < 0)
    {
        qDebug() << "Failed to retrieve device properties with error: " << errno;
    }
    else
    {
        qDebug() << "Device properties of " << filename << ": " << QString(nameBuffer);
    }

    Q_ASSERT(m_deviceCaps);
    if (::ioctl(m_fd, EVIOCGBIT(EV_FF, m_deviceCapsByteCount), m_deviceCaps) < 0)
    {
        qDebug() << "Failed to retrieve device caps with error: " << errno;
        close();
        return false;
    }
#endif

    return true;
}

void GameController::close(void)
{
    if (isOpen())
    {
#ifdef Q_OS_LINUX
        ::close(m_fd);
#endif
        m_fd = -1;
    }

    m_effectCount = 0;
    m_name.clear();
    m_filename.clear();
    memset(m_deviceCaps, 0, m_deviceCapsByteCount);
}

bool GameController::isOpen(void) const
{
    return (0 <= m_fd);
}

QString GameController::name(void) const
{
    return m_name;
}

QString GameController::filename(void) const
{
    return m_filename;
}

int GameController::numEffects(void) const
{
    return m_effectCount;
}

bool GameController::canRenderConstant(void) const
{
    return hasSupportFor(FF_CONSTANT);
}

bool GameController::canRenderPeriodic(void) const
{
    return hasSupportFor(FF_PERIODIC);
}

bool GameController::canRenderSquare(void) const
{
    return hasSupportFor(FF_SQUARE);
}

bool GameController::canRenderTriangle(void) const
{
    return hasSupportFor(FF_TRIANGLE);
}

bool GameController::canRenderSine(void) const
{
    return hasSupportFor(FF_SINE);
}

bool GameController::canRenderSawUp(void) const
{
    return hasSupportFor(FF_SAW_UP);
}

bool GameController::canRenderSawDown(void) const
{
    return hasSupportFor(FF_SAW_DOWN);
}

bool GameController::canRenderCustom(void) const
{
    return hasSupportFor(FF_CUSTOM);
}

bool GameController::canRenderRamp(void) const
{
    return hasSupportFor(FF_RAMP);
}

bool GameController::canRenderSpring(void) const
{
    return hasSupportFor(FF_SPRING);
}

bool GameController::canRenderFriction(void) const
{
    return hasSupportFor(FF_FRICTION);
}

bool GameController::canRenderDamper(void) const
{
    return hasSupportFor(FF_DAMPER);
}

bool GameController::canRenderRumble(void) const
{
    return hasSupportFor(FF_RUMBLE);
}

bool GameController::canRenderInertia(void) const
{
    return hasSupportFor(FF_INERTIA);
}

bool GameController::canAdjustGain(void) const
{
    return hasSupportFor(FF_GAIN);
}

bool GameController::canAdjustAutocenter(void) const
{
    return hasSupportFor(FF_AUTOCENTER);
}

#ifdef Q_OS_MAC
GameControllerPtrList GameController::allControllers(void)
{
    GameControllerPtrList result;
    result.append(GameControllerPtr(new FakeGameController));
    return result;
}
#endif

#ifdef Q_OS_LINUX
GameControllerPtrList GameController::allControllers(void)
{
    GameControllerPtrList result;
    for (int i = 0; i < 32; ++i)
    {
        QString filename = QString("/dev/input/event%1").arg(i);
        GameControllerPtr controller(new GameController());
        if (controller->open(filename))
        {
            result.append(controller);
        }
    }
    return result;
}
#endif

bool GameController::addForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't add a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't download force on non-open device."));
        return false;
    }

#ifdef Q_OS_LINUX
    if (::ioctl(m_fd, EVIOCSFF, force->ffEffect()) < 0)
    {
        emit errorMessage(tr("Failed to set force with error: %1.").arg(errno));
        return false;
    }
#else
#pragma warning("No add force support on current platform")
#endif
    return true;
}

bool GameController::startForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't start a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't start a force on non-open device."));
        return false;
    }

    if (!force->isDownloaded())
    {
        if (!addForce(force))
        {
            return false;
        }
    }

#ifdef Q_OS_LINUX
    input_event play;
    play.type = EV_FF;
    play.code = force->ffEffect()->id;
    play.value = 1; // TODO: add play count
    write(m_fd, (const void *) &play, sizeof(play));
    // TODO: add error handling
#else
#pragma warning("No start force support on current platform")
#endif
    return true;
}

bool GameController::stopForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't stop a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't stop a force on non-open device."));
        return false;
    }

    if (!force->isDownloaded())
    {
        qDebug() << "No need to stop a force that hasn't been downloaded.";
        return true;
    }

#ifdef Q_OS_LINUX
    input_event stop;
    stop.type = EV_FF;
    stop.code = force->ffEffect()->id;
    stop.value = 0;

    write(m_fd, (const void *) &stop, sizeof(stop));
    // TODO: add error handling
#else
#pragma warning("No stop force support on current platform")
#endif
    return true;
}

bool GameController::removeForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't unload a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't unload a force from a non-open device."));
        return false;
    }

    if (!force->isDownloaded())
    {
        qDebug() << "No need to remove a force that hasn't been downloaded.";
        return true;
    }

#ifdef Q_OS_LINUX
    if (::ioctl(m_fd, EVIOCRMFF, force->ffEffect()->id) < 0)
    {
        emit errorMessage(tr("Failed to remove force with error: %1.").arg(errno));
        return false;
    }
    force->ffEffect()->id = -1;
#else
#pragma warning("No remove force support on current platform")
#endif
    return true;
}

bool GameController::stopAllForces(void)
{
    return false;
}

bool GameController::pauseAllForces(void)
{
    return false;
}

bool GameController::continueAllForces(void)
{
    return false;
}

bool GameController::resetAllForces(void)
{
    return false;
}

bool GameController::enableActuators(bool enable)
{
    return false;
}

bool GameController::sendCommand(const QByteArray& command, QByteArray& response)
{
    return false;
}

bool GameController::hasSupportFor(int capability) const
{
    if (!m_deviceCaps)
    {
        qDebug() << "Don't have any capability array!";
        return false;
    }

    int offset = capability / (8 * sizeof(unsigned long));
    int bitnum = capability % (8 * sizeof(unsigned long));

    return (0 != (m_deviceCaps[offset] & (1 << bitnum)));
}
