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

#include "FGWRamp.h"
#include "QForceStudio.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

enum DragHandlerIds
{
    StartLevel,
    EndLevel
};

FGWRamp::FGWRamp(QWidget *parent) :
    QWidget(parent),
    m_force(NULL),
    m_handleRadius(5.0),
    m_displayedDuration(5.0),
    m_dragHandler(new DragHandler(this))
{
    setMinimumSize(2 * m_handleRadius + 1, 2 * m_handleRadius + 1);

    connect(m_dragHandler, SIGNAL(posDragged(int,QPointF)), this, SLOT(onPosDragged(int,QPointF)));

    m_dragHandler->addHandle(StartLevel, Qt::SizeVerCursor);
    m_dragHandler->addHandle(EndLevel, Qt::SizeVerCursor);
}

void FGWRamp::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = qobject_cast<FERamp *>(force);

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(update()));
        connect(m_force, SIGNAL(durationChanged(double)), this, SLOT(updateTransforms()));

        updateTransforms();
    }
}

void FGWRamp::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_force)
    {
        QPointF startPos(0.0, 0.0);
        QPointF startViewPos = m_model2View.map(startPos);

        QPointF endPos(m_displayedDuration, 0.0);
        QPointF endViewPos = m_model2View.map(endPos);

        QPointF startLevelPos(0, m_force->startLevel());
        QPointF startLevelViewPos = m_model2View.map(startLevelPos);

        QPointF endLevelPos(m_force->duration(), m_force->endLevel());
        QPointF endLevelViewPos = m_model2View.map(endLevelPos);

        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        QPolygon envelope;
        envelope << startViewPos.toPoint()
                 << startLevelViewPos.toPoint()
                 << endLevelViewPos.toPoint()
                 << endViewPos.toPoint();
        painter.drawPolygon(envelope);

        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(startLevelViewPos, m_handleRadius, m_handleRadius);
        painter.drawEllipse(endLevelViewPos, m_handleRadius, m_handleRadius);

        if (!m_dragHandler->isDragging())
        {
            m_dragHandler->setHitAreaView(StartLevel, startLevelViewPos, m_handleRadius);
            m_dragHandler->setHitAreaView(EndLevel, endLevelViewPos, m_handleRadius);
        }
    }
}

void FGWRamp::mousePressEvent(QMouseEvent *e)
{
    m_dragHandler->handleMousePressEvent(e);
}

void FGWRamp::mouseMoveEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseMoveEvent(e);
}

void FGWRamp::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseReleaseEvent(e);
}

void FGWRamp::resizeEvent(QResizeEvent *)
{
    updateTransforms();
}

void FGWRamp::onPosDragged(int handleId, QPointF newPos)
{
    if (m_force)
    {
        double level = CLIP11(newPos.y());

        switch (handleId)
        {
            case StartLevel:
                m_force->setStartLevel(level);
                break;

            case EndLevel:
                m_force->setEndLevel(level);
                break;
        }
    }
}

void FGWRamp::updateTransforms(void)
{
    m_model2View.reset();
    m_model2View.translate(m_handleRadius, height() - m_handleRadius);
    m_displayedDuration = 5.0;
    if (m_force && !m_force->isInfiniteDuration())
    {
        m_displayedDuration = qMax(0.01, m_force->duration());
    }
    m_model2View.scale((width() - 2.0 * m_handleRadius) / m_displayedDuration, -(height() - 2.0 * m_handleRadius) / 2.0);
    m_model2View.translate(0.0, 1.0);
    Q_ASSERT(m_model2View.isInvertible());
    m_view2Model = m_model2View.inverted();
    m_dragHandler->setView2ModelTransform(m_view2Model);
    update();
}
