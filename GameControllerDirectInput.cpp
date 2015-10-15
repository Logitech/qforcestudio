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

#include "QForceStudio.h"
#include "GameControllerDirectInput.h"
#include "ForceEffect.h"
#include "MainWindow.h"
#include <QDebug>
#include <QtMath>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

extern MainWindow *s_mainWindowInstance;


GameControllerDirectInput::GameControllerDirectInput(CComPtr<IDirectInputDevice8> pDIDevice, QObject *parent) :
    GameController(parent),
    m_pDIDevice(pDIDevice),
    m_hWnd(NULL)
{
    // How do we get this?
    m_effectCount = 64;
    m_effectIdCounter = 1;

    // Get the device instance
    HRESULT hr = S_OK;
    ZeroMemory(&m_diDeviceInfo, sizeof(m_diDeviceInfo));
    m_diDeviceInfo.dwSize = sizeof(m_diDeviceInfo);
    hr = pDIDevice->GetDeviceInfo(&m_diDeviceInfo);

    // name
    m_name = QString::fromUtf16((unsigned short*)m_diDeviceInfo.tszProductName);

    // filename
    DIPROPGUIDANDPATH guidPath;
    ZeroMemory(&guidPath, sizeof(guidPath));
    guidPath.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    guidPath.diph.dwSize = sizeof(DIPROPGUIDANDPATH);

    hr = pDIDevice->GetProperty(DIPROP_GUIDANDPATH, (LPDIPROPHEADER)&guidPath);
    m_filename = QString::fromUtf16((unsigned short*)guidPath.wszPath);

    // get supported effects
    DirectInputEffectEnumContext diEnumContext = { pDIDevice, &m_supportedEffects };
    pDIDevice->EnumEffects((LPDIENUMEFFECTSCALLBACK)enumFFDeviceEffectsCallback, &diEnumContext, DIEFT_ALL);

    if (hasSupportForAutoCenter())
    {
        m_supportedEffects.append(FF_AUTOCENTER);
    }
    if (hasSupportForFFGain())
    {
        m_supportedEffects.append(FF_GAIN);
    }
}

GameControllerDirectInput::~GameControllerDirectInput(void)
{
    SAFE_DELETE(m_deviceCaps);
}

bool GameControllerDirectInput::open(const QString &filename)
{
    if (!m_pDIDevice)
    {
        return false;
    }

    // Already open
    return true;
}

void GameControllerDirectInput::close(void)
{
    if (m_pDIDevice)
    {
        m_pDIDevice->Unacquire();
        m_pDIDevice.Release();
        m_hWnd = NULL;
    }
}

bool GameControllerDirectInput::isOpen(void) const
{
    if (!m_pDIDevice)
    {
        return false;
    }

    return true;
}

QString GameControllerDirectInput::name(void) const
{
    return m_name;
}

QString GameControllerDirectInput::filename(void) const
{
    return m_filename;
}

int GameControllerDirectInput::numEffects(void) const
{
    return m_effectCount;
}

BOOL CALLBACK GameControllerDirectInput::enumFFDevicesCallback(const DIDEVICEINSTANCE* pInst, VOID* pContext)
{
    if (NULL == pContext)
    {
        return FALSE;
    }
    DirectInputDeviceEnumContext* pdiEnumContext = (DirectInputDeviceEnumContext*)pContext;

    CComPtr<IDirectInputDevice8> pDevice;
    HRESULT hr;

    // Obtain an interface to the enumerated force feedback device.
    hr = pdiEnumContext->pDI8->CreateDevice(pInst->guidInstance, &pDevice, NULL);
    if (FAILED(hr))
    {
        return DIENUM_CONTINUE;
    }

    GameControllerPtr gc = GameControllerPtr(new GameControllerDirectInput(pDevice));
    if (gc->open())
    {
        // Create a new game controller
        pdiEnumContext->pDevices->append(gc);
    }

    return DIENUM_CONTINUE;
}

