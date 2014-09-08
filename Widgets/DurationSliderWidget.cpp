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

#include "DurationSliderWidget.h"
#include <QPainter>

enum DragHandlerIds
{
    DelayLength,
    Delay,
    Duration
};

DurationSliderWidget::DurationSliderWidget(QWidget *parent) :
    QWidget(parent),
    m_delay(0.0),
    m_duration(1.0),
    m_isInfinite(false),
    m_displayDuration(10.0),
    m_dragHandler(new DragHandler(this))
{
    connect(m_dragHandler, SIGNAL(posDragged(int,QPointF)), this, SLOT(onPosDragged(int,QPointF)));

    m_dragHandler->addHandle(DelayLength, Qt::SizeHorCursor);
    m_dragHandler->addHandle(Delay, Qt::OpenHandCursor);
    m_dragHandler->addHandle(Duration, Qt::SizeHorCursor);
}

void DurationSliderWidget::updateDelay(double newDelay)
{
    if (newDelay != m_delay)
    {
        m_delay = newDelay;
        update();
    }
}

void DurationSliderWidget::updateDuration(double requestedDuration)
{
    if (requestedDuration != m_duration)
    {
        m_duration = requestedDuration;
        update();
    }
}

void DurationSliderWidget::updateInfiniteDuration(bool isNowInfinite)
{
    if (isNowInfinite != m_isInfinite)
    {
        m_isInfinite = isNowInfinite;
        update();
    }
}

void DurationSliderWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // scale
    QVector<QLineF> scale;
    scale << m_model2View.map(QLineF(12.0, 0.0, 0.0, 0.0))
          << m_model2View.map(QLineF(0.0, 0.0, 0.0, 1.0))
          << m_model2View.map(QLineF(0.0, 1.0, 12.0, 1.0));
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawLines(scale);

    if (m_isInfinite)
    {
        QPolygonF effect;
        effect << m_model2View.map(QPointF(m_delay, 0.0))
               << m_model2View.map(QPointF(m_delay, 1.0))
               << m_model2View.map(QPointF(12.0, 1.0))
               << m_model2View.map(QPointF(12.0, 0.0));
        painter.setBrush(Qt::blue);
        painter.drawPolygon(effect);

        if (!m_dragHandler->isDragging())
        {
            m_dragHandler->setHitAreaView(DelayLength, m_model2View.map(QPointF(m_delay, 0.5)), 10, height());
            m_dragHandler->setHitArea(Delay, QRectF(m_delay, 0.0, m_duration, 1.0));
            m_dragHandler->setHitArea(Duration, QRectF());
        }
    }
    else
    {
        QPolygonF effect;
        effect << m_model2View.map(QPointF(m_delay, 0.0))
               << m_model2View.map(QPointF(m_delay, 1.0))
               << m_model2View.map(QPointF(m_delay + m_duration, 1.0))
               << m_model2View.map(QPointF(m_delay + m_duration, 0.0));
        painter.setBrush(QBrush(Qt::blue));
        painter.drawPolygon(effect);

        if (!m_dragHandler->isDragging())
        {
            m_dragHandler->setHitAreaView(DelayLength, m_model2View.map(QPointF(m_delay, 0.5)), 10, height());
            m_dragHandler->setHitArea(Delay, QRectF(m_delay, 0.0, m_duration, 1.0));
            m_dragHandler->setHitAreaView(Duration, m_model2View.map(QPointF(m_delay + m_duration, 0.5)), 10, height());
        }
    }
}

void DurationSliderWidget::mousePressEvent(QMouseEvent *e)
{
    m_dragHandler->handleMousePressEvent(e);
}

void DurationSliderWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseMoveEvent(e);
}

void DurationSliderWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseReleaseEvent(e);
}

void DurationSliderWidget::resizeEvent(QResizeEvent *)
{
    updateTransforms();
}

void DurationSliderWidget::updateTransforms(void)
{
    m_model2View.reset();
    m_displayDuration = 13.0;
    m_model2View.scale(width() / m_displayDuration, height());
    Q_ASSERT(m_model2View.isInvertible());
    m_view2Model = m_model2View.inverted();
    m_dragHandler->setView2ModelTransform(m_view2Model);
    update();
}

void DurationSliderWidget::onPosDragged(int handleId, QPointF newPos)
{
    double pos = qMax(0.0, qMin(12.0, newPos.x()));
    double delayDelta = pos - m_delay;
    switch (handleId)
    {
    case DelayLength:
        emit delayChanged(pos);
        if (!m_isInfinite)
        {
            emit durationChanged(m_duration - delayDelta);
        }
        break;

    case Delay:
        emit delayChanged(pos - m_duration / 2.0);
        break;

    case Duration:
        if (10.0 < pos)
        {
            if (!m_isInfinite)
            {
                emit infiniteDurationChanged(true);
            }
        }
        else
        {
            if (m_isInfinite)
            {
                emit infiniteDurationChanged(false);
            }
            emit durationChanged(pos - m_delay);
        }
        break;
    }
}
