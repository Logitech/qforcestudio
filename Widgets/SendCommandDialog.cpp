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

#include "SendCommandDialog.h"
#include "ui_SendCommandDialog.h"
#include <math.h>
#include <QRegExp>
#include <QDebug>


SendCommandDialog::SendCommandDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SendCommandDialog)
{
    ui->setupUi(this);

    connect(ui->sendCommand, SIGNAL(pressed()), this, SLOT(onSendCommand()));
    connect(ui->command, SIGNAL(textChanged(
                                    const QString&)), this, SLOT(commandChanged(
                                                                     const QString&)));
    ui->sendCommand->setEnabled(false);
}


SendCommandDialog::~SendCommandDialog(void)
{
    delete ui;
}


void SendCommandDialog::setGameController(GameControllerPtr controller)
{
    m_gameController = controller;
}


GameControllerPtr SendCommandDialog::gameController(void) const
{
    return m_gameController;
}


void SendCommandDialog::onSendCommand(void)
{
    if (m_gameController)
    {
        QByteArray command = commandAsBytes(ui->command->text());
        QString dbg = "Sending command: ";
        for (int i = 0; i < command.size(); i++)
        {
            dbg += QString(" 0x%1").arg((unsigned char) command[i], 0, 16);
        }
        qDebug() << dbg;

        QByteArray response;
        m_gameController->sendCommand(command, response);

        QString responseString;
        for (int i = 0; i < response.size(); i++)
        {
            responseString += QString("%1 ").arg((unsigned char) response[i], 0, 16);
        }
        ui->response->setText(responseString);
    }
}


void SendCommandDialog::commandChanged(const QString &command)
{
    ui->sendCommand->setEnabled(!commandAsBytes(command).isEmpty());
}


QByteArray SendCommandDialog::commandAsBytes(const QString &command)
{
    QByteArray ret;
    QStringList bytes = command.split(" ");
    if (bytes.isEmpty())
    {
        return ret;
    }

    for (const QString &byte : bytes)
    {
        if (byte.isEmpty())
        {
            continue;
        }

        QRegularExpression hexMatcher("^[0-9A-F]{1,2}$", QRegularExpression::CaseInsensitiveOption);

        if (!hexMatcher.match(byte).hasMatch())
        {
            return QByteArray();
        }

        bool ok = false;
        ret.append((unsigned char) byte.toUInt(&ok, 16));
    }

    return ret;
}
