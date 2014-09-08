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

#include "FWConstant.h"
#include "ui_FWConstant.h"
#include "FEConstant.h"

FWConstant::FWConstant(QWidget *parent) :
    ForceWidget(parent),
    ui(new Ui::FWConstant),
    m_force(NULL)
{
    ui->setupUi(this);
}

FWConstant::~FWConstant(void)
{
    delete ui;
}

ForceEffect *FWConstant::force(void) const
{
    return m_force;
}

void FWConstant::setForce(ForceEffect *force)
{
    m_force = qobject_cast<FEConstant *>(force);
    ui->forceButtons->setForce(m_force);
    ui->directionWidget->setForce(m_force);
    ui->forceGraph->setForce(m_force);
    ui->envelopeWidget->setForce(m_force);
    ui->durationWidget->setForce(m_force);
}
