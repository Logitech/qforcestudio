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

#ifndef FEPERIODIC_H
#define FEPERIODIC_H

#include "ForceEffect.h"

class FEPeriodic : public ForceEffect
{
    Q_OBJECT
public:
    explicit FEPeriodic(QObject *parent = 0);

    // range: FF_SQUARE ... FF_SAW_DOWN
    int waveform(void) const;

    virtual double level(void) const;

    // range: [-1.0 ... 1.0]
    virtual double offset(void) const;

    virtual QString gist(void) const;
    virtual void reset(void);

    virtual double value(double atTimeIndex) const;

signals:
    void waveformChange(int newWaveform);

public slots:
    virtual void setWaveform(int newWaveform);
    virtual void setLevel(double newLevel);
    virtual void setOffset(double newOffset);
};

#endif // FEPERIODIC_H
