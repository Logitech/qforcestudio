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

#include "FGWCondition.h"
#include "QForceStudio.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <math.h>

enum DragHandlerIds
{
    NegativeSaturation,
    NegativeCoefficient,
    NegativeDeadBand,
    Offset,
    PositiveDeadBand,
    PositiveCoefficient,
    PositiveSaturation
};

FGWCondition::FGWCondition(QWidget *parent) :
    QWidget(parent),
    m_forceCondition(NULL),
    m_handleRadius(5.0),
    m_dragHandler(new DragHandler(this))
{
    setMinimumSize(2 * m_handleRadius + 100, 2 * m_handleRadius + 100);

    connect(m_dragHandler, SIGNAL(posDragged(int,QPointF)), this, SLOT(onPosDragged(int,QPointF)));

    m_dragHandler->addHandle(NegativeCoefficient, Qt::CrossCursor);
    m_dragHandler->addHandle(PositiveCoefficient, Qt::CrossCursor);
    m_dragHandler->addHandle(NegativeDeadBand, Qt::CrossCursor);
    m_dragHandler->addHandle(PositiveDeadBand, Qt::CrossCursor);
    m_dragHandler->addHandle(NegativeSaturation, Qt::SizeVerCursor);
    m_dragHandler->addHandle(PositiveSaturation, Qt::SizeVerCursor);
    m_dragHandler->addHandle(Offset, Qt::SizeHorCursor);
}

void FGWCondition::setForceCondition(ForceCondition *forceCondition)
{
    if (m_forceCondition)
    {
        disconnect(m_forceCondition, 0, this, 0);
    }

    m_forceCondition = forceCondition;

    if (m_forceCondition)
    {
        connect(m_forceCondition, SIGNAL(changed()), this, SLOT(update()));

        update();
    }
}

void FGWCondition::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_forceCondition)
    {
        QPointF startPos(-1.0, 0.0);
        QPointF startViewPos = m_model2View.map(startPos);

        QPointF endPos(1.0, 0.0);
        QPointF endViewPos = m_model2View.map(endPos);

        double negativeSaturation = m_forceCondition->negativeSaturation();
        if (m_forceCondition->negativeCoefficient() < 0.0)
        {
            negativeSaturation = -negativeSaturation;
        }

        double positiveSaturation = m_forceCondition->positiveSaturation();
        if (m_forceCondition->positiveCoefficient() < 0.0)
        {
            positiveSaturation = -positiveSaturation;
        }

        double negativeDeadBandValue = CLIP11(m_forceCondition->offset() - m_forceCondition->deadBand());
        double positiveDeadBandValue = CLIP11(m_forceCondition->offset() + m_forceCondition->deadBand());

        double negCoeff = m_forceCondition->negativeCoefficient();
        double negCoeffValue = 1000.0;
        if (0.001 < fabs(negCoeff))
        {
            negCoeffValue = (1.0 / fabs(negCoeff)) - 1.0;
        }
        negCoeffValue *= m_forceCondition->negativeSaturation();
        double negativeSaturationValue = negativeDeadBandValue - negCoeffValue;
        if (negativeSaturationValue < -1.0)
        {
            double negCoeffVisible = negativeDeadBandValue + 1.0;
            double ratio = negCoeffVisible / negCoeffValue;
            negativeSaturation *= ratio;
            negativeSaturationValue = -1.0;
        }

        double posCoeff = m_forceCondition->positiveCoefficient();
        double posCoeffValue = 1000.0;
        if (0.001 < fabs(posCoeff))
        {
            posCoeffValue = (1.0 / fabs(posCoeff)) - 1.0;
        }
        posCoeffValue *= m_forceCondition->positiveSaturation();
        double positiveSaturationValue = positiveDeadBandValue + posCoeffValue;
        if (1.0 < positiveSaturationValue)
        {
            double posCoeffVisible = 1.0 - positiveDeadBandValue;
            double ratio = posCoeffVisible / posCoeffValue;
            positiveSaturation *= ratio;
            positiveSaturationValue = 1.0;
        }

        QPointF offsetPos(m_forceCondition->offset(), 0.0);
        QPointF offsetViewPos = m_model2View.map(offsetPos);

        QPointF negativeSaturationPos(-1.0, negativeSaturation);
        QPointF negativeSaturationViewPos = m_model2View.map(negativeSaturationPos);

        QPointF negativeCoefficientPos(negativeSaturationValue, negativeSaturation);
        QPointF negativeCoefficientViewPos = m_model2View.map(negativeCoefficientPos);

        QPointF negativeDeadBandPos(negativeDeadBandValue, 0.0);
        QPointF negativeDeadBandViewPos = m_model2View.map(negativeDeadBandPos);

        QPointF positiveDeadBandPos(positiveDeadBandValue, 0.0);
        QPointF positiveDeadBandViewPos = m_model2View.map(positiveDeadBandPos);

        QPointF positiveCoefficientPos(positiveSaturationValue, positiveSaturation);
        QPointF positiveCoefficientViewPos = m_model2View.map(positiveCoefficientPos);

        QPointF positiveSaturationPos(1.0, positiveSaturation);
        QPointF positiveSaturationViewPos = m_model2View.map(positiveSaturationPos);

        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        QPolygon envelope;
        envelope << startViewPos.toPoint()
                 << negativeSaturationViewPos.toPoint()
                 << negativeCoefficientViewPos.toPoint()
                 << negativeDeadBandViewPos.toPoint()
                 << positiveDeadBandViewPos.toPoint()
                 << positiveCoefficientViewPos.toPoint()
                 << positiveSaturationViewPos.toPoint()
                 << endViewPos.toPoint();
        painter.drawPolygon(envelope);

        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(negativeCoefficientViewPos, m_handleRadius, m_handleRadius);
        painter.drawEllipse(negativeDeadBandViewPos, m_handleRadius, m_handleRadius);
        painter.drawEllipse(positiveDeadBandViewPos, m_handleRadius, m_handleRadius);
        painter.drawEllipse(positiveCoefficientViewPos, m_handleRadius, m_handleRadius);

        if (!m_dragHandler->isDragging())
        {
            double negSatHalfWidth = (negativeCoefficientViewPos.x() - startViewPos.x()) / 2.0;
            m_dragHandler->setHitAreaView(NegativeSaturation,
                                          QPointF(startViewPos.x() + negSatHalfWidth, negativeSaturationViewPos.y()),
                                          negSatHalfWidth, m_handleRadius);
            m_dragHandler->setHitAreaView(NegativeCoefficient, negativeCoefficientViewPos);
            m_dragHandler->setHitAreaView(NegativeDeadBand, negativeDeadBandViewPos);
            double offsetHalfWidth = (positiveDeadBandViewPos.x() - negativeDeadBandViewPos.x()) / 2.0;
            m_dragHandler->setHitAreaView(Offset, offsetViewPos, offsetHalfWidth, m_handleRadius);
            m_dragHandler->setHitAreaView(PositiveDeadBand, positiveDeadBandViewPos);
            m_dragHandler->setHitAreaView(PositiveCoefficient, positiveCoefficientViewPos);
            double posSatHalfWidth = (endViewPos.x() - positiveCoefficientViewPos.x()) / 2.0;
            m_dragHandler->setHitAreaView(PositiveSaturation,
                                          QPointF(endViewPos.x() - posSatHalfWidth, positiveSaturationViewPos.y()),
                                          posSatHalfWidth, m_handleRadius);
        }
    }
}

