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

#include "FEConstant.h"
#include <math.h>

FEConstant::FEConstant(QObject *parent) :
    ForceEffect(FF_CONSTANT, parent)
{
    initializeEnvelope(&m_effect.u.constant.envelope);

    reset();
}

QString FEConstant::gist(void) const
{
    QString result = ForceEffect::gist();
    result.append(QString("e.u.constant.level = %1;\n").arg(m_effect.u.constant.level));
    result.append(envelopeGist(QLatin1String("e.u.constant"), m_effect.u.constant.envelope));
    return result;
}

void FEConstant::reset(void)
{
    setLevel(0.5);
    setAttackLevel(0.25);
    setAttackLength(1.0);
    setFadeLevel(1.0);
    setFadeLength(1.0);
    ForceEffect::reset();
}

double FEConstant::level(void) const
{
    double result = 0.0;
    if (m_effect.u.constant.level < 0)
    {
        result = m_effect.u.constant.level / 32768.0;
    }
    else if (0 < m_effect.u.constant.level)
    {
        result = m_effect.u.constant.level / 32767.0;
    }
    Q_ASSERT(-1.0 <= result);
    Q_ASSERT(result <= 1.0);
    return result;
}

void FEConstant::setLevel(double newLevel)
{
    newLevel = qMax(-1.0, qMin(1.0, newLevel));
    qint16 result = 0;
    if (newLevel < 0.0)
    {
        result = qRound(newLevel * 32768.0f);
    }
    else if (0.0f < newLevel)
    {
        result = qRound(newLevel * 32767.0f);
    }
    if (result != m_effect.u.constant.level)
    {
        m_effect.u.constant.level = result;
        emit levelChanged(newLevel);
        emit valuesChanged();
        update();
    }
}
