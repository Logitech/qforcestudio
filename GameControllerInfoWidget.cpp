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

#include "GameControllerInfoWidget.h"
#include "ui_GameControllerInfoWidget.h"

GameControllerInfoWidget::GameControllerInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameControllerInfoWidget)
{
    ui->setupUi(this);
}

GameControllerInfoWidget::~GameControllerInfoWidget(void)
{
    delete ui;
}

void GameControllerInfoWidget::setController(GameControllerPtr controller)
{
    m_controller = controller;

    ui->deviceNameLabel->setText(m_controller->name());
    ui->devicePathLabel->setText(m_controller->filename());
    ui->forceEffectSlotsLabel->setText(QString::number(m_controller->numEffects()));

    ui->canRenderConstant->setChecked(m_controller->canRenderConstant());
    ui->canRenderPeriodic->setChecked(m_controller->canRenderPeriodic());
    ui->canRenderSquare->setChecked(m_controller->canRenderSquare());
    ui->canRenderTriangle->setChecked(m_controller->canRenderTriangle());
    ui->canRenderSine->setChecked(m_controller->canRenderSine());
    ui->canRenderSawUp->setChecked(m_controller->canRenderSawUp());
    ui->canRenderSawDown->setChecked(m_controller->canRenderSawDown());
    ui->canRenderCustom->setChecked(m_controller->canRenderCustom());
    ui->canRenderRamp->setChecked(m_controller->canRenderRamp());
    ui->canRenderSpring->setChecked(m_controller->canRenderSpring());
    ui->canRenderFriction->setChecked(m_controller->canRenderFriction());
    ui->canRenderDamper->setChecked(m_controller->canRenderDamper());
    ui->canRenderRumble->setChecked(m_controller->canRenderRumble());
    ui->canRenderInertia->setChecked(m_controller->canRenderInertia());
    ui->canAdjustGain->setChecked(m_controller->canAdjustGain());
    ui->canAdjustAutocenter->setChecked(m_controller->canAdjustAutocenter());

    ui->canRenderConstant->setEnabled(false);
    ui->canRenderPeriodic->setEnabled(false);
    ui->canRenderSquare->setEnabled(false);
    ui->canRenderTriangle->setEnabled(false);
    ui->canRenderSine->setEnabled(false);
    ui->canRenderSawUp->setEnabled(false);
    ui->canRenderSawDown->setEnabled(false);
    ui->canRenderCustom->setEnabled(false);
    ui->canRenderRamp->setEnabled(false);
    ui->canRenderSpring->setEnabled(false);
    ui->canRenderFriction->setEnabled(false);
    ui->canRenderDamper->setEnabled(false);
    ui->canRenderRumble->setEnabled(false);
    ui->canRenderInertia->setEnabled(false);
    ui->canAdjustGain->setEnabled(false);
    ui->canAdjustAutocenter->setEnabled(false);
}