BOOL CALLBACK GameControllerDirectInput::enumFFDeviceEffectsCallback(LPDIEFFECTINFO pDIEffectInfo, LPVOID pContext)
{
    if (NULL == pContext)
    {
        return FALSE;
    }

    bool hasPeriodicForce = false;
    DirectInputEffectEnumContext* pdiEnumContext = (DirectInputEffectEnumContext*)pContext;
    if (GUID_Sine == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_SINE);
        hasPeriodicForce = true;
    }
    if (GUID_Square == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_SQUARE);
        hasPeriodicForce = true;
    }
    if (GUID_Triangle == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_TRIANGLE);
        hasPeriodicForce = true;
    }
    if (GUID_SawtoothUp == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_SAW_UP);
        hasPeriodicForce = true;
    }
    if (GUID_SawtoothDown == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_SAW_DOWN);
        hasPeriodicForce = true;
    }
    if (GUID_ConstantForce == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_CONSTANT);
    }
    if (GUID_Inertia == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_INERTIA);
    }
    if (GUID_Damper == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_DAMPER);
    }
    if (GUID_Friction == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_FRICTION);
    }
    if (GUID_Spring == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_SPRING);
    }
    if (GUID_RampForce == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_RAMP);
    }
    if (GUID_CustomForce == pDIEffectInfo->guid)
    {
        pdiEnumContext->pSupportedEffects->append(FF_CUSTOM);
    }

    if (hasPeriodicForce)
    {
        pdiEnumContext->pSupportedEffects->append(FF_PERIODIC);
    }

    return DIENUM_CONTINUE;
}


#ifdef Q_OS_WIN
GameControllerPtrList GameController::allControllers(void)
{
    return GameControllerDirectInput::allControllers();
}
#endif

GameControllerPtrList GameControllerDirectInput::allControllers(void)
{
    GameControllerPtrList result;

    // Get handle to direct input
    CComPtr<IDirectInput8> pDI8;
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
        IID_IDirectInput8, (VOID**)&pDI8, NULL);
    if (!pDI8)
    {
        return result;
    }

    DirectInputDeviceEnumContext diEnumContext = { pDI8, &result };
    if (FAILED(pDI8->EnumDevices(DI8DEVCLASS_GAMECTRL,
        enumFFDevicesCallback, &diEnumContext,
        DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK)))
    {
        return result;
    }

    return result;
}

bool GameControllerDirectInput::addForce(ForceEffect *force)
{
    HRESULT hr = S_OK;

    if (!GameController::addForce(force))
    {
        return false;
    }

    if (!force)
    {
        emit errorMessage(tr("Can't add a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't download force on non-open device."));
        return false;
    }

    if (NULL == m_hWnd)
    {
        m_hWnd = (HWND)s_mainWindowInstance->effectiveWinId();
        hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick);
        hr = m_pDIDevice->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
        hr = m_pDIDevice->Acquire();
        hr = m_pDIDevice->SendForceFeedbackCommand(DISFFC_RESET);
    }

    DirectInputForceEffect dife;
    ZeroMemory(&dife, sizeof(dife));
    if (!toDIEFFECT(force, dife))
    {
        return false;
    }

    // Do we already have a force?
    DirectInputEffectPairing* pairing = findEffectPairing(force);
    if (NULL == pairing)
    {
        LPDIRECTINPUTEFFECT pDIEffect = NULL;
        HRESULT hr = m_pDIDevice->CreateEffect(dife.type, &dife.effect, &pDIEffect, NULL);
        if (SUCCEEDED(hr))
        {
            // Assign an incrementing id
            // NOTE: This is NOT the id from the FF driver or the FF device
            force->ffEffect()->id = m_effectIdCounter++;

            // Save it to our list
            DirectInputEffectPairing pairing = { force, pDIEffect };
            m_diEffects.append(pairing);
        }
    }
    else
    {
        // Update the force
        hr = pairing->diEffect->SetParameters(&dife.effect,
            DIEP_DURATION | DIEP_GAIN | DIEP_DIRECTION | DIEP_ENVELOPE | DIEP_TYPESPECIFICPARAMS | DIEP_START);
    }

    return true;
}

