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

#include "FERamp.h"
#include "QForceStudio.h"
#include <linux/input.h>

FERamp::FERamp(QObject *parent) :
    ForceEffect(FF_RAMP, parent)
{
    reset();
}

QString FERamp::gist(void) const
{
    QString result = ForceEffect::gist();
    result.append(QString("e.u.ramp.start_level = %1; // 0x%2\n")
                  .arg(m_effect.u.ramp.start_level).arg(m_effect.u.ramp.start_level, 0, 16));
    result.append(QString("e.u.ramp.end_level = %1; // 0x%2\n")
                  .arg(m_effect.u.ramp.end_level).arg(m_effect.u.ramp.end_level, 0, 16));
    return result;
}

void FERamp::reset(void)
{
    ForceEffect::reset();
    setStartLevel(0.25);
    setEndLevel(1.0);
}

double FERamp::startLevel(void) const
{
    // TODO: deal with -32768?
    return m_effect.u.ramp.start_level / 32767.0;
}

double FERamp::endLevel(void) const
{
    // TODO: deal with -32768?
    return m_effect.u.ramp.end_level / 32767.0;
}

void FERamp::setStartLevel(double newLevel)
{
    newLevel = CLIP11(newLevel);
    qint16 level = qRound(newLevel * 32767.0);
    if (level != m_effect.u.ramp.start_level)
    {
        m_effect.u.ramp.start_level = level;
        emit valuesChanged();
    }
}

void FERamp::setEndLevel(double newLevel)
{
    newLevel = CLIP11(newLevel);
    qint16 level = qRound(newLevel * 32767.0);
    if (level != m_effect.u.ramp.end_level)
    {
        m_effect.u.ramp.end_level = level;
        emit valuesChanged();
    }
}
