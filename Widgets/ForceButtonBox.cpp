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

#include "ForceButtonBox.h"
#include "ui_ForceButtonBox.h"
#include "QForceStudio.h"
#include "MainWindow.h"
#include "CodeGistWidget.h"


ForceButtonBox::ForceButtonBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForceButtonBox),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartForce()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(onStopForce()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(onResetForce()));
    connect(ui->codeButton, SIGNAL(clicked()), this, SLOT(onCodeGist()));
}

ForceButtonBox::~ForceButtonBox(void)
{
    delete ui;
}

void ForceButtonBox::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->codeButton->setEnabled(false);

    m_force = force;

    if (m_force)
    {
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        ui->resetButton->setEnabled(true);
        ui->codeButton->setEnabled(true);
    }
}

void ForceButtonBox::onStartForce(void)
{
    if (m_force)
    {
        m_force->start();
    }
}

void ForceButtonBox::onStopForce(void)
{
    if (m_force)
    {
        m_force->stop();
    }
}

void ForceButtonBox::onResetForce(void)
{
    if (m_force)
    {
        m_force->stop();
        m_force->reset();
    }
}

void ForceButtonBox::onCodeGist(void)
{
    mainWindowInstance()->addForceMdiChild(new CodeGistWidget(), m_force);
}
