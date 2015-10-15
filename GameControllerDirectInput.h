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

#ifndef GAMECONTROLLER_DIRECT_INPUT_H
#define GAMECONTROLLER_DIRECT_INPUT_H

#include <QSharedPointer>
#include <QVector>
#include <atlbase.h>
#define DIRECTINPUT_VERSION     (0x0800)
#include <dinput.h>

#include "GameController.h"

class ForceEffect;

struct DirectInputEffectPairing
{
    ForceEffect* force;
    CComPtr<IDirectInputEffect> diEffect;
};

class GameControllerDirectInput : public GameController
{
    struct DirectInputDeviceEnumContext
    {
        IDirectInput8* pDI8;
        GameControllerPtrList* pDevices;
    };

    struct DirectInputEffectEnumContext
    {
        CComPtr<IDirectInputDevice8> pDIDevice;
        QVector<int>* pSupportedEffects;
    };

    struct DirectInputForceEffect
    {
        GUID type;
        DIEFFECT effect;
        DIENVELOPE envelope;
        DWORD axes[2];
        LONG direction[2];

        union
        {
            DICONDITION condition;
            DICUSTOMFORCE custon;
            DIPERIODIC  periodic;
            DICONSTANTFORCE constant;
            DIRAMPFORCE ramp;
        }t;
    };

    Q_OBJECT
public:
    explicit GameControllerDirectInput(CComPtr<IDirectInputDevice8> pDIDevice, QObject *parent = 0);
    virtual ~GameControllerDirectInput(void);

    // open/close
    virtual bool open(const QString &filename = QString());
    virtual void close(void);
    virtual bool isOpen(void) const;

    virtual QString name(void) const;
    virtual QString filename(void) const;
    virtual int numEffects(void) const;

    // enumeration
    static GameControllerPtrList allControllers(void);

    // forces on this device
    bool addForce(ForceEffect *force);
    bool startForce(ForceEffect *force);
    bool stopForce(ForceEffect *force);
    bool removeForce(ForceEffect *force);

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
    bool hasSupportFor(int capability) const;
    bool hasSupportForAutoCenter(void);
    bool hasSupportForFFGain(void);

private:
    static BOOL CALLBACK enumFFDevicesCallback(const DIDEVICEINSTANCE* pInst, LPVOID pContext);
    static BOOL CALLBACK enumFFDeviceEffectsCallback(LPDIEFFECTINFO pDIEffectInfo, LPVOID pContext);
    bool toDIEFFECT(ForceEffect *force, DirectInputForceEffect& dife);
    DirectInputEffectPairing* findEffectPairing(ForceEffect* force);
    double scale(double fromMin, double fromMax, double fromValue, double toMin, double toMax);

private:
    CComPtr<IDirectInputDevice8> m_pDIDevice;
    DIDEVICEINSTANCE m_diDeviceInfo;
    QVector<int> m_supportedEffects;
    QVector<DirectInputEffectPairing> m_diEffects;
    quint32 m_effectIdCounter;
    HWND m_hWnd;
};

#endif // GAMECONTROLLER_H
