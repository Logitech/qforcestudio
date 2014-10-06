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

#include "FWCondition.h"
#include "ui_FWCondition.h"

FWCondition::FWCondition(QWidget *parent) :
    ForceWidget(parent),
    ui(new Ui::FWCondition),
    m_force(NULL)
{
    ui->setupUi(this);

    ui->secondCondition->disableBothAxesCheckBox();

    connect(ui->firstCondition, SIGNAL(bothAxesChanged(int)), this, SLOT(onBothAxesChanged(int)));
}

FWCondition::~FWCondition(void)
{
    delete ui;
}

void FWCondition::setForceName(const QString &forceName, const QString &xAxisLabel)
{
    setWindowTitle(forceName);
    ui->titleLabel->setText(forceName);

    m_xAxisLabel = xAxisLabel;
}

ForceEffect *FWCondition::force(void) const
{
    return m_force;
}

void FWCondition::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
        ui->firstCondition->setForceCondition(NULL);
        ui->secondCondition->setForceCondition(NULL);
    }

    m_force = qobject_cast<FECondition *>(force);
    ui->forceButtons->setForce(m_force);
    ui->durationWidget->setForce(m_force);

    if (m_force)
    {
        ui->firstGraph->setForceCondition(m_force->firstCondition(), m_xAxisLabel);
        ui->firstCondition->setForceCondition(m_force->firstCondition());
        ui->secondGraph->setForceCondition(m_force->secondCondition(), m_xAxisLabel);
        ui->secondCondition->setForceCondition(m_force->secondCondition());

        connect(m_force->firstCondition(), SIGNAL(changed()), this, SLOT(onFirstConditionChanged()));
    }
}

void FWCondition::onBothAxesChanged(int newState)
{
    bool bothAxesChecked = (Qt::Checked == newState);

    if (bothAxesChecked)
    {
        ui->secondCondition->setEnabled(false);
        m_force->secondCondition()->copyFrom(m_force->firstCondition());
    }
    else
    {
        ui->secondCondition->setEnabled(true);
    }
}

void FWCondition::onFirstConditionChanged(void)
{
    if (ui->firstCondition->bothAxesChecked())
    {
        if (m_force && m_force->firstCondition() && m_force->secondCondition())
        {
            m_force->secondCondition()->copyFrom(m_force->firstCondition());
        }
    }
}