void FGWCondition::mousePressEvent(QMouseEvent *e)
{
    m_dragHandler->handleMousePressEvent(e);
}

void FGWCondition::mouseMoveEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseMoveEvent(e);
}

void FGWCondition::mouseReleaseEvent(QMouseEvent *e)
{
    m_dragHandler->handleMouseReleaseEvent(e);
}

void FGWCondition::resizeEvent(QResizeEvent *)
{
    updateTransforms();
}

void FGWCondition::updateTransforms(void)
{
    m_model2View.reset();
    m_model2View.translate(m_handleRadius, height() - m_handleRadius);
    m_model2View.scale((width() - 2.0 * m_handleRadius) / 2.0, -(height() - 2.0 * m_handleRadius) / 2.0);
    m_model2View.translate(1.0, 1.0);
    Q_ASSERT(m_model2View.isInvertible());
    m_view2Model = m_model2View.inverted();
    m_dragHandler->setView2ModelTransform(m_view2Model);
    update();
}

void FGWCondition::onPosDragged(int handleId, QPointF newPos)
{
    if (m_forceCondition)
    {
        double fullLevel = qMax(-1.0, qMin(1.0, newPos.y()));
        double fullValue = qMax(-1.0, qMin(1.0, newPos.x()));

        switch (handleId)
        {
            case NegativeSaturation:
                m_forceCondition->setNegativeSaturation(fabs(fullLevel));
                if (fullLevel < 0.0)
                {
                    m_forceCondition->setNegativeCoefficient(-fabs(m_forceCondition->negativeCoefficient()));
                }
                else
                {
                    m_forceCondition->setNegativeCoefficient(+fabs(m_forceCondition->negativeCoefficient()));
                }
                break;

            case NegativeCoefficient:
                m_forceCondition->setNegativeCoefficient(fullLevel);
                break;

            case NegativeDeadBand:
                m_forceCondition->setDeadBand(m_forceCondition->offset() - fullValue);
                break;

            case Offset:
                m_forceCondition->setOffset(fullValue);
                break;

            case PositiveDeadBand:
                m_forceCondition->setDeadBand(fullValue - m_forceCondition->offset());
                break;

            case PositiveCoefficient:
                m_forceCondition->setPositiveCoefficient(fullLevel);
                break;

            case PositiveSaturation:
                m_forceCondition->setPositiveSaturation(fabs(fullLevel));
                if (fullLevel < 0.0)
                {
                    m_forceCondition->setPositiveCoefficient(-fabs(m_forceCondition->positiveCoefficient()));
                }
                else
                {
                    m_forceCondition->setPositiveCoefficient(+fabs(m_forceCondition->positiveCoefficient()));
                }
                break;
        }
    }
}