bool GameControllerDirectInput::startForce(ForceEffect *force)
{
    HRESULT hr = S_OK;

    if (!force)
    {
        emit errorMessage(tr("Can't start a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't start a force on non-open device."));
        return false;
    }

    if (NULL == m_hWnd)
    {
        m_hWnd = (HWND)s_mainWindowInstance->effectiveWinId();
        hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick);
        hr = m_pDIDevice->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
        hr = m_pDIDevice->Acquire();
        hr = m_pDIDevice->SendForceFeedbackCommand(DISFFC_RESET);
    }

    if (!force->isDownloaded())
    {
        if (!addForce(force))
        {
            return false;
        }
    }

    // Update the force
    DirectInputEffectPairing* pairing = findEffectPairing(force);
    if (NULL == pairing)
    {
        return false;
    }

    hr = pairing->diEffect->Start(1, 0);
    return true;
}

bool GameControllerDirectInput::stopForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't stop a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't stop a force on non-open device."));
        return false;    HRESULT hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick);

    }

    if (!force->isDownloaded())
    {
        qDebug() << "No need to stop a force that hasn't been downloaded.";
        return true;
    }

    DirectInputEffectPairing* pairing = findEffectPairing(force);
    if (NULL == pairing)
    {
        return false;
    }

    HRESULT hr = pairing->diEffect->Stop();
    return true;
}

bool GameControllerDirectInput::removeForce(ForceEffect *force)
{
    if (!force)
    {
        emit errorMessage(tr("Can't unload a NULL force."));
        return false;
    }

    if (!isOpen())
    {
        emit errorMessage(tr("Can't unload a force from a non-open device."));
        return false;
    }

    if (!force->isDownloaded())
    {
        qDebug() << "No need to remove a force that hasn't been downloaded.";
        return true;
    }

    for (int i = 0; i < m_diEffects.count(); i++)
    {
        if (m_diEffects[i].force == force)
        {
            m_diEffects[i].diEffect->Unload();
            m_diEffects[i].diEffect.Release();
            m_diEffects.remove(i);
            return true;
        }
    }

    return false;
}

bool GameControllerDirectInput::hasSupportFor(int capability) const
{
    return m_supportedEffects.contains(capability);
}

bool GameControllerDirectInput::hasSupportForAutoCenter(void)
{
    DIPROPDWORD prop;
    ZeroMemory(&prop, sizeof(prop));
    prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    prop.diph.dwSize = sizeof(DIPROPDWORD);
    prop.diph.dwHow = DIPH_DEVICE;

    // Check for auto center
    if (SUCCEEDED(m_pDIDevice->GetProperty(DIPROP_AUTOCENTER, (LPDIPROPHEADER)&prop)))
    {
        return (DIPROPAUTOCENTER_ON == prop.dwData);
    }

    return false;
}

bool GameControllerDirectInput::hasSupportForFFGain(void)
{
    DIPROPDWORD prop;
    ZeroMemory(&prop, sizeof(prop));
    prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    prop.diph.dwSize = sizeof(DIPROPDWORD);
    prop.diph.dwHow = DIPH_DEVICE;

    // Check for ff gain
    if (SUCCEEDED(m_pDIDevice->GetProperty(DIPROP_FFGAIN, (LPDIPROPHEADER)&prop)))
    {
        return (prop.dwData <= 10000);
    }

    return false;
}

