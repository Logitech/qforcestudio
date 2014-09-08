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

#include "ConditionWidget.h"
#include "ui_ConditionWidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionWidget),
    m_forceCondition(NULL)
{
    ui->setupUi(this);

    ui->negativeCoefficientSpinBox->setRange(-1.0, 1.0);
    ui->negativeCoefficientSpinBox->setSingleStep(0.05);
    ui->positiveCoefficientSpinBox->setRange(-1.0, 1.0);
    ui->positiveCoefficientSpinBox->setSingleStep(0.05);
    ui->negativeSaturationSpinBox->setRange(0.0, 1.0);
    ui->negativeSaturationSpinBox->setSingleStep(0.05);
    ui->positiveSaturationSpinBox->setRange(0.0, 1.0);
    ui->positiveSaturationSpinBox->setSingleStep(0.05);
    ui->deadBandSpinBox->setRange(0.0, 1.0);
    ui->deadBandSpinBox->setSingleStep(0.05);
    ui->offsetSpinBox->setRange(-1.0, 1.0);
    ui->offsetSpinBox->setSingleStep(0.05);

    connect(ui->negativeCoefficientSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onNegativeCoefficientChanged(double)));
    connect(ui->positiveCoefficientSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPositiveCoefficientChanged(double)));
    connect(ui->negativeSaturationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onNegativeSaturationChanged(double)));
    connect(ui->positiveSaturationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onPositiveSaturationChanged(double)));
    connect(ui->deadBandSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onDeadBandChanged(double)));
    connect(ui->offsetSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onOffsetChanged(double)));
    connect(ui->bothAxesCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(bothAxesChanged(int)));
    connect(ui->symmetricCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onSymmetricChanged(int)));
}

ConditionWidget::~ConditionWidget(void)
{
    delete ui;
}

void ConditionWidget::setForceCondition(ForceCondition *forceCondition)
{
    if (m_forceCondition)
    {
        disconnect(m_forceCondition, 0, this, 0);
    }

    m_forceCondition = forceCondition;

    if (m_forceCondition)
    {
        connect(m_forceCondition, SIGNAL(changed()), this, SLOT(updateCondition()));
        updateCondition();
    }
}

void ConditionWidget::disableBothAxesCheckBox(void)
{
    ui->bothAxesCheckBox->setVisible(false);
}

bool ConditionWidget::bothAxesChecked(void) const
{
    return (Qt::Checked == ui->bothAxesCheckBox->checkState());
}

void ConditionWidget::onNegativeCoefficientChanged(double newCoefficient)
{
    if (m_forceCondition)
    {
        m_forceCondition->setNegativeCoefficient(newCoefficient);
    }
}

void ConditionWidget::onPositiveCoefficientChanged(double newCoefficient)
{
    if (m_forceCondition)
    {
        m_forceCondition->setPositiveCoefficient(newCoefficient);
    }
}

void ConditionWidget::onNegativeSaturationChanged(double newSaturation)
{
    if (m_forceCondition)
    {
        m_forceCondition->setNegativeSaturation(newSaturation);
    }
}

void ConditionWidget::onPositiveSaturationChanged(double newSaturation)
{
    if (m_forceCondition)
    {
        m_forceCondition->setPositiveSaturation(newSaturation);
    }
}

void ConditionWidget::onDeadBandChanged(double newDeadBand)
{
    if (m_forceCondition)
    {
        m_forceCondition->setDeadBand(newDeadBand);
    }
}

void ConditionWidget::onOffsetChanged(double newOffset)
{
    if (m_forceCondition)
    {
        m_forceCondition->setOffset(newOffset);
    }
}

void ConditionWidget::onSymmetricChanged(int newState)
{
    if (m_forceCondition)
    {
        m_forceCondition->setIsSymmetric(newState == Qt::Checked);
    }
}

void ConditionWidget::updateCondition(void)
{
    if (m_forceCondition)
    {
        bool wasBlocked = ui->negativeCoefficientSpinBox->blockSignals(true);
        ui->negativeCoefficientSpinBox->setValue(m_forceCondition->negativeCoefficient());
        ui->negativeCoefficientSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->positiveCoefficientSpinBox->blockSignals(true);
        ui->positiveCoefficientSpinBox->setValue(m_forceCondition->positiveCoefficient());
        ui->positiveCoefficientSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->negativeSaturationSpinBox->blockSignals(true);
        ui->negativeSaturationSpinBox->setValue(m_forceCondition->negativeSaturation());
        ui->negativeSaturationSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->positiveSaturationSpinBox->blockSignals(true);
        ui->positiveSaturationSpinBox->setValue(m_forceCondition->positiveSaturation());
        ui->positiveSaturationSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->deadBandSpinBox->blockSignals(true);
        ui->deadBandSpinBox->setValue(m_forceCondition->deadBand());
        ui->deadBandSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->offsetSpinBox->blockSignals(true);
        ui->offsetSpinBox->setValue(m_forceCondition->offset());
        ui->offsetSpinBox->blockSignals(wasBlocked);

        wasBlocked = ui->symmetricCheckBox->blockSignals(true);
        ui->symmetricCheckBox->setChecked(m_forceCondition->isSymmetric());
        ui->symmetricCheckBox->blockSignals(wasBlocked);

        ui->positiveCoefficientSpinBox->setEnabled(!m_forceCondition->isSymmetric());
        ui->positiveSaturationSpinBox->setEnabled(!m_forceCondition->isSymmetric());
        ui->offsetSpinBox->setEnabled(!m_forceCondition->isSymmetric());
    }
}
