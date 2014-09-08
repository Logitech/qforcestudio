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

#include "QForceStudio.h"
#include "DirectionWidget.h"
#include "ui_DirectionWidget.h"
#include "FEConstant.h"

DirectionWidget::DirectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirectionWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->angleWidget, SIGNAL(angleChanged(double)), this, SLOT(onDirectionRadChanged(double)));
    connect(ui->directionRadSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onDirectionRadChanged(double)));
    connect(ui->directionDegSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDirectionDegChanged(int)));

    ui->directionRadSpinBox->setRange(0.0, M_TAU);
    ui->directionRadSpinBox->setSingleStep(0.1);

    ui->directionDegSpinBox->setRange(0, 359);
    ui->directionDegSpinBox->setSingleStep(1.0);
}

DirectionWidget::~DirectionWidget(void)
{
    delete ui;
}

void DirectionWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(directionChanged(double)), this, SLOT(updateDirection(double)));

        updateDirection(m_force->direction());
    }
}

void DirectionWidget::updateDirection(double requestedDirection)
{
    ui->angleWidget->setAngle(requestedDirection);
    ui->directionRadSpinBox->setValue(requestedDirection);
    ui->directionDegSpinBox->setValue(convertRad2Deg(requestedDirection));
}

void DirectionWidget::onDirectionRadChanged(double newDirection)
{
    if (m_force)
    {
        m_force->setDirection(newDirection);
    }
}

void DirectionWidget::onDirectionDegChanged(int newDirectionInDeg)
{
    if (m_force)
    {
        m_force->setDirection(convertDeg2Rad(newDirectionInDeg));
    }
}