bool GameControllerDirectInput::toDIEFFECT(ForceEffect *force, DirectInputForceEffect& dife)
{
    if ( (NULL == force) || (NULL == force->ffEffect()) )
    {
        return false;
    }

    ZeroMemory(&dife, sizeof(DirectInputForceEffect));
    dife.effect.dwSize = sizeof(DIEFFECT);
    dife.envelope.dwSize = sizeof(DIENVELOPE);

    dife.effect.dwFlags |= DIEFF_CARTESIAN;
    dife.effect.rglDirection = dife.direction;
    dife.effect.dwFlags |= DIEFF_OBJECTOFFSETS;
    dife.effect.cAxes = 1;
    dife.effect.rgdwAxes = dife.axes;
    dife.axes[0] = DIJOFS_X;
    int directionInDegrees = MulDiv(force->ffEffect()->direction, 359, 65535);
    float directionInRadians = qDegreesToRadians((float)directionInDegrees);
    qreal x = qSin(directionInRadians);
    dife.direction[0] = 0;

    switch(force->ffEffect()->type)
    {
        case FF_PERIODIC:
            dife.t.periodic.dwMagnitude = MulDiv(force->ffEffect()->u.periodic.magnitude, DI_FFNOMINALMAX, 32767);
            dife.t.periodic.dwPeriod = force->ffEffect()->u.periodic.period * 1000;
            // Special case if period is zero
            if (0 == dife.t.periodic.dwPeriod) dife.t.periodic.dwPeriod = INFINITE;
            dife.t.periodic.lOffset = MulDiv(force->ffEffect()->u.periodic.offset, DI_FFNOMINALMAX, 32767);
            dife.t.periodic.dwPhase = MulDiv(force->ffEffect()->u.periodic.phase, 35999, 65535);

            dife.effect.cbTypeSpecificParams = sizeof(DIPERIODIC);
            dife.effect.lpvTypeSpecificParams = &dife.t.periodic;
            switch(force->ffEffect()->u.periodic.waveform)
            {
                case FF_SQUARE:
                    dife.type = GUID_Square;
                    break;
                case FF_TRIANGLE:
                    dife.type = GUID_Triangle;
                    break;
                case FF_SINE:
                    dife.type = GUID_Sine;
                    break;
                case FF_SAW_UP:
                    dife.type = GUID_SawtoothUp;
                    break;
                case FF_SAW_DOWN:
                    dife.type = GUID_SawtoothDown;
                    break;
                case FF_CUSTOM:
                    dife.type = GUID_CustomForce;
                    break;
                default:
                    break;
            }

            dife.envelope.dwAttackLevel = force->attackLevel() * DI_FFNOMINALMAX;
            dife.envelope.dwAttackTime = force->attackLength() * 1000 * 1000;
            dife.envelope.dwFadeLevel = force->fadeLevel() * DI_FFNOMINALMAX;
            dife.envelope.dwAttackTime = force->fadeLength() * 1000 * 1000;
            break;

        case FF_CONSTANT:
            dife.type = GUID_ConstantForce;
            dife.t.constant.lMagnitude = (LONG)scale(-32767, 32767, force->ffEffect()->u.constant.level, -10000, 10000);
            // scale the constant and invert it because our UI has 'direction', not 'origin'
            dife.t.constant.lMagnitude = -1 * dife.t.constant.lMagnitude * x;
            dife.effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
            dife.effect.lpvTypeSpecificParams = &dife.t.constant;

            dife.envelope.dwAttackLevel = force->attackLevel() * DI_FFNOMINALMAX;
            dife.envelope.dwAttackTime = force->attackLength() * 1000 * 1000;
            dife.envelope.dwFadeLevel = force->fadeLevel() * DI_FFNOMINALMAX;
            dife.envelope.dwAttackTime = force->fadeLength() * 1000 * 1000;
            break;

        case FF_DAMPER:
        case FF_SPRING:
        case FF_INERTIA:
        case FF_FRICTION:
            dife.t.condition.dwNegativeSaturation = MulDiv(force->ffEffect()->u.condition->left_saturation, DI_FFNOMINALMAX, 65535);
            dife.t.condition.dwPositiveSaturation = MulDiv(force->ffEffect()->u.condition->right_saturation, DI_FFNOMINALMAX, 65535);
            dife.t.condition.lNegativeCoefficient = (LONG)scale(-32767, 32767, force->ffEffect()->u.condition->left_coeff, -10000, 10000);
            dife.t.condition.lPositiveCoefficient = (LONG)scale(-32767, 32767, force->ffEffect()->u.condition->right_coeff, -10000, 10000);
            dife.t.condition.lDeadBand = (LONG)scale(0, 65535, force->ffEffect()->u.condition->deadband, 0, 10000);
            dife.t.condition.lOffset = (LONG)scale(-32767, 32767, force->ffEffect()->u.condition->center, -10000, 10000);
            dife.effect.cbTypeSpecificParams = sizeof(DICONDITION);
            dife.effect.lpvTypeSpecificParams = &dife.t.condition;
            switch (force->ffEffect()->type)
            {
                case FF_DAMPER:
                    dife.type = GUID_Damper;
                    break;
                case FF_SPRING:
                    dife.type = GUID_Spring;
                    break;
                case FF_INERTIA:
                    dife.type = GUID_Inertia;
                    break;
                case FF_FRICTION:
                    dife.type = GUID_Friction;
                    break;
                default:
                    break;
            }
            break;

        case FF_RAMP:
            dife.type = GUID_RampForce;
            dife.t.ramp.lStart = (LONG)scale(-32767, 32767, force->ffEffect()->u.ramp.start_level, -10000, 10000);
            dife.t.ramp.lEnd = (LONG)scale(-32767, 32767, force->ffEffect()->u.ramp.end_level, -10000, 10000);
            // scale the constant and invert it because our UI has 'direction', not 'origin'
            dife.t.ramp.lStart = -1 * dife.t.ramp.lStart * x;
            dife.t.ramp.lEnd = -1 * dife.t.ramp.lEnd * x;
            dife.effect.cbTypeSpecificParams = sizeof(DIRAMPFORCE);
            dife.effect.lpvTypeSpecificParams = &dife.t.ramp;

            break;

        default:
            break;
    }

    // Forces define the gain
    dife.effect.dwGain = DI_FFNOMINALMAX;

    // Duration in microseconds
    dife.effect.dwDuration = force->isInfiniteDuration() ? INFINITE : force->duration() * 1000 * 1000;

    // Envelope
    dife.effect.lpEnvelope = &dife.envelope;

    // No trigger
    dife.effect.dwTriggerButton = DIEB_NOTRIGGER;

    return true;
}

