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

#include "ForceEnvelope.h"

ForceEnvelope::ForceEnvelope(ff_envelope *envelope, QObject *parent /* = 0 */) :
    QObject(parent),
    m_envelope(envelope)
{
}

bool ForceEnvelope::exists(void) const
{
    return (NULL != m_envelope);
}

double ForceEnvelope::attackLevel(void) const
{
    if (exists())
    {
        return m_envelope->attack_level / 32767.0;
    }
    return 0.0;
}

double ForceEnvelope::attackLength(void) const
{
    if (exists())
    {
        return m_envelope->attack_length / 1000.0;
    }
    return 0.0;
}

double ForceEnvelope::fadeLevel(void) const
{
    if (exists())
    {
        return m_envelope->fade_level / 32767.0;
    }
    return 0.0;
}

double ForceEnvelope::fadeLength(void) const
{
    if (exists())
    {
        return m_envelope->fade_length / 1000.0;
    }
    return 0.0;
}

bool ForceEnvelope::withinAttackLength(double atTimeIndex, double &ratio) const
{
    if (exists())
    {
        if ((0.0 <= atTimeIndex) && (atTimeIndex < attackLength()))
        {
            ratio = atTimeIndex / attackLength();
            return true;
        }
    }
    return false;
}

bool ForceEnvelope::withinFadeLength(double atTimeIndex, double totalLength, double &ratio) const
{
    if (exists() && (0.0 < totalLength))
    {
        double adjusted = totalLength - atTimeIndex;
        if ((0.0 <= adjusted) && (adjusted < fadeLength()))
        {
            ratio = adjusted / fadeLength();
            return true;
        }
    }
    return 0.0;
}

void ForceEnvelope::setAttackLevel(double newLevel)
{
    if (exists())
    {
        int level = qRound(newLevel * 32767.0);
        if (level != m_envelope->attack_level)
        {
            m_envelope->attack_level = level;
            emit envelopeChanged();
        }
    }
}

void ForceEnvelope::setAttackLength(double newLength)
{
    if (exists())
    {
        int length = qMax(0, qRound(newLength * 1000.0));
        if (length != m_envelope->attack_length)
        {
            m_envelope->attack_length = length;
            emit envelopeChanged();
        }
    }
}

void ForceEnvelope::setFadeLevel(double newLevel)
{
    if (exists())
    {
        int level = qRound(newLevel * 32767.0);
        if (level != m_envelope->fade_level)
        {
            m_envelope->fade_level = level;
            emit envelopeChanged();
        }
    }
}

void ForceEnvelope::setFadeLength(double newLength)
{
    if (exists())
    {
        int length = qMax(0, qRound(newLength * 1000.0));
        if (length != m_envelope->fade_length)
        {
            m_envelope->fade_length = length;
            emit envelopeChanged();
        }
    }
}
