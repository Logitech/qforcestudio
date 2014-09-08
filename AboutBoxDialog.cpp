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

#include "AboutBoxDialog.h"
#include "ui_AboutBoxDialog.h"
#include <QFile>
#include "QForceStudio.h"
#include <QDebug>

AboutBoxDialog::AboutBoxDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBoxDialog)
{
    ui->setupUi(this);

    connect(ui->licenseButton, SIGNAL(clicked()), this, SLOT(onLicense()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    ui->licenseTextEdit->setVisible(false);

    QFile licenseFile(":LICENSE");
    licenseFile.open(QIODevice::ReadOnly);
    QByteArray license = licenseFile.readAll();
    licenseFile.close();

    ui->licenseTextEdit->setPlainText(QString::fromUtf8(license));

    ui->versionLabel->setText(tr("QForceStudio V%1").arg(_VERSION_STR_));
}

AboutBoxDialog::~AboutBoxDialog(void)
{
    delete ui;
}

void AboutBoxDialog::onLicense(void)
{
    qDebug() << "Showing license...";
    ui->licenseTextEdit->setVisible(true);
    ui->licenseButton->setEnabled(false);
}
