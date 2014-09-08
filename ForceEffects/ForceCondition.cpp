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

#include "ForceCondition.h"

ForceCondition::ForceCondition(ff_condition_effect *condition, QObject *parent) :
    QObject(parent),
    m_conditionEffect(condition)
{
    // don't allow construction, if there is no underlying structure!
    Q_ASSERT(m_conditionEffect);
}

double ForceCondition::negativeCoefficient(void) const
{
    // TODO: handle -32768?
    return m_conditionEffect->left_coeff / 32767.0;
}

double ForceCondition::positiveCoefficient(void) const
{
    // TODO: handle -32768?
    return m_conditionEffect->right_coeff / 32767.0;
}

double ForceCondition::negativeSaturation(void) const
{
    return m_conditionEffect->left_saturation / 65535.0;
}

double ForceCondition::positiveSaturation(void) const
{
    return m_conditionEffect->right_saturation / 65535.0;
}

double ForceCondition::deadBand(void) const
{
    return m_conditionEffect->deadband / 65535.0;
}

double ForceCondition::offset(void) const
{
    // TODO: handle -32768?
    return m_conditionEffect->center / 32767.0;
}

bool ForceCondition::isSymmetric(void) const
{
    return m_isSymmetric;
}

void ForceCondition::copyFrom(ForceCondition *other)
{
    m_conditionEffect = other->m_conditionEffect;
    emit changed();
}

QString ForceCondition::gist(const QString &prefix) const
{
    QString result;
    result.append(QString("%1.right_saturation = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->right_saturation));
    result.append(QString("%1.left_saturation = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->left_saturation));
    result.append(QString("%1.right_coeff = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->right_coeff));
    result.append(QString("%1.left_coeff = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->left_coeff));
    result.append(QString("%1.deadband = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->deadband));
    result.append(QString("%1.center = %2;\n")
                  .arg(prefix).arg(m_conditionEffect->center));
    return result;
}

void ForceCondition::setNegativeCoefficient(double newCoefficient)
{
    newCoefficient = qMax(-1.0, qMin(1.0, newCoefficient));
    int coeff = qRound(newCoefficient * 32767.0);
    if (m_conditionEffect->left_coeff != coeff)
    {
        m_conditionEffect->left_coeff = coeff;
        emit changed();
    }

    if (isSymmetric())
    {
        setPositiveCoefficient(newCoefficient);
    }
}

void ForceCondition::setPositiveCoefficient(double newCoefficient)
{
    newCoefficient = qMax(-1.0, qMin(1.0, newCoefficient));
    int coeff = qRound(newCoefficient * 32767.0);
    if (m_conditionEffect->right_coeff != coeff)
    {
        m_conditionEffect->right_coeff = coeff;
        emit changed();
    }
}

void ForceCondition::setNegativeSaturation(double newSaturation)
{
    newSaturation = qMax(0.0, qMin(1.0, newSaturation));
    int saturation = qRound(newSaturation * 65535.0);
    if (m_conditionEffect->left_saturation != saturation)
    {
        m_conditionEffect->left_saturation = saturation;
        emit changed();
    }

    if (isSymmetric())
    {
        setPositiveSaturation(newSaturation);
    }
}

void ForceCondition::setPositiveSaturation(double newSaturation)
{
    newSaturation = qMax(0.0, qMin(1.0, newSaturation));
    int saturation = qRound(newSaturation * 65535.0);
    if (m_conditionEffect->right_saturation != saturation)
    {
        m_conditionEffect->right_saturation = saturation;
        emit changed();
    }
}

void ForceCondition::setDeadBand(double newDeadBand)
{
    newDeadBand = qMax(0.0, qMin(1.0, newDeadBand));
    int deadBand = qRound(newDeadBand * 65535.0);
    if (m_conditionEffect->deadband != deadBand)
    {
        m_conditionEffect->deadband = deadBand;
        emit changed();
    }
}

void ForceCondition::setOffset(double newOffset)
{
    newOffset = qMax(-1.0, qMin(1.0, newOffset));
    if (isSymmetric())
    {
        newOffset = 0.0;
    }
    int offset = qRound(newOffset * 32767.0);
    if (m_conditionEffect->center != offset)
    {
        m_conditionEffect->center = offset;
        emit changed();
    }
}

void ForceCondition::setIsSymmetric(bool isNowSymmetric)
{
    if (isNowSymmetric != m_isSymmetric)
    {
        m_isSymmetric = isNowSymmetric;

        if (m_isSymmetric)
        {
            m_conditionEffect->right_coeff = m_conditionEffect->left_coeff;
            m_conditionEffect->right_saturation = m_conditionEffect->left_saturation;
            m_conditionEffect->center = 0;
        }
        emit changed();
    }
}
