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


FakeGameController::FakeGameController(QObject *parent)
    : GameController(parent)
{
    m_name = "Fake Game Controller";
    m_filename = "/tmp/fake_device";
    m_effectCount = 64;
}


FakeGameController::~FakeGameController(void)
{
}


bool FakeGameController::open(const QString &filename)
{
    m_fd = 42;
    return true;
}


void FakeGameController::close(void)
{
    m_fd = -1;
}


bool FakeGameController::addForce(ForceEffect *force)
{
    return true;
}


bool FakeGameController::startForce(ForceEffect *force)
{
    return true;
}


bool FakeGameController::stopForce(ForceEffect *force)
{
    return true;
}


bool FakeGameController::removeForce(ForceEffect *force)
{
    return true;
}


bool FakeGameController::stopAllForces(void)
{
    return true;
}


bool FakeGameController::pauseAllForces(void)
{
    return true;
}


bool FakeGameController::continueAllForces(void)
{
    return true;
}


bool FakeGameController::resetAllForces(void)
{
    return true;
}


bool FakeGameController::enableActuators(bool enable)
{
    return true;
}


bool FakeGameController::sendCommand(const QByteArray &command, QByteArray &response)
{
    return true;
}


bool FakeGameController::hasSupportFor(int capability) const
{
    return true;
}
