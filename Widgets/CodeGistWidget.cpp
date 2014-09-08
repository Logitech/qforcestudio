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

#include "CodeGistWidget.h"
#include "ui_CodeGistWidget.h"
#include <QDebug>
#include <QClipboard>

CodeGistWidget::CodeGistWidget(QWidget *parent) :
    ForceWidget(parent),
    ui(new Ui::CodeGistWidget),
    m_force(NULL)
{
    ui->setupUi(this);

    connect(ui->copyToClipboardButton, SIGNAL(clicked()), this, SLOT(onCopyToClipboard()));
}

CodeGistWidget::~CodeGistWidget(void)
{
    delete ui;
}

ForceEffect *CodeGistWidget::force(void) const
{
    return m_force;
}

void CodeGistWidget::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(updateGist()));

        updateGist();
    }
}

void CodeGistWidget::onCopyToClipboard(void)
{
    QClipboard *clipboard = qApp->clipboard();
    if (clipboard && !m_latestGist.isEmpty())
    {
        clipboard->setText(m_latestGist);
    }
}

void CodeGistWidget::updateGist(void)
{
    if (m_force)
    {
        QString newGist = m_force->gist();
        if (newGist != m_latestGist)
        {
            m_latestGist = newGist;
            ui->gistTextEdit->setPlainText(m_latestGist);
        }
    }
}
