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

#include "FERumble.h"

FERumble::FERumble(QObject *parent) :
    ForceEffect(FF_RUMBLE, parent)
{
    reset();
}

QString FERumble::gist(void) const
{
    QString result = ForceEffect::gist();
    result.append(QString("e.u.rumble.strong_magnitude = %1; // %2\n")
                  .arg(m_effect.u.rumble.strong_magnitude).arg(m_effect.u.rumble.strong_magnitude));
    result.append(QString("e.u.rumble.weak_magnitude = %1; // %2\n")
                  .arg(m_effect.u.rumble.weak_magnitude).arg(m_effect.u.rumble.weak_magnitude));
    return result;
}

int FERumble::strongLevel(void) const
{
    int result = qRound(m_effect.u.rumble.strong_magnitude / 655.35f);
    return result;
}

int FERumble::weakLevel(void) const
{
    int result = qRound(m_effect.u.rumble.weak_magnitude / 655.35f);
    return result;
}

void FERumble::reset(void)
{
    setStrongLevel(50);
    setWeakLevel(50);
    ForceEffect::reset();
}

void FERumble::setStrongLevel(int newLevel)
{
    newLevel = qMax(0, qMin(100, newLevel));
    quint16 magnitude = qRound(655.35f * newLevel);
    if (magnitude != m_effect.u.rumble.strong_magnitude)
    {
        m_effect.u.rumble.strong_magnitude = magnitude;
        emit valuesChanged();
        update();
    }
}

void FERumble::setWeakLevel(int newLevel)
{
    newLevel = qMax(0, qMin(100, newLevel));
    quint16 magnitude = qRound(655.35f * newLevel);
    if (magnitude != m_effect.u.rumble.weak_magnitude)
    {
        m_effect.u.rumble.weak_magnitude = magnitude;
        emit valuesChanged();
        update();
    }
}
