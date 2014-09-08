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

#include "QForceStudio.h"
#include "AngleWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <math.h>


AngleWidget::AngleWidget(QWidget *parent) :
    QWidget(parent),
    m_dragging(false),
    m_angle(0.0)
{
    setMinimumSize(50, 50);

    setMouseTracking(true);
}

void AngleWidget::setAngle(double newAngle)
{
    if (m_angle != newAngle)
    {
        m_angle = newAngle;
        update();
    }
}

void AngleWidget::mousePressEvent(QMouseEvent *e)
{
    m_dragging = true;
    setCursor(Qt::ClosedHandCursor);
    updateAngle(e->pos());
    e->accept();
}

void AngleWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragging)
    {
        updateAngle(e->pos());
        e->accept();
    }
}

void AngleWidget::mouseReleaseEvent(QMouseEvent *e)
{
    updateAngle(e->pos());
    setCursor(Qt::ArrowCursor);
    m_dragging = false;
    e->accept();
}

void AngleWidget::updateAngle(QPoint mousePos)
{
    mousePos -= QPoint(frameSize().width() / 2, frameSize().height() / 2);
    double newAngle = normalizeAngle(atan2(mousePos.y(), mousePos.x()) + M_PI_2);
    emit angleChanged(newAngle);
}

void AngleWidget::paintEvent(QPaintEvent *)
{
    static const qreal outerRadius = 49;
    static const qreal centerRadius = 10;
    static const QPoint pointer[] = {
        QPoint(10, 0),
        QPoint(-10, 0),
        QPoint(0, -outerRadius)
    };

    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 100.0, side / 100.0);

    // outer circle
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::gray));
    painter.drawEllipse(QPointF(), outerRadius, outerRadius);

    // needle
    painter.setPen(Qt::red);
    painter.setBrush(QBrush(Qt::red));
    painter.save();
    painter.rotate(m_angle * 360.0 / M_TAU);
    painter.drawConvexPolygon(pointer, array_size(pointer));
    painter.restore();

    // inner circle
    painter.setPen(Qt::gray);
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(QPointF(), centerRadius, centerRadius);
}
