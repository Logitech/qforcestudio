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

#include "DurationWidget.h"
#include "ui_DurationWidget.h"
#include <QDebug>

DurationWidget::DurationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DurationWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->durationSlider, SIGNAL(delayChanged(double)), this, SLOT(onDelayChanged(double)));
    connect(ui->durationSlider, SIGNAL(durationChanged(double)), this, SLOT(onDurationChanged(double)));
    connect(ui->durationSlider, SIGNAL(infiniteDurationChanged(bool)), this, SLOT(onDurationIsInfiniteChanged(bool)));
    connect(ui->durationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onDurationChanged(double)));
    connect(ui->infiniteCheckBox, SIGNAL(clicked(bool)), this, SLOT(onDurationIsInfiniteChanged(bool)));

    ui->durationSpinBox->setRange(0.0, 32.767);
    ui->durationSpinBox->setSingleStep(0.1);
}

DurationWidget::~DurationWidget(void)
{
    delete ui;
}

void DurationWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(delayChanged(double)), this, SLOT(updateDelay(double)));
        connect(m_force, SIGNAL(durationChanged(double)), this, SLOT(updateDuration(double)));
        connect(m_force, SIGNAL(durationIsInfiniteChanged(bool)), this, SLOT(updateInfiniteDuration(bool)));

        updateDuration(m_force->duration());
        updateInfiniteDuration(m_force->isInfiniteDuration());
    }
}

void DurationWidget::onDelayChanged(double newDelay)
{
    if (m_force)
    {
        m_force->setDelay(newDelay);
    }
}

void DurationWidget::onDurationChanged(double newDuration)
{
    if (m_force)
    {
        m_force->setDuration(newDuration);
    }
}

void DurationWidget::onDurationIsInfiniteChanged(bool isNowInfinite)
{
    if (m_force)
    {
        m_force->setDuration(m_force->duration(), isNowInfinite);
    }
}

void DurationWidget::updateDelay(double requestedDelay)
{
    ui->durationSlider->updateDelay(requestedDelay);
}

void DurationWidget::updateDuration(double requestedDuration)
{
    ui->durationSlider->updateDuration(requestedDuration);
    ui->durationSpinBox->setValue(requestedDuration);
}

void DurationWidget::updateInfiniteDuration(bool isNowInfinite)
{
    ui->durationSlider->updateInfiniteDuration(isNowInfinite);
    ui->durationSpinBox->setEnabled(!isNowInfinite);
    ui->infiniteCheckBox->setChecked(isNowInfinite);
}
