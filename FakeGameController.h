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

#ifndef FAKEGAMECONTROLLER_H
#define FAKEGAMECONTROLLER_H

#include "GameController.h"

class FakeGameController : public GameController
{
Q_OBJECT
public:
    explicit FakeGameController(QObject *parent = 0);
    virtual ~FakeGameController(void);

    // open/close
    virtual bool open(const QString &filename = QString());
    virtual void close(void);

    virtual bool addForce(ForceEffect *force);
    virtual bool startForce(ForceEffect *force);
    virtual bool stopForce(ForceEffect *force);
    virtual bool removeForce(ForceEffect *force);

    virtual bool stopAllForces(void);
    virtual bool pauseAllForces(void);
    virtual bool continueAllForces(void);
    virtual bool resetAllForces(void);
    virtual bool enableActuators(bool enable);

    virtual bool sendCommand(const QByteArray &command, QByteArray &response);

protected:
    virtual bool hasSupportFor(int capability) const;
};

#endif // FAKEGAMECONTROLLER_H
