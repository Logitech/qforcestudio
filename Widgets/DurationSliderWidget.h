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

#ifndef DURATIONSLIDERWIDGET_H
#define DURATIONSLIDERWIDGET_H

#include <QWidget>
#include "DragHandle.h"
#include "ForceEffect.h"

class DurationSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DurationSliderWidget(QWidget *parent = 0);

    void setForce(ForceEffect *force);

signals:
    void delayChanged(double newDelay);
    void durationChanged(double newDuration);
    void infiniteDurationChanged(bool isNowInfinite);

public slots:
    void updateDelay(double requestedDelay);
    void updateDuration(double requestedDuration);
    void updateInfiniteDuration(bool isNowInfinite);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void resizeEvent(QResizeEvent *);

    void updateTransforms(void);

protected slots:
    void onPosDragged(int handleId, QPointF newPos);

private:
    double m_delay;
    double m_duration;
    bool m_isInfinite;

    double m_displayDuration;
    QTransform m_model2View;
    QTransform m_view2Model;

    DragHandler *m_dragHandler;
};

#endif // DURATIONSLIDERWIDGET_H
