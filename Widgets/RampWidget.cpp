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

#include "RampWidget.h"
#include "ui_RampWidget.h"


RampWidget::RampWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RampWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->startLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onStartLevelChanged(double)));
    connect(ui->endLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onEndLevelChanged(double)));

    ui->startLevelSpinBox->setRange(-1.0, 1.0);
    ui->startLevelSpinBox->setSingleStep(0.05);
    ui->endLevelSpinBox->setRange(-1.0, 1.0);
    ui->endLevelSpinBox->setSingleStep(0.05);
}

RampWidget::~RampWidget(void)
{
    delete ui;
}

void RampWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = qobject_cast<FERamp *>(force);

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(updateUserInterface()));

        updateUserInterface();
    }
}

void RampWidget::onStartLevelChanged(double newLevel)
{
    if (m_force)
    {
        m_force->setStartLevel(newLevel);
    }
}

void RampWidget::onEndLevelChanged(double newLevel)
{
    if (m_force)
    {
        m_force->setEndLevel(newLevel);
    }
}

void RampWidget::updateUserInterface(void)
{
    if (m_force)
    {
        bool wasBlocked = ui->startLevelSpinBox->blockSignals(true);
        ui->startLevelSpinBox->setValue(m_force->startLevel());
        ui->startLevelSpinBox->blockSignals(wasBlocked);
        wasBlocked = ui->endLevelSpinBox->blockSignals(true);
        ui->endLevelSpinBox->setValue(m_force->endLevel());
        ui->endLevelSpinBox->blockSignals(wasBlocked);
    }
}
