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

#ifndef FECONDITION_H
#define FECONDITION_H

#include "ForceEffect.h"

class FECondition : public ForceEffect
{
    Q_OBJECT
public:
    FECondition(quint16 forceType, QObject *parent = 0);

    virtual ForceCondition *firstCondition(void) const;
    virtual ForceCondition *secondCondition(void) const;

    virtual QString gist(void) const;

    virtual void reset(void);

protected:
    ForceCondition *m_firstCondition;
    ForceCondition *m_secondCondition;
};


#endif // FECONDITION_H
