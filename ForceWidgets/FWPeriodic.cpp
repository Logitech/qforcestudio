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

#include "FWPeriodic.h"
#include "ui_FWPeriodic.h"
#include "QForceStudio.h"

FWPeriodic::FWPeriodic(QWidget *parent) :
    ForceWidget(parent),
    ui(new Ui::FWPeriodic),
    m_force(NULL)
{
    ui->setupUi(this);

    ui->waveformComboBox->addItem(tr("Square"), QVariant(FF_SQUARE));
    ui->waveformComboBox->addItem(tr("Triangle"), QVariant(FF_TRIANGLE));
    ui->waveformComboBox->addItem(tr("Sine"), QVariant(FF_SINE));
    ui->waveformComboBox->addItem(tr("Sawtooth Up"), QVariant(FF_SAW_UP));
    ui->waveformComboBox->addItem(tr("Sawtooth Down"), QVariant(FF_SAW_DOWN));
    connect(ui->waveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onWaveformChanged(int)));

    ui->offsetSpinBox->setRange(-1.0, 1.0);
    ui->offsetSpinBox->setSingleStep(0.05);
    connect(ui->offsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onOffsetChanged(double)));

    ui->phaseSpinBox->setRange(0.0, 1.0);
    ui->phaseSpinBox->setSingleStep(0.05);
    connect(ui->phaseSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPhaseChanged(double)));

    ui->directionSpinBox->setRange(0.0, 359.0);
    ui->directionSpinBox->setSingleStep(1.0);
    connect(ui->directionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onDirectionChanged(double)));

    ui->periodSpinBox->setRange(0.001, 100);
    ui->periodSpinBox->setSingleStep(0.1);
    connect(ui->periodSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPeriodChanged(double)));
}

FWPeriodic::~FWPeriodic(void)
{
    delete ui;
}

ForceEffect *FWPeriodic::force(void) const
{
    return m_force;
}

void FWPeriodic::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = qobject_cast<FEPeriodic *>(force);
    ui->forceButtons->setForce(m_force);
    ui->graphWidget->setForce(m_force);
    ui->envelopeWidget->setForce(m_force);
    ui->durationWidget->setForce(m_force);

    if (m_force)
    {
        connect(m_force, SIGNAL(waveformChange(int)), this, SLOT(updateWaveform(int)));
        connect(m_force, SIGNAL(offsetChanged(double)), this, SLOT(updateOffset(double)));
        connect(m_force, SIGNAL(phaseChanged(double)), this, SLOT(updatePhase(double)));
        connect(m_force, SIGNAL(directionChanged(double)), this, SLOT(updateDirection(double)));
        connect(m_force, SIGNAL(periodChanged(double)), this, SLOT(updatePeriod(double)));

        updateWaveform(m_force->waveform());
        updateOffset(m_force->offset());
        updatePhase(m_force->phase());
        updateDirection(m_force->direction());
        updatePeriod(m_force->period());
    }
}

void FWPeriodic::onWaveformChanged(int newWaveformIndex)
{
    QVariant newWaveform = ui->waveformComboBox->itemData(newWaveformIndex);
    if (m_force)
    {
        m_force->setWaveform(newWaveform.toInt());
    }
}

void FWPeriodic::onOffsetChanged(double newOffset)
{
    if (m_force)
    {
        m_force->setOffset(newOffset);
    }
}

void FWPeriodic::onPhaseChanged(double newPhase)
{
    if (m_force)
    {
        m_force->setPhase(newPhase);
    }
}

void FWPeriodic::onDirectionChanged(double newDirection)
{
    if (m_force)
    {
        m_force->setDirection(convertDeg2Rad(newDirection));
    }
}

void FWPeriodic::onPeriodChanged(double newPeriod)
{
    if (m_force)
    {
        m_force->setPeriod(newPeriod);
    }
}

void FWPeriodic::updateWaveform(int requestedWaveform)
{
    for(int index = 0; index < ui->waveformComboBox->count(); ++index)
    {
        QVariant itemData = ui->waveformComboBox->itemData(index);
        if (itemData.toInt() == requestedWaveform)
        {
            bool wasBlocked = ui->waveformComboBox->blockSignals(true);
            ui->waveformComboBox->setCurrentIndex(index);
            ui->waveformComboBox->blockSignals(wasBlocked);
            return;
        }
    }
}

void FWPeriodic::updateOffset(double requestedOffset)
{
    bool wasBlocked = ui->offsetSpinBox->blockSignals(true);
    ui->offsetSpinBox->setValue(requestedOffset);
    ui->offsetSpinBox->blockSignals(wasBlocked);
}

void FWPeriodic::updatePhase(double requestedPhase)
{
    bool wasBlocked = ui->phaseSpinBox->blockSignals(true);
    ui->phaseSpinBox->setValue(requestedPhase);
    ui->phaseSpinBox->blockSignals(wasBlocked);
}

void FWPeriodic::updateDirection(double requestedDirection)
{
    bool wasBlocked = ui->directionSpinBox->blockSignals(true);
    ui->directionSpinBox->setValue(convertRad2Deg(requestedDirection));
    ui->directionSpinBox->blockSignals(wasBlocked);
}

void FWPeriodic::updatePeriod(double requestedPeriod)
{
    bool wasBlocked = ui->periodSpinBox->blockSignals(true);
    ui->periodSpinBox->setValue(requestedPeriod);
    ui->periodSpinBox->blockSignals(wasBlocked);
}
