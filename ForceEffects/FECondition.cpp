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

#include "FECondition.h"

FECondition::FECondition(quint16 forceType, QObject *parent /* = 0 */) :
    ForceEffect(forceType, parent)
{
    m_firstCondition = new ForceCondition(&m_effect.u.condition[0], this);
    m_secondCondition = new ForceCondition(&m_effect.u.condition[1], this);

    connect(m_firstCondition, SIGNAL(changed()), this, SIGNAL(valuesChanged()));
    connect(m_secondCondition, SIGNAL(changed()), this, SIGNAL(valuesChanged()));
}

ForceCondition *FECondition::firstCondition(void) const
{
    return m_firstCondition;
}

ForceCondition *FECondition::secondCondition(void) const
{
    return m_secondCondition;
}

QString FECondition::gist(void) const
{
    QString result = ForceEffect::gist();
    result.append(m_firstCondition->gist(QLatin1String("e.u.condition[0]")));
    result.append(m_secondCondition->gist(QLatin1String("e.u.condition[1]")));
    return result;
}

void FECondition::reset(void)
{
    ForceEffect::reset();
    m_firstCondition->setIsSymmetric(false);
    m_firstCondition->setNegativeCoefficient(0.7);
    m_firstCondition->setNegativeSaturation(1.0);
    m_firstCondition->setDeadBand(0.1);
    m_firstCondition->setOffset(0.0);
    m_firstCondition->setPositiveCoefficient(0.7);
    m_firstCondition->setPositiveSaturation(1.0);

    m_secondCondition->setIsSymmetric(false);
    m_secondCondition->setNegativeCoefficient(0.7);
    m_secondCondition->setNegativeSaturation(1.0);
    m_secondCondition->setDeadBand(0.1);
    m_secondCondition->setOffset(0.0);
    m_secondCondition->setPositiveCoefficient(0.7);
    m_secondCondition->setPositiveSaturation(1.0);
}
