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

#ifndef FWRumble_H
#define FWRumble_H

#include "ForceWidget.h"
#include "FERumble.h"

namespace Ui {
class FWRumble;
}

class FWRumble : public ForceWidget
{
    Q_OBJECT
public:
    explicit FWRumble(QWidget *parent = 0);
    ~FWRumble(void);

    virtual ForceEffect *force(void) const;
    virtual void setForce(ForceEffect *force);

protected slots:
    void onStrongValueChanged(int newLevel);
    void onWeakValueChanged(int newLevel);

    void updateUserInterface(void);

private:
    Ui::FWRumble *ui;
    FERumble *m_force;
};

#endif // FWRumble_H
