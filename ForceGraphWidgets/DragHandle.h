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

#ifndef DRAGHANDLE_H
#define DRAGHANDLE_H

#include <QWidget>

class DragHandle : public QObject
{
    Q_OBJECT
public:
    explicit DragHandle(QObject *parent = 0);

    int id(void) const;
    void setId(int newId);

    void setDragCursorShape(Qt::CursorShape cursorShape);
    Qt::CursorShape dragCursorShape(void) const;

    QRectF hitArea(void) const;
    void setHitArea(const QRectF newHitArea);
    bool hitTest(const QPointF mouseModelPos) const;

    // model space
    void startDrag(const QPointF startPos);
    void drag(const QPointF dragPos);

signals:
    void posDragged(int id, QPointF newPos);

protected:
    int m_id;
    Qt::CursorShape m_dragCursorShape;
    QRectF m_hitArea;
    QPointF m_offset;
    QPointF m_lastPos;
};

//--

class DragHandler : public QObject
{
    Q_OBJECT
public:
    explicit DragHandler(QWidget *parentWidget);

    void addHandle(int newId, Qt::CursorShape cursorShape);

    void setView2ModelTransform(const QTransform &view2Model);
    void setHitAreaView(int id, QPointF viewPos, double halfWidth = 10, double halfHeight = 0);
    void setHitArea(int id, QRectF modelRect);

    void handleMousePressEvent(QMouseEvent *e);
    void handleMouseMoveEvent(QMouseEvent *e);
    void handleMouseReleaseEvent(QMouseEvent *e);

    bool isDragging(void) const;

signals:
    void posDragged(int id, QPointF newPos);

protected:
    DragHandle *findHandleWithId(int idToFind);

protected:
    QWidget *m_widget;
    DragHandle *m_currentDragHandle;
    QList<DragHandle *> m_dragHandles;
    QTransform m_view2Model;
};

#endif // DRAGHANDLE_H
