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

#include "FGWConstant.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

enum DragHandlerIds
{
    AttackLength,
    FadeLength,
    ForceLevel,
    AttackLevel,
    FadeLevel
};

FGWConstant::FGWConstant(QWidget *parent) :
    QWidget(parent),
    m_force(NULL),
    m_handleRadius(5.0),
    m_displayedDuration(5.0),
    m_dragHandler(new DragHandler(this))
{
    setMinimumSize(2 * m_handleRadius + 1, 2 * m_handleRadius + 1);

    connect(m_dragHandler, SIGNAL(posDragged(int,QPointF)), this, SLOT(onPosDragged(int,QPointF)));

    m_dragHandler->addHandle(AttackLength, Qt::CrossCursor);
    m_dragHandler->addHandle(FadeLength, Qt::CrossCursor);
    m_dragHandler->addHandle(ForceLevel, Qt::SizeVerCursor);
    m_dragHandler->addHandle(AttackLevel, Qt::SizeVerCursor);
    m_dragHandler->addHandle(FadeLevel, Qt::SizeVerCursor);
}

void FGWConstant::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = force;

    if (m_force)
    {
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(update()));
        connect(m_force, SIGNAL(durationChanged(double)), this, SLOT(updateTransforms()));

        updateTransforms();
    }
}

void FGWConstant::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_force)
    {
        QPointF startPos(0.0, 0.0);
        QPointF endPos(m_displayedDuration, 0.0);
        QPointF attackLevelPos(0, m_force->attackLevel());
        QPointF attackLengthPos(m_force->attackLength(), m_force->level());
        QPointF fadeLengthPos(m_force->duration() - m_force->fadeLength(), m_force->level());
        QPointF fadeLevelPos(m_force->duration(), m_force->fadeLevel());
        QPointF infinitePos(m_displayedDuration, m_force->level());

        QPointF startViewPos = m_model2View.map(startPos);
        QPointF endViewPos = m_model2View.map(endPos);
        QPointF attackLevelViewPos = m_model2View.map(attackLevelPos);
        QPointF attackLengthViewPos = m_model2View.map(attackLengthPos);
        QPointF fadeLengthViewPos = m_model2View.map(fadeLengthPos);
        QPointF fadeLevelViewPos = m_model2View.map(fadeLevelPos);
        QPointF infiniteViewPos = m_model2View.map(infinitePos);

        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        QPolygon envelope;
        if (m_force->isInfiniteDuration())
        {
            envelope << startViewPos.toPoint()
                     << attackLevelViewPos.toPoint()
                     << attackLengthViewPos.toPoint()
                     << infiniteViewPos.toPoint()
                     << endViewPos.toPoint();
        }
        else
        {
            envelope << startViewPos.toPoint()
                     << attackLevelViewPos.toPoint()
                     << attackLengthViewPos.toPoint()
                     << fadeLengthViewPos.toPoint()
                     << fadeLevelViewPos.toPoint()
                     << endViewPos.toPoint();
        }
        painter.drawPolygon(envelope);

        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(attackLevelViewPos, m_handleRadius, m_handleRadius);
        painter.drawEllipse(attackLengthViewPos, m_handleRadius, m_handleRadius);
        if (!m_force->isInfiniteDuration())
        {
            painter.drawEllipse(fadeLengthViewPos, m_handleRadius, m_handleRadius);
            painter.drawEllipse(fadeLevelViewPos, m_handleRadius, m_handleRadius);
        }

        if (!m_dragHandler->isDragging())
        {
            m_dragHandler->setHitAreaView(AttackLevel, attackLevelViewPos, m_handleRadius);
            m_dragHandler->setHitAreaView(AttackLength, attackLengthViewPos, m_handleRadius);
            if (m_force->isInfiniteDuration())
            {
                QPointF centerViewPos = (attackLengthViewPos + infiniteViewPos) / 2;
                m_dragHandler->setHitAreaView(ForceLevel, centerViewPos,
                                              m_handleRadius + (infiniteViewPos.x() - attackLengthViewPos.x()) / 2,
                                              m_handleRadius);
            }
            else
            {
                QPointF centerViewPos = (attackLengthViewPos + fadeLengthViewPos) / 2;
                m_dragHandler->setHitAreaView(ForceLevel, centerViewPos,
                                              m_handleRadius + (fadeLengthViewPos.x() - attackLengthViewPos.x()) / 2,
                                              m_handleRadius);
                m_dragHandler->setHitAreaView(FadeLength, fadeLengthViewPos, m_handleRadius);
                m_dragHandler->setHitAreaView(FadeLevel, fadeLevelViewPos, m_handleRadius);
            }
        }
    }
}

void FGWConstant::mousePressEvent(QMouseEvent *e)
{
    m_dragHandler->handleMousePressEvent(e);
}

void FGWConstant::mouseMoveEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseMoveEvent(e);
}

void FGWConstant::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseReleaseEvent(e);
}

void FGWConstant::resizeEvent(QResizeEvent *)
{
    updateTransforms();
}

void FGWConstant::onPosDragged(int handleId, QPointF newPos)
{
    if (m_force)
    {
        double level = qMax(0.0, qMin(1.0, newPos.y()));
        double attackLength = qMax(0.0, qMin(m_displayedDuration, newPos.x()));
        double fadeLength = qMax(0.0, qMin(m_displayedDuration, m_displayedDuration - newPos.x()));

        switch (handleId)
        {
            case AttackLevel:
                m_force->setAttackLevel(level);
                break;

            case AttackLength:
                m_force->setAttackLength(attackLength);
                m_force->setLevel(level);
                break;

            case ForceLevel:
                m_force->setLevel(level);
                break;

            case FadeLength:
                m_force->setFadeLength(fadeLength);
                m_force->setLevel(level);
                break;

            case FadeLevel:
                m_force->setFadeLevel(level);
                break;
        }
    }
}

void FGWConstant::updateTransforms(void)
{
    m_model2View.reset();
    m_model2View.translate(m_handleRadius, height() - m_handleRadius);
    m_displayedDuration = 5.0;
    if (m_force && !m_force->isInfiniteDuration())
    {
        m_displayedDuration = qMax(0.01, m_force->duration());
    }
    m_model2View.scale((width() - 2.0 * m_handleRadius) / m_displayedDuration, -(height() - 2.0 * m_handleRadius));
    Q_ASSERT(m_model2View.isInvertible());
    m_view2Model = m_model2View.inverted();
    m_dragHandler->setView2ModelTransform(m_view2Model);
    update();
}
