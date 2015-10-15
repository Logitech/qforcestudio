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

#ifndef SENDCOMMANDDIALOG_H
#define SENDCOMMANDDIALOG_H

#include <QDialog>
#include "GameController.h"


namespace Ui {
class SendCommandDialog;
}

class SendCommandDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SendCommandDialog(QWidget *parent = 0);
    ~SendCommandDialog(void);

    void setGameController(GameControllerPtr controller);
    GameControllerPtr gameController(void) const;

protected slots:
    void onSendCommand(void);
    void commandChanged(const QString& command);

private:
    QByteArray commandAsBytes(const QString& command);

private:
    Ui::SendCommandDialog *ui;
    GameControllerPtr m_gameController;
};

#endif // SENDCOMMANDDIALOG_H
