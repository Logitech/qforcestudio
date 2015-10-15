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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameController.h"
#include "ForceEffect.h"
#include "ForceWidget.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGameController(GameControllerPtr controller);

protected slots:
    void onSelectGameController(void);
    void onAbout(void);
    void onCreateRumbleForce(void);
    void onCreatePeriodicForce(void);
    void onCreateConstantForce(void);
    void onCreateSpringForce(void);
    void onCreateFrictionForce(void);
    void onCreateDamperForce(void);
    void onCreateInertiaForce(void);
    void onCreateRampForce(void);

    void onResetAllForces(void);
    void onPauseAllForces(void);
    void onContinueAllForces(void);
    void onStopAllForces(void);
    void onSetActuatorsOn(void);
    void onSetActuatorsOff(void);
    void onSendCommand(void);

    void onForceMdiChildDestroyed(void);

public:
    void addForceMdiChild(ForceWidget *mdiChild, ForceEffect *force = NULL);

private:
    Ui::MainWindow *ui;
    GameControllerPtr m_gameController;
    ForceEffectList m_forces;
};

#endif // MAINWINDOW_H
