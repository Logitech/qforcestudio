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

#include "ForceEffect.h"
#include <math.h>
#include "QForceStudio.h"
#include "GameController.h"
#include <QDebug>


ForceEffect::ForceEffect(quint16 forceType, QObject *parent) :
    QObject(parent),
    m_envelope(NULL)
{
    memset(&m_effect, 0, sizeof(m_effect));
    m_effect.type = forceType;
    m_effect.id = -1;

    initializeEnvelope(NULL);
}

double ForceEffect::convertFF2RAD(quint16 direction)
{
    double result = (M_TAU * direction) / 0x10000;
    return result;
}

quint16 ForceEffect::convertRAD2FF(double direction)
{
    quint16 result = qRound((direction * 0x10000) / M_TAU) % 0xffff;
    return result;
}

void ForceEffect::setGameController(GameController *controller)
{
    bool forceWasDownloaded = isDownloaded();
    if (m_gameController)
    {
        if (forceWasDownloaded)
        {
            // remove from existing controller...
            m_gameController->removeForce(this);
        }

        disconnect(m_gameController, 0, this, 0);
    }

    m_gameController = controller;

    if (m_gameController)
    {
        connect(m_gameController, SIGNAL(errorMessage(const QString&)), this, SLOT(onErrorMessage(const QString&)));

        if (forceWasDownloaded)
        {
            m_gameController->addForce(this);
        }
    }
}

ff_effect *ForceEffect::ffEffect(void)
{
    return &m_effect;
}

QString ForceEffect::gist(void) const
{
    QString result;
    result.append(QString("struct ff_effect e;\n"));
    result.append(QString("e.type = %1; // %2 \n").arg(m_effect.type).arg(forceType2GistString(m_effect.type)));
    result.append(QString("e.id = -1; // currently: %1\n").arg(m_effect.id));
    result.append(QString("e.direction = 0x%1;\n").arg(m_effect.direction, 0, 16));
    result.append(QString("e.trigger.button = 0x%1;\n").arg(m_effect.trigger.button, 0, 16));
    result.append(QString("e.trigger.interval = 0x%1;\n").arg(m_effect.trigger.interval, 0, 16));
    result.append(QString("e.replay.delay = %1; // 0x%2\n").arg(m_effect.replay.delay).arg(m_effect.replay.delay, 0, 16));
    result.append(QString("e.replay.length = %1; // 0x%2\n").arg(m_effect.replay.length).arg(m_effect.replay.length, 0, 16));
    return result;
}

QString ForceEffect::forceType2GistString(int forceType)
{
    switch (forceType)
    {
    case FF_RUMBLE:
        return QLatin1String("FF_RUMBLE");
    case FF_PERIODIC:
        return QLatin1String("FF_PERIODIC");
    case FF_CONSTANT:
        return QLatin1String("FF_CONSTANT");
    case FF_SPRING:
        return QLatin1String("FF_SPRING");
    case FF_FRICTION:
        return QLatin1String("FF_FRICTION");
    case FF_DAMPER:
        return QLatin1String("FF_DAMPER");
    case FF_INERTIA:
        return QLatin1String("FF_INERTIA");
    case FF_RAMP:
        return QLatin1String("FF_RAMP");
    default:
        break;
    }
    return QString(">UNKNOWN (%1, 0x%2)<").arg(forceType).arg(forceType, 0, 16);
}

QString ForceEffect::waveformType2GistString(int waveform)
{
    switch (waveform)
    {
        case FF_SQUARE:
            return QLatin1String("FF_SQUARE");
        case FF_TRIANGLE:
            return QLatin1String("FF_TRIANGLE");
        case FF_SINE:
            return QLatin1String("FF_SINE");
        case FF_SAW_UP:
            return QLatin1String("FF_SAW_UP");
        case FF_SAW_DOWN:
            return QLatin1String("FF_SAW_DOWN");
        case FF_CUSTOM:
            return QLatin1String("FF_CUSTOM");
        default:
            break;
    }
    return QString(">UNKNOWN (%1, 0x%2)<").arg(waveform).arg(waveform, 0, 16);
}

