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

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QSettings>
#include <QMdiSubWindow>
#include "SelectGameControllerDialog.h"
#include "AboutBoxDialog.h"
#include "GameController.h"
#include "ForceEffect.h"

// ForceWidgets
#include "FWRumble.h"
#include "FWPeriodic.h"
#include "FWConstant.h"
#include "FWSpring.h"
#include "FWFriction.h"
#include "FWDamper.h"
#include "FWInertia.h"
#include "FWRamp.h"

// ForceEffects
#include "FERumble.h"
#include "FEPeriodic.h"
#include "FEConstant.h"
#include "FESpring.h"
#include "FEFriction.h"
#include "FEDamper.h"
#include "FEInertia.h"
#include "FERamp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_gameController(NULL)
{
    ui->setupUi(this);

    setWindowTitle(tr("QForceStudio V%1").arg(_VERSION_STR_));

    // can't edit the status bar in QtDesigner/QtCreator, hence we have to do it here:
    ui->statusBar->addPermanentWidget(ui->currentDeviceLabel);

    connect(ui->actionSelectGameController, SIGNAL(triggered()), this, SLOT(onSelectGameController()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui->actionCreateRumble, SIGNAL(triggered()), this, SLOT(onCreateRumbleForce()));
    connect(ui->actionCreatePeriodic, SIGNAL(triggered()), this, SLOT(onCreatePeriodicForce()));
    connect(ui->actionCreateConstant, SIGNAL(triggered()), this, SLOT(onCreateConstantForce()));
    connect(ui->actionCreateSpring, SIGNAL(triggered()), this, SLOT(onCreateSpringForce()));
    connect(ui->actionCreateFriction, SIGNAL(triggered()), this, SLOT(onCreateFrictionForce()));
    connect(ui->actionCreateDamper, SIGNAL(triggered()), this, SLOT(onCreateDamperForce()));
    connect(ui->actionCreateInertia, SIGNAL(triggered()), this, SLOT(onCreateInertiaForce()));
    connect(ui->actionCreateRamp, SIGNAL(triggered()), this, SLOT(onCreateRampForce()));

    QSettings settings;
    restoreGeometry(settings.value("mainWindow.geometry").toByteArray());
    restoreState(settings.value("mainWindow.state").toByteArray());

    GameControllerPtrList list = GameController::allControllers();
    if (!list.empty())
    {
        setGameController(list.first());
    }
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("mainWindow.geometry", saveGeometry());
    settings.setValue("mainWindow.state", saveState());
    settings.sync();

    delete ui;
}

void MainWindow::setGameController(GameControllerPtr controller)
{
    if (m_gameController)
    {
        // TODO: remove any connections / effects
        m_gameController->close();
        m_gameController.clear();
    }

    m_gameController = controller;

    if (m_gameController)
    {
        // change all forces over to this device
        foreach (ForceEffect *effect, m_forces)
        {
            effect->setGameController(m_gameController.data());
        }

        ui->currentDeviceLabel->setText(tr("Device: %1").arg(m_gameController->name()));
    }
}

void MainWindow::onSelectGameController(void)
{
    qDebug() << "Selecting game controller";

    SelectGameControllerDialog *dialog = new SelectGameControllerDialog(this);
    dialog->setGameController(m_gameController);
    if (QDialog::Accepted == dialog->exec())
    {
        setGameController(dialog->gameController());
    }
}

void MainWindow::onAbout(void)
{
    AboutBoxDialog *dialog = new AboutBoxDialog(this);
    dialog->exec();
}

void MainWindow::onCreateRumbleForce(void)
{
    addForceMdiChild(new FWRumble(this), new FERumble());
}

void MainWindow::onCreatePeriodicForce(void)
{
    addForceMdiChild(new FWPeriodic(this), new FEPeriodic());
}

void MainWindow::onCreateConstantForce(void)
{
    addForceMdiChild(new FWConstant(this), new FEConstant());
}

void MainWindow::onCreateSpringForce(void)
{
    addForceMdiChild(new FWSpring(this), new FESpring());
}

void MainWindow::onCreateFrictionForce(void)
{
    addForceMdiChild(new FWFriction(this), new FEFriction());
}

void MainWindow::onCreateDamperForce(void)
{
    addForceMdiChild(new FWDamper(this), new FEDamper());
}

void MainWindow::onCreateInertiaForce(void)
{
    addForceMdiChild(new FWInertia(this), new FEInertia());
}

void MainWindow::onCreateRampForce(void)
{
    addForceMdiChild(new FWRamp(this), new FERamp());
}

void MainWindow::onForceMdiChildDestroyed(void)
{
    ForceEffectList forcesInUse;
    foreach (QMdiSubWindow *mdi, ui->mdiArea->subWindowList())
    {
        ForceWidget *forceWidget = qobject_cast<ForceWidget *>(mdi->widget());
        if (forceWidget)
        {
            ForceEffect *force = forceWidget->force();
            if (force && !forcesInUse.contains(force))
            {
                forcesInUse.append(forceWidget->force());
            }
        }
    }

    foreach (ForceEffect *force, m_forces)
    {
        if (!forcesInUse.contains(force))
        {
            force->deleteLater();
        }
    }

    m_forces = forcesInUse;
}

void MainWindow::addForceMdiChild(ForceWidget *mdiChild, ForceEffect *force)
{
    Q_ASSERT(mdiChild);
    if (force)
    {
        if (!m_forces.contains(force))
        {
            m_forces.append(force);
            force->setParent(this);
        }
        force->setGameController(m_gameController.data());
        mdiChild->setForce(force);
    }
    ui->mdiArea->addSubWindow(mdiChild);
    mdiChild->setAttribute(Qt::WA_DeleteOnClose);
    mdiChild->show();

    connect(mdiChild, SIGNAL(destroyed()), this, SLOT(onForceMdiChildDestroyed()));
}
