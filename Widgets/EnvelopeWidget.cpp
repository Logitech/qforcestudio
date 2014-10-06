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

#include "EnvelopeWidget.h"
#include "ui_EnvelopeWidget.h"

EnvelopeWidget::EnvelopeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnvelopeWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->attackLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAttackLevelChanged(double)));
    connect(ui->attackLengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAttackLengthChanged(double)));
    connect(ui->effectLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onEffectLevelChanged(double)));
    connect(ui->fadeLevelSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onFadeLevelChanged(double)));
    connect(ui->fadeLengthSpingBox, SIGNAL(valueChanged(double)), this, SLOT(onFadeLengthChanged(double)));

    ui->attackLevelSpinBox->setRange(0.0, 1.0);
    ui->attackLevelSpinBox->setSingleStep(0.05);
    ui->attackLengthSpinBox->setRange(0.0, 32.767);
    ui->attackLengthSpinBox->setSingleStep(0.1);
    ui->effectLevelSpinBox->setRange(0.0, 1.0);
    ui->effectLevelSpinBox->setSingleStep(0.05);
    ui->fadeLevelSpinBox->setRange(0.0, 1.0);
    ui->fadeLevelSpinBox->setSingleStep(0.05);
    ui->fadeLengthSpingBox->setRange(0.0, 32.767);
    ui->fadeLengthSpingBox->setSingleStep(0.1);
}

EnvelopeWidget::~EnvelopeWidget(void)
{
    delete ui;
}

void EnvelopeWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(updateUserInterface()));

        updateUserInterface();
    }
}

void EnvelopeWidget::onAttackLevelChanged(double newLevel)
{
    if (m_force)
    {
        m_force->setAttackLevel(newLevel);
    }
}

void EnvelopeWidget::onAttackLengthChanged(double newLength)
{
    if (m_force)
    {
        m_force->setAttackLength(newLength);
    }
}

void EnvelopeWidget::onEffectLevelChanged(double newLevel)
{
    if (m_force)
    {
        m_force->setLevel(newLevel);
    }
}

void EnvelopeWidget::onFadeLevelChanged(double newLevel)
{
    if (m_force)
    {
        m_force->setFadeLevel(newLevel);
    }
}

void EnvelopeWidget::onFadeLengthChanged(double newLength)
{
    if (m_force)
    {
        m_force->setFadeLength(newLength);
    }
}

void EnvelopeWidget::updateUserInterface(void)
{
    if (m_force)
    {
        bool wasBlocked = ui->attackLevelSpinBox->blockSignals(true);
        ui->attackLevelSpinBox->setValue(m_force->attackLevel());
        ui->attackLevelSpinBox->blockSignals(wasBlocked);
        wasBlocked = ui->attackLengthSpinBox->blockSignals(true);
        ui->attackLengthSpinBox->setValue(m_force->attackLength());
        ui->attackLengthSpinBox->blockSignals(wasBlocked);
        wasBlocked = ui->fadeLevelSpinBox->blockSignals(true);
        ui->fadeLevelSpinBox->setValue(m_force->fadeLevel());
        ui->fadeLevelSpinBox->blockSignals(wasBlocked);
        wasBlocked = ui->fadeLengthSpingBox->blockSignals(true);
        ui->fadeLengthSpingBox->setValue(m_force->fadeLength());
        ui->fadeLengthSpingBox->blockSignals(wasBlocked);
        wasBlocked = ui->effectLevelSpinBox->blockSignals(true);
        ui->effectLevelSpinBox->setValue(m_force->level());
        ui->effectLevelSpinBox->blockSignals(wasBlocked);

        bool isInfinite = m_force->isInfiniteDuration();
        ui->fadeLevelSpinBox->setEnabled(!isInfinite);
        ui->fadeLengthSpingBox->setEnabled(!isInfinite);
    }
}
