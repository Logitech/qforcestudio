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

#include "DragHandle.h"
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>

DragHandle::DragHandle(QObject *parent) :
    QObject(parent),
    m_dragCursorShape(Qt::CrossCursor)
{
}

int DragHandle::id(void) const
{
    return m_id;
}

void DragHandle::setId(int newId)
{
    m_id = newId;
}

void DragHandle::setDragCursorShape(Qt::CursorShape cursorShape)
{
    m_dragCursorShape = cursorShape;
}

Qt::CursorShape DragHandle::dragCursorShape(void) const
{
    return m_dragCursorShape;
}

QRectF DragHandle::hitArea(void) const
{
    return m_hitArea;
}

void DragHandle::setHitArea(const QRectF newHitArea)
{
    m_hitArea = newHitArea;
}

bool DragHandle::hitTest(const QPointF mouseModelPos) const
{
    return m_hitArea.contains(mouseModelPos);
}

void DragHandle::startDrag(const QPointF startPos)
{
    m_offset = startPos - m_hitArea.center();
    m_lastPos = startPos;
    emit posDragged(m_id, m_lastPos - m_offset);
}

void DragHandle::drag(const QPointF dragPos)
{
    m_lastPos = dragPos;
    emit posDragged(m_id, m_lastPos - m_offset);
}

//--

DragHandler::DragHandler(QWidget *parentWidget) :
    QObject(parentWidget),
    m_widget(parentWidget),
    m_currentDragHandle(NULL)
{
    Q_ASSERT_X(parentWidget, __FUNCTION__, "DragHandler requires a parent widget.");
    m_widget->setMouseTracking(true);
}

void DragHandler::addHandle(int newId, Qt::CursorShape cursorShape)
{
    DragHandle *handle = new DragHandle(this);
    handle->setId(newId);
    handle->setDragCursorShape(cursorShape);
    connect(handle, SIGNAL(posDragged(int,QPointF)), this, SIGNAL(posDragged(int,QPointF)));
    m_dragHandles.append(handle);
}

void DragHandler::setView2ModelTransform(const QTransform &view2Model)
{
    m_view2Model = view2Model;
}

void DragHandler::setHitAreaView(int id, QPointF viewPos, double halfWidth, double halfHeight)
{
    if (0 == halfHeight)
    {
        halfHeight = halfWidth;
    }
    QRectF viewRect(viewPos, QSize());
    viewRect.adjust(-halfWidth, -halfHeight, halfWidth, halfHeight);
    QRectF modelRect = m_view2Model.mapRect(viewRect);
    setHitArea(id, modelRect);
}

void DragHandler::setHitArea(int id, QRectF modelRect)
{
    DragHandle *handle = findHandleWithId(id);
    if (handle)
    {
        handle->setHitArea(modelRect);
    }
}

void DragHandler::handleMousePressEvent(QMouseEvent *e)
{
    QPointF mousePos = m_view2Model.map(
            #ifdef QT5
                e->localPos()
            #else
                e->posF()
            #endif
                );
    e->accept();
    foreach (DragHandle *handle, m_dragHandles)
    {
        if (handle->hitTest(mousePos))
        {
            m_widget->setCursor(Qt::ClosedHandCursor);
            m_currentDragHandle = handle;
            m_currentDragHandle->startDrag(mousePos);
            return;
        }
    }
}

void DragHandler::handleMouseMoveEvent(QMouseEvent *e)
{
    QPointF mousePos = m_view2Model.map(
            #ifdef QT5
                e->localPos()
            #else
                e->posF()
            #endif
                );
    e->accept();
    if (m_currentDragHandle)
    {
        m_currentDragHandle->drag(mousePos);
    }
    else
    {
        foreach (DragHandle *handle, m_dragHandles)
        {
            if (handle->hitTest(mousePos))
            {
                m_widget->setCursor(handle->dragCursorShape());
                return;
            }
        }
        m_widget->unsetCursor();
    }
}

void DragHandler::handleMouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    if (m_currentDragHandle)
    {
        m_currentDragHandle = NULL;
        m_widget->update();
    }
    m_widget->unsetCursor();
}

bool DragHandler::isDragging(void) const
{
    return (NULL != m_currentDragHandle);
}

DragHandle *DragHandler::findHandleWithId(int idToFind)
{
    foreach (DragHandle *handle, m_dragHandles)
    {
        if (handle->id() == idToFind)
        {
            return handle;
        }
    }
    return NULL;
}