DirectInputEffectPairing* GameControllerDirectInput::findEffectPairing(ForceEffect* force)
{
    foreach(const DirectInputEffectPairing& pairing, m_diEffects)
    {
        if (pairing.force == force)
        {
            return (DirectInputEffectPairing*)&pairing;
        }
    }

    return NULL;
}


//*************************************************************************
//
// GameControllerDirectInput::scale
//
//*************************************************************************

double GameControllerDirectInput::scale(double fromMin, double fromMax, double fromValue, double toMin, double toMax)
{
    double fFromValueN = (fromValue - fromMin) / (fromMax - fromMin);

    // now scale to the output
    double fToRange = toMax - toMin;

    return (toMin + (fFromValueN * fToRange));
}


bool GameControllerDirectInput::stopAllForces(void)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }
    return SUCCEEDED(m_pDIDevice->SendForceFeedbackCommand(DISFFC_STOPALL));
}

bool GameControllerDirectInput::pauseAllForces(void)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }
    return SUCCEEDED(m_pDIDevice->SendForceFeedbackCommand(DISFFC_PAUSE));
}

bool GameControllerDirectInput::continueAllForces(void)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }
    return SUCCEEDED(m_pDIDevice->SendForceFeedbackCommand(DISFFC_CONTINUE));
}

bool GameControllerDirectInput::resetAllForces(void)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }
    return SUCCEEDED(m_pDIDevice->SendForceFeedbackCommand(DISFFC_RESET));
}

bool GameControllerDirectInput::enableActuators(bool enable)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }
    return SUCCEEDED(m_pDIDevice->SendForceFeedbackCommand(enable ? DISFFC_SETACTUATORSON : DISFFC_SETACTUATORSOFF));
}

bool GameControllerDirectInput::sendCommand(const QByteArray& command, QByteArray& response)
{
    if (NULL == m_pDIDevice)
    {
        return false;
    }

    if (NULL == m_hWnd)
    {
        HRESULT hr;
        m_hWnd = (HWND)s_mainWindowInstance->effectiveWinId();
        hr = m_pDIDevice->SetDataFormat(&c_dfDIJoystick);
        hr = m_pDIDevice->SetCooperativeLevel(m_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
        hr = m_pDIDevice->Acquire();
    }

    // We need at least 1 byte
    if (command.size() < 1)
    {
        return false;
    }

    response.resize(512);

    DIEFFESCAPE diEscape;
    DWORD dwBytesReturned = 0;
    ZeroMemory(&diEscape, sizeof(diEscape));
    diEscape.dwCommand = command[0];
    diEscape.dwSize = sizeof(DIEFFESCAPE);
    diEscape.lpvInBuffer = (command.size() > 1) ? (LPVOID)(command.data() + 1) : NULL;
    diEscape.cbInBuffer = command.size() - 1;
    diEscape.lpvOutBuffer = (LPVOID)response.data();

    response.resize(diEscape.cbOutBuffer);

    return SUCCEEDED(m_pDIDevice->Escape(&diEscape));
}

