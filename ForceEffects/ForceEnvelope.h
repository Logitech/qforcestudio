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

#ifndef FORCEENVELOPE_H
#define FORCEENVELOPE_H

#include <QObject>
#include <linux/input.h>

class ForceEnvelope : public QObject
{
    Q_OBJECT
public:
    explicit ForceEnvelope(ff_envelope *envelope, QObject *parent = 0);

    bool exists(void) const;

    // as coefficient [0.0 ... 1.0]
    double attackLevel(void) const;

    // in seconds [0.0 ... 32.767]
    double attackLength(void) const;

    // as coefficient [0.0 ... 1.0]
    double fadeLevel(void) const;

    // in seconds [0.0 ... 32.767]
    double fadeLength(void) const;

    // returns ratio as coefficient [0.0 ... 1.0] of being inside
    // the envelope levels.
    bool withinAttackLength(double atTimeIndex, double &ratio) const;
    bool withinFadeLength(double atTimeIndex, double totalLength, double &ratio) const;

signals:
    void envelopeChanged(void);

public slots:
    void setAttackLevel(double newLevel);
    void setAttackLength(double newLength);
    void setFadeLevel(double newLevel);
    void setFadeLength(double newLength);

protected:
    ff_envelope *m_envelope;
};

#endif // FORCEENVELOPE_H