QString ForceEffect::envelopeGist(const QString &prefix, const ff_envelope &env)
{
    QString result;
    result.append(QString("%1.attack_length = %2;\n").arg(prefix).arg(env.attack_length));
    result.append(QString("%1.attack_level = %2;\n").arg(prefix).arg(env.attack_level));
    result.append(QString("%1.fade_length = %2;\n").arg(prefix).arg(env.fade_length));
    result.append(QString("%1.fade_level = %2;\n").arg(prefix).arg(env.fade_level));
    return result;
}

bool ForceEffect::isDownloaded(void) const
{
    return (-1 != m_effect.id);
}

void ForceEffect::start(void)
{
    if (!m_gameController)
    {
        emit statusChanged(tr("Can't start effect if no game controller is present."));
        return;
    }

    if (m_gameController->startForce(this))
    {
        emit statusChanged(tr("Force downloaded as id %1.").arg(m_effect.id));
    }
}

void ForceEffect::stop(void)
{
    if (m_gameController)
    {
        if (m_gameController->stopForce(this))
        {
            emit statusChanged(tr("Force stopped."));
        }
    }
}

void ForceEffect::reset(void)
{
    setDirection(0.0);
    setDelay(0.0);
    setDuration(3.0);

    setAttackLevel(0.0);
    setAttackLength(0.0);
    setFadeLength(0.0);
    setFadeLevel(0.0);
}

void ForceEffect::update(void)
{
    if (isDownloaded() && m_gameController)
    {
        if (m_gameController->addForce(this))
        {
            emit statusChanged(tr("Force updated."));
        }
    }
}

double ForceEffect::direction(void) const
{
    return convertFF2RAD(m_effect.direction);
}

double ForceEffect::delay(void) const
{
    return m_effect.replay.delay / 1000.0;
}

double ForceEffect::duration(void) const
{
    return (m_effect.replay.length & 0x7fff) / 1000.0;
}

bool ForceEffect::isInfiniteDuration(void) const
{
    return (0x7fff < m_effect.replay.length);
}

double ForceEffect::level(void) const
{
    return 0.0;
}

double ForceEffect::period(void) const
{
    return (m_effect.u.periodic.period / 1000.0);
}

double ForceEffect::phase(void) const
{
    return (m_effect.u.periodic.phase / 65535.0);
}

double ForceEffect::offset(void) const
{
    return 0.0;
}

double ForceEffect::attackLevel(void) const
{
    Q_ASSERT(m_envelope);
    if (m_envelope->exists())
    {
        return m_envelope->attackLevel();
    }
    return 0.0;
}

double ForceEffect::fadeLevel(void) const
{
    Q_ASSERT(m_envelope);
    if (m_envelope->exists())
    {
        return m_envelope->fadeLevel();
    }
    return 0.0;
}

double ForceEffect::attackLength(void) const
{
    Q_ASSERT(m_envelope);
    if (m_envelope->exists())
    {
        return m_envelope->attackLength();
    }
    return 0.0;
}

double ForceEffect::fadeLength(void) const
{
    Q_ASSERT(m_envelope);
    if (m_envelope->exists())
    {
        return m_envelope->fadeLength();
    }
    return 0.0;
}

bool ForceEffect::withinAttackLength(double atTimeIndex) const
{
    Q_ASSERT(m_envelope);
    double unused;
    return m_envelope->withinAttackLength(atTimeIndex, unused);
}

bool ForceEffect::withinFadeLength(double atTimeIndex, double totalLength) const
{
    Q_ASSERT(m_envelope);
    if (!isInfiniteDuration())
    {
        double unused;
        return m_envelope->withinFadeLength(atTimeIndex, totalLength, unused);
    }
    return false;
}

double ForceEffect::envelopeLevel(double atTimeIndex, double totalLength) const
{
    Q_ASSERT(m_envelope);
    if (m_envelope->exists())
    {
        double ratio = 0;
        if (m_envelope->withinAttackLength(atTimeIndex, ratio))
        {
            return attackLevel() + (level() - attackLevel()) * ratio;
        }

        if (m_envelope->withinFadeLength(atTimeIndex, totalLength, ratio))
        {
            return fadeLevel() + (level() - fadeLevel()) * ratio;
        }
        return level();
    }
    return level();
}

