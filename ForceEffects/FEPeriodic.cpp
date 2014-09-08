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

#include "FEPeriodic.h"
#include "QForceStudio.h"
#include <linux/input.h>

FEPeriodic::FEPeriodic(QObject *parent) :
    ForceEffect(FF_PERIODIC, parent)
{
    initializeEnvelope(&m_effect.u.periodic.envelope);

    reset();
}

int FEPeriodic::waveform(void) const
{
    return m_effect.u.periodic.waveform;
}

double FEPeriodic::level(void) const
{
    return m_effect.u.periodic.magnitude / 32767.0;
}

double FEPeriodic::offset(void) const
{
    double result = 0;
    if (m_effect.u.periodic.offset < 0)
    {
        result = m_effect.u.periodic.offset / 32768.0;
    }
    else
    {
        result = m_effect.u.periodic.offset / 32767.0;
    }
    return result;
}

QString FEPeriodic::gist(void) const
{
    QString result = ForceEffect::gist();
    result.append(QString("e.u.periodic.waveform = %1; // %2\n").arg(m_effect.u.periodic.waveform)
                  .arg(waveformType2GistString(m_effect.u.periodic.waveform)));
    result.append(QString("e.u.periodic.period = %1;\n").arg(m_effect.u.periodic.period));
    result.append(QString("e.u.periodic.magnitude = %1;\n").arg(m_effect.u.periodic.magnitude));
    result.append(QString("e.u.periodic.offset = %1;\n").arg(m_effect.u.periodic.offset));
    result.append(QString("e.u.periodic.phase = %1;\n").arg(m_effect.u.periodic.phase));
    result.append(envelopeGist(QLatin1String("e.u.periodic"), m_effect.u.periodic.envelope));
    return result;
}

void FEPeriodic::reset(void)
{
    ForceEffect::reset();
    setWaveform(FF_SQUARE);
    setPeriod(0.5);
    setLevel(0.75);
    setOffset(0.0);
    setPhase(0.0);
    setAttackLevel(0.25);
    setAttackLength(1.0);
    setFadeLength(1.0);
    setFadeLevel(1.0);
}

double FEPeriodic::value(double atTimeIndex) const
{
    if (0.001 <= period())
    {
        double wavePeriod = fmod(M_TAU * (atTimeIndex / period() + phase()), M_TAU);
        double wavePercent = wavePeriod / M_TAU;
        double val = offset();
        double totalLength = isInfiniteDuration() ? 0.0 : duration();
        double lev = envelopeLevel(atTimeIndex, totalLength);
        switch (waveform())
        {
            case FF_SQUARE:
                val += wavePeriod < M_PI ? lev : -lev;
                break;

            case FF_TRIANGLE:
                if (wavePercent <= 0.25)
                {
                    val += lev * (wavePercent * 4);
                }
                else if (wavePercent <= 0.75)
                {
                    val += lev * (1.0 - (wavePercent - 0.25) * 4);
                }
                else
                {
                    val += lev * ((wavePercent - 0.75) * 4 - 1);
                }
                break;

            case FF_SINE:
                val += lev * sin(wavePeriod);
                break;

            case FF_SAW_UP:
                val += lev * wavePercent;
                break;

            case FF_SAW_DOWN:
                val += lev * (1.0 - wavePercent);
                break;

            default:
                break;
        }
        return qMax(-1.0, qMin(1.0, val));
    }
    return 0.0;
}

void FEPeriodic::setWaveform(int newWaveform)
{
    newWaveform = qMax(FF_SQUARE, qMin(FF_SAW_DOWN, newWaveform));
    if (m_effect.u.periodic.waveform != newWaveform)
    {
        m_effect.u.periodic.waveform = newWaveform;
        emit waveformChange(newWaveform);
        emit valuesChanged();
        update();
    }
}

void FEPeriodic::setLevel(double newLevel)
{
    newLevel = qMax(0.0, qMin(1.0, newLevel));
    qint16 magnitude = qRound(newLevel * 32767.0);
    if (m_effect.u.periodic.magnitude != magnitude)
    {
        m_effect.u.periodic.magnitude = magnitude;
        emit levelChanged(newLevel);
        emit valuesChanged();
        update();
    }
}

void FEPeriodic::setOffset(double newOffset)
{
    newOffset = qMax(-1.0, qMin(1.0, newOffset));
    qint16 offset = 0;
    if (newOffset < 0.0)
    {
        offset = qRound(newOffset * 32768.0);
    }
    else
    {
        offset = qRound(newOffset * 32767.0);
    }
    if (m_effect.u.periodic.offset != offset)
    {
        m_effect.u.periodic.offset = offset;
        emit offsetChanged(newOffset);
        emit valuesChanged();
        update();
    }
}
