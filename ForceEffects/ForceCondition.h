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

#ifndef FORCECONDITION_H
#define FORCECONDITION_H

#include <QObject>
#include <linux/input.h>

class ForceCondition : public QObject
{
    Q_OBJECT
public:
    explicit ForceCondition(ff_condition_effect *condition, QObject *parent = 0);

    // as coefficient [-1.0 ... 1.0]
    double negativeCoefficient(void) const;
    double positiveCoefficient(void) const;

    // as coefficient [0.0 ... 1.0]
    double negativeSaturation(void) const;
    double positiveSaturation(void) const;

    // as coefficient [0.0 ... 1.0]
    double deadBand(void) const;

    // as coefficient [-1.0 ... 1.0]
    double offset(void) const;

    // helper functionality
    bool isSymmetric(void) const;
    void copyFrom(ForceCondition *other);
    QString gist(const QString &prefix) const;

signals:
    void changed(void);

public slots:
    void setNegativeCoefficient(double newCoefficient);
    void setPositiveCoefficient(double newCoefficient);
    void setNegativeSaturation(double newSaturation);
    void setPositiveSaturation(double newSaturation);
    void setDeadBand(double newDeadBand);
    void setOffset(double newOffset);
    void setIsSymmetric(bool isNowSymmetric);

protected:
    ff_condition_effect *m_conditionEffect;
    bool m_isSymmetric;
};

#endif // FORCECONDITION_H
