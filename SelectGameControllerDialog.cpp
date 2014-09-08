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

#include "SelectGameControllerDialog.h"
#include "ui_SelectGameControllerDialog.h"
#include "GameController.h"

SelectGameControllerDialog::SelectGameControllerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectGameControllerDialog),
    m_gameController(NULL)
{
    ui->setupUi(this);

    connect(ui->controllersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onControllerIndexChanged(int)));

    fillControllerList();
}

SelectGameControllerDialog::~SelectGameControllerDialog()
{
    delete ui;
}

void SelectGameControllerDialog::setGameController(GameControllerPtr controller)
{
    m_gameController = controller;
}

GameControllerPtr SelectGameControllerDialog::gameController(void) const
{
    return m_gameController;
}

void SelectGameControllerDialog::fillControllerList(void)
{
    ui->controllersComboBox->clear();

    int selectedIndex = -1;
    GameControllerPtrList list = GameController::allControllers();
    foreach (GameControllerPtr controller, list)
    {
        ui->controllersComboBox->addItem(controller->name(),
                                         controller->filename());
        if (m_gameController)
        {
            if (m_gameController->filename() == controller->filename())
            {
                selectedIndex = ui->controllersComboBox->count();
            }
        }
    }
    if (-1 != selectedIndex)
    {
        ui->controllersComboBox->setCurrentIndex(selectedIndex);
    }
}

void SelectGameControllerDialog::onRefreshList(void)
{
    fillControllerList();
}

GameControllerPtr SelectGameControllerDialog::controllerForIndex(int index)
{
    GameControllerPtr result;
    QVariant itemData = ui->controllersComboBox->itemData(index);
    if (!itemData.isNull())
    {
        QString controllerFilename = itemData.toString();
        foreach (GameControllerPtr controller, GameController::allControllers())
        {
            if (controller->filename() == controllerFilename)
            {
                result = controller;
                break;
            }
        }
    }
    return result;
}


void SelectGameControllerDialog::onControllerIndexChanged(int newIndex)
{
    ui->controllerInfoWidget->setController(controllerForIndex(newIndex));
}

void SelectGameControllerDialog::accept(void)
{
    int index = ui->controllersComboBox->currentIndex();
    m_gameController = controllerForIndex(index);

    QDialog::accept();
}
