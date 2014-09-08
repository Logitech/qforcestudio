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

#include "PeriodFrequencyWidget.h"
#include "ui_PeriodFrequencyWidget.h"
#include <math.h>

PeriodFrequencyWidget::PeriodFrequencyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeriodFrequencyWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->periodSlider, SIGNAL(valueChanged(int)), this, SLOT(onPeriodExpChanged(int)));
    connect(ui->periodSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPeriodChanged(double)));
    connect(ui->frequencySpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFrequencyChanged(double)));

    ui->periodSlider->setRange(-30, 20);
    ui->periodSpinBox->setRange(0.0f, 32.767f);
}

PeriodFrequencyWidget::~PeriodFrequencyWidget(void)
{
    delete ui;
}

void PeriodFrequencyWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(periodChanged(double)), this, SLOT(updatePeriod(double)));

        updatePeriod(m_force->period());
    }
}

void PeriodFrequencyWidget::onPeriodExpChanged(int newPeriodExp)
{
    if (m_force)
    {
        double newPeriod = pow(10.0, newPeriodExp / 10.0);
        m_force->setPeriod(newPeriod);
    }
}

void PeriodFrequencyWidget::onPeriodChanged(double newPeriod)
{
    if (m_force)
    {
        m_force->setPeriod(newPeriod);
    }
}

void PeriodFrequencyWidget::onFrequencyChanged(double newFrequency)
{
    if (m_force)
    {
        newFrequency = qMax(1.0 / 32.0, qMin(1000.0, newFrequency));
        double newPeriod = 1.0 / newFrequency;
        m_force->setPeriod(newPeriod);
    }
}

void PeriodFrequencyWidget::updatePeriod(double requestedPeriod)
{
    bool blocked = ui->periodSpinBox->blockSignals(true);
    ui->periodSpinBox->setValue(requestedPeriod);
    ui->periodSpinBox->blockSignals(blocked);

    int periodExp = qMax(-30, qMin(20, qRound(log10(requestedPeriod) * 10)));
    blocked = ui->periodSlider->blockSignals(true);
    ui->periodSlider->setValue(periodExp);
    ui->periodSlider->blockSignals(blocked);

    double frequency = 1.0 / requestedPeriod;
    blocked = ui->frequencySpinBox->blockSignals(true);
    ui->frequencySpinBox->setValue(frequency);
    ui->frequencySpinBox->blockSignals(blocked);
}
