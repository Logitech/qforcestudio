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

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QSharedPointer>

class ForceEffect;
class GameController;
typedef QSharedPointer<GameController> GameControllerPtr;
typedef QList<GameControllerPtr> GameControllerPtrList;

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = 0);
    virtual ~GameController(void);

    // open/close
    virtual bool open(const QString &filename = QString());
    virtual void close(void);
    virtual bool isOpen(void) const;

    virtual QString name(void) const;
    virtual QString filename(void) const;
    virtual int numEffects(void) const;

    // capabilities
    bool canRenderConstant(void) const;
    bool canRenderPeriodic(void) const;
    bool canRenderSquare(void) const;
    bool canRenderTriangle(void) const;
    bool canRenderSine(void) const;
    bool canRenderSawUp(void) const;
    bool canRenderSawDown(void) const;
    bool canRenderCustom(void) const;
    bool canRenderRamp(void) const;
    bool canRenderSpring(void) const;
    bool canRenderFriction(void) const;
    bool canRenderDamper(void) const;
    bool canRenderRumble(void) const;
    bool canRenderInertia(void) const;
    bool canAdjustGain(void) const;
    bool canAdjustAutocenter(void) const;

    // enumeration
    static GameControllerPtrList allControllers(void);

    // forces on this device
    virtual bool addForce(ForceEffect *force);
    virtual bool startForce(ForceEffect *force);
    virtual bool stopForce(ForceEffect *force);
    virtual bool removeForce(ForceEffect *force);

    virtual bool stopAllForces(void);
    virtual bool pauseAllForces(void);
    virtual bool continueAllForces(void);
    virtual bool resetAllForces(void);
    virtual bool enableActuators(bool enable);

    virtual bool sendCommand(const QByteArray& command, QByteArray& response);

signals:
    void errorMessage(const QString &newMessage);

public slots:

protected:
    virtual bool hasSupportFor(int capability) const;

protected:
    int m_fd;
    QString m_filename;
    QString m_name;
    int m_effectCount;
    int m_deviceCapsByteCount;
    unsigned long *m_deviceCaps;
};

#endif // GAMECONTROLLER_H
