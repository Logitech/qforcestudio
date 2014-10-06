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

#ifndef FGWCONDITION_H
#define FGWCONDITION_H

#include <QWidget>
#include "ForceEffect.h"
#include "DragHandle.h"

class FGWCondition : public QWidget
{
    Q_OBJECT
public:
    explicit FGWCondition(QWidget *parent = 0);

    void setForceCondition(ForceCondition *forceCondition, const QString &xAxisLabel);

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
    ForceCondition *m_forceCondition;
    QString m_xAxisLabel;
    QString m_yAxisLabel;
    int m_fontHeight;
    qreal m_handleRadius;
    QTransform m_model2View;
    QTransform m_view2Model;
    DragHandler *m_dragHandler;
};

#endif // FGWCONDITION_H
