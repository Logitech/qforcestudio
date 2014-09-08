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

#ifndef ENVELOPEWIDGET_H
#define ENVELOPEWIDGET_H

#include <QWidget>
#include "ForceEffect.h"

namespace Ui {
class EnvelopeWidget;
}


class EnvelopeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EnvelopeWidget(QWidget *parent = 0);
    ~EnvelopeWidget(void);

    void setForce(ForceEffect *force);

protected slots:
    void onAttackLevelChanged(double newLevel);
    void onAttackLengthChanged(double newLength);
    void onEffectLevelChanged(double newLevel);
    void onFadeLevelChanged(double newLevel);
    void onFadeLengthChanged(double newLength);

    void updateUserInterface(void);

private:
    Ui::EnvelopeWidget *ui;
    ForceEffect *m_force;
};

#endif // ENVELOPEWIDGET_H
