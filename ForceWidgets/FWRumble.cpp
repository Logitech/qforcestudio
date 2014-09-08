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

#include "FWRumble.h"
#include "ui_FWRumble.h"

FWRumble::FWRumble(QWidget *parent) :
    ForceWidget(parent),
    ui(new Ui::FWRumble),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->strongDial, SIGNAL(valueChanged(int)), this, SLOT(onStrongValueChanged(int)));
    connect(ui->strongSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onStrongValueChanged(int)));
    connect(ui->weakDial, SIGNAL(valueChanged(int)), this, SLOT(onWeakValueChanged(int)));
    connect(ui->weakSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onWeakValueChanged(int)));
}

FWRumble::~FWRumble(void)
{
    delete ui;
}

ForceEffect *FWRumble::force(void) const
{
    return m_force;
}

void FWRumble::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = qobject_cast<FERumble *>(force);
    ui->forceButtons->setForce(m_force);
    ui->durationWidget->setForce(m_force);

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(updateUserInterface()));

        updateUserInterface();
    }
}

void FWRumble::onStrongValueChanged(int newLevel)
{
    if (m_force)
    {
        m_force->setStrongLevel(newLevel);
    }
}

void FWRumble::onWeakValueChanged(int newLevel)
{
    if (m_force)
    {
        m_force->setWeakLevel(newLevel);
    }
}

void FWRumble::updateUserInterface(void)
{
    bool wasBlocked = ui->strongDial->blockSignals(true);
    ui->strongDial->setValue(m_force->strongLevel());
    ui->strongDial->blockSignals(wasBlocked);
    wasBlocked = ui->strongSpinBox->blockSignals(true);
    ui->strongSpinBox->setValue(m_force->strongLevel());
    ui->strongSpinBox->blockSignals(wasBlocked);
    wasBlocked = ui->weakDial->blockSignals(true);
    ui->weakDial->setValue(m_force->weakLevel());
    ui->weakDial->blockSignals(wasBlocked);
    wasBlocked = ui->weakSpinBox->blockSignals(true);
    ui->weakSpinBox->setValue(m_force->weakLevel());
    ui->weakSpinBox->blockSignals(wasBlocked);
}
