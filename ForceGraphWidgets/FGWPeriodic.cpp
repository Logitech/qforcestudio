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

#include "FGWPeriodic.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

enum DragHandlerIds
{
    AttackLength,
    FadeLength,
    ForceLevel,
    OffsetLevel,
    AttackLevel,
    FadeLevel
};

FGWPeriodic::FGWPeriodic(QWidget *parent) :
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
    m_dragHandler->addHandle(OffsetLevel, Qt::SizeVerCursor);
}

void FGWPeriodic::setForce(ForceEffect *force)
{
    if (m_force)
    {
        disconnect(m_force, 0, this, 0);
    }

    m_force = qobject_cast<FEPeriodic *>(force);

    if (m_force)
    {
        connect(m_force, SIGNAL(levelChanged(double)), this, SLOT(updateLevel(double)));
        connect(m_force, SIGNAL(durationChanged(double)), this, SLOT(updateDuration(double)));
        connect(m_force, SIGNAL(offsetChanged(double)), this, SLOT(updateOffset(double)));
        connect(m_force, SIGNAL(valuesChanged()), this, SLOT(update()));
        // TODO: phase, waveform

        update();
        updateLevel(m_force->level());
        updateOffset(m_force->offset());
        // TODO: phase, waveform
    }
}

void FGWPeriodic::paintEvent(QPaintEvent *)
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

        // the waveform is drawn for each pixel along the X (time) axis
        int waveFormViewPixelCount = endViewPos.x() - startViewPos.x();
        for (int waveFormViewX = 0; waveFormViewX < waveFormViewPixelCount; ++waveFormViewX)
        {
            QPointF waveFormViewBase(startViewPos.x() + waveFormViewX, startViewPos.y());
            QPointF waveFormBase = m_view2Model.map(waveFormViewBase);
            QPointF waveFormPeak(waveFormBase.x(), m_force->value(waveFormBase.x()));
            QPointF waveFormViewPeak = m_model2View.map(waveFormPeak);
            if (m_force->withinAttackLength(waveFormBase.x()))
            {
                painter.setPen(Qt::red);
            }
            else if (m_force->withinFadeLength(waveFormBase.x(), m_displayedDuration))
            {
                painter.setPen(Qt::blue);
            }
            else
            {
                painter.setPen(Qt::green);
            }
            painter.drawLine(waveFormViewBase, waveFormViewPeak);
        }

        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
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

        QPointF offsetPos0(0.0, m_force->offset());
        QPointF offsetPos1(m_displayedDuration, m_force->offset());
        QPointF offsetViewPos0 = m_model2View.map(offsetPos0);
        QPointF offsetViewPos1 = m_model2View.map(offsetPos1);
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(offsetViewPos0, offsetViewPos1);

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
            QPointF centerOffsetViewPos = (offsetViewPos0 + offsetViewPos1) / 2;
            m_dragHandler->setHitAreaView(OffsetLevel, centerOffsetViewPos,
                                          width() / 2, m_handleRadius);
        }
    }
}

void FGWPeriodic::mousePressEvent(QMouseEvent *e)
{
    m_dragHandler->handleMousePressEvent(e);
}

void FGWPeriodic::mouseMoveEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseMoveEvent(e);
}

void FGWPeriodic::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseReleaseEvent(e);
}

void FGWPeriodic::resizeEvent(QResizeEvent *)
{
    updateTransforms();
}

void FGWPeriodic::updateTransforms(void)
{
    m_model2View.reset();
    m_model2View.translate(m_handleRadius, height() - m_handleRadius);
    m_displayedDuration = 5.0;
    if (m_force && !m_force->isInfiniteDuration())
    {
        m_displayedDuration = qMax(0.01, m_force->duration());
    }
    m_model2View.scale((width() - 2.0 * m_handleRadius) / m_displayedDuration,
                       -(height() - 2.0 * m_handleRadius) / 2.0);
    m_model2View.translate(0.0, 1.0);
    Q_ASSERT(m_model2View.isInvertible());
    m_view2Model = m_model2View.inverted();
    m_dragHandler->setView2ModelTransform(m_view2Model);
    update();
}

void FGWPeriodic::onPosDragged(int handleId, QPointF newPos)
{
    if (m_force)
    {
        double level = qMax(0.0, qMin(1.0, newPos.y()));
        double offset = qMax(-1.0, qMin(1.0, newPos.y()));
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
            if ((m_displayedDuration - m_force->fadeLength()) < attackLength)
            {
                fadeLength = m_displayedDuration - attackLength;
                m_force->setFadeLength(fadeLength);
            }
            break;

        case ForceLevel:
            m_force->setLevel(level);
            break;

        case OffsetLevel:
            m_force->setOffset(offset);
            break;

        case FadeLength:
            m_force->setFadeLength(fadeLength);
            m_force->setLevel(level);
            if ((m_displayedDuration - fadeLength) < m_force->attackLength())
            {
                attackLength = m_displayedDuration - fadeLength;
                m_force->setAttackLength(attackLength);
            }
            break;

        case FadeLevel:
            m_force->setFadeLevel(level);
            break;

        default:
            break;
        }
    }
}

void FGWPeriodic::updateLevel(double)
{
    update();
}

void FGWPeriodic::updateWaveform(int)
{
    update();
}

void FGWPeriodic::updatePeriod(double)
{
    update();
}

void FGWPeriodic::updatePhase(double)
{
    update();
}

void FGWPeriodic::updateOffset(double)
{
    update();
}

void FGWPeriodic::updateDuration(double)
{
    updateTransforms();
}