double ForceEffect::value(double atTimeIndex) const
{
    Q_UNUSED(atTimeIndex);
    return 0.0;
}

ForceCondition *ForceEffect::firstCondition(void) const
{
    return NULL;
}

ForceCondition *ForceEffect::secondCondition(void) const
{
    return NULL;
}

void ForceEffect::setDirection(double newDirection)
{
    newDirection = qMax(0.0, qMin(M_TAU, newDirection));
    int direction = convertRAD2FF(newDirection);
    if (direction != m_effect.direction)
    {
        m_effect.direction = direction;
        emit directionChanged(newDirection);
        emit valuesChanged();
        update();
    }
}

void ForceEffect::setDelay(double newDelay)
{
    newDelay = qMax(0.0, qMin(32.767, newDelay));
    int delay = qRound(newDelay * 1000.0);
    if (delay != m_effect.replay.delay)
    {
        m_effect.replay.delay = delay;
        emit delayChanged(newDelay);
        emit valuesChanged();
        update();
    }
}

void ForceEffect::setDuration(double newDuration, bool setInfinite /* = false */)
{
    if (setInfinite)
    {
        if (isInfiniteDuration() != setInfinite)
        {
            m_effect.replay.length |= 0x8000;
            emit durationIsInfiniteChanged(setInfinite);
            emit valuesChanged();
            update();
        }
    }
    else
    {
        bool wasInfinite = isInfiniteDuration();
        newDuration = qMax(0.0, qMin(32.767, newDuration));
        int duration = qRound(newDuration * 1000);
        if (duration != m_effect.replay.length)
        {
            m_effect.replay.length = duration;
            emit durationChanged(newDuration);
            emit valuesChanged();
            if (wasInfinite)
            {
                emit durationIsInfiniteChanged(false);
                emit valuesChanged();
            }
            setAttackLength(qMin(attackLength(), newDuration - fadeLength()));
            setFadeLength(qMin(fadeLength(), newDuration - attackLength()));
            update();
        }
    }
}

void ForceEffect::setLevel(double newLevel)
{
    Q_UNUSED(newLevel);
}

void ForceEffect::setPeriod(double newPeriod)
{
    newPeriod = qMax(0.0, qMin(65.535, newPeriod));
    int period = qRound(newPeriod * 1000.0);
    if (period != m_effect.u.periodic.period)
    {
        m_effect.u.periodic.period = period;
        emit periodChanged(newPeriod);
        emit valuesChanged();
        update();
    }
}

void ForceEffect::setPhase(double newPhase)
{
    newPhase = qMax(0.0, qMin(1.0, newPhase));
    int phase = qRound(newPhase * 65535.0);
    if (phase != m_effect.u.periodic.phase)
    {
        m_effect.u.periodic.phase = phase;
        emit phaseChanged(newPhase);
        emit valuesChanged();
        update();
    }
}

void ForceEffect::setOffset(double newOffset)
{
    Q_UNUSED(newOffset);
}

void ForceEffect::setAttackLevel(double newLevel)
{
    Q_ASSERT(m_envelope);
    m_envelope->setAttackLevel(newLevel);
    update();
}

void ForceEffect::setAttackLength(double newLength)
{
    Q_ASSERT(m_envelope);
    m_envelope->setAttackLength(newLength);
    update();
}

void ForceEffect::setFadeLevel(double newLevel)
{
    Q_ASSERT(m_envelope);
    m_envelope->setFadeLevel(newLevel);
    update();
}

void ForceEffect::setFadeLength(double newLength)
{
    Q_ASSERT(m_envelope);
    m_envelope->setFadeLength(newLength);
    update();
}

void ForceEffect::onErrorMessage(const QString &newMessage)
{
    emit statusChanged(newMessage);
}

void ForceEffect::initializeEnvelope(ff_envelope *env)
{
    SAFE_DELETE(m_envelope);

    m_envelope = new ForceEnvelope(env, this);
    connect(m_envelope, SIGNAL(envelopeChanged()), this, SIGNAL(valuesChanged()));
}
