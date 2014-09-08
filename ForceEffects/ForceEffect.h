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

#ifndef FORCEEFFECT_H
#define FORCEEFFECT_H

#include <QObject>
#include <QPointer>
#include <linux/input.h>
#include "ForceEnvelope.h"
#include "ForceCondition.h"

class GameController;

class ForceEffect : public QObject
{
    Q_OBJECT
public:
    explicit ForceEffect(quint16 forceType, QObject *parent = 0);

    static double convertFF2RAD(quint16 direction);
    static quint16 convertRAD2FF(double direction);

    // device we're operating on
    void setGameController(GameController *controller);
    ff_effect *ffEffect(void);
    virtual QString gist(void) const;
    static QString forceType2GistString(int forceType);
    static QString waveformType2GistString(int waveform);
    static QString envelopeGist(const QString &prefix, const ff_envelope &env);
    bool isDownloaded(void) const;

    void start(void);
    void stop(void);
    virtual void reset(void);
    virtual void update(void);

    // in RADIANS, [0.0 ... M_TAU[
    double direction(void) const;

    // in seconds, [0.0 ... 32.767]
    double delay(void) const;

    // in seconds, [0.001 ... 32.767], returns 0 if infinite
    double duration(void) const;
    bool isInfiniteDuration(void) const;

    // as coefficient, [0.0 ... 1.0] - value depends on force type.
    virtual double level(void) const;

    // in seconds, [0.001 ... 32.767]
    virtual double period(void) const;

    // in RADIANS, [0.0 ... M_TAU[
    virtual double phase(void) const;

    // as coefficient, [-1.0 ... 1.0] - value depends on force type.
    virtual double offset(void) const;

    // as coefficients, [0.0 ... 1.0]
    double attackLevel(void) const;
    double fadeLevel(void) const;

    // in seconds, [0.0 ... 32.767]
    double attackLength(void) const;
    double fadeLength(void) const;

    bool withinAttackLength(double atTimeIndex) const;
    bool withinFadeLength(double atTimeIndex, double totalLength) const;

    // as force level, [-1.0 ... 1.0] - final value of periodic/constant, given the time offset
    virtual double envelopeLevel(double atTimeIndex, double totalLength) const;
    virtual double value(double atTimeIndex) const;

    // for condition forces
    virtual ForceCondition *firstCondition(void) const;
    virtual ForceCondition *secondCondition(void) const;

signals:
    void directionChanged(double newDirection);
    void delayChanged(double newDelay);
    void durationChanged(double newDuration);
    void durationIsInfiniteChanged(bool isNowInfinite);
    void statusChanged(const QString &newStatus);
    void levelChanged(double newLevel);
    void periodChanged(double newPeriod);
    void phaseChanged(double newPhase);
    void offsetChanged(double newOffset);
    void valuesChanged(void);

public slots:
    void setDirection(double newDirection);
    void setDelay(double newDelay);
    void setDuration(double newDuration, bool setInfinite = false);
    virtual void setLevel(double newLevel);
    virtual void setPeriod(double newPeriod);
    virtual void setPhase(double newPhase);
    virtual void setOffset(double newOffset);
    void setAttackLevel(double newLevel);
    void setAttackLength(double newLength);
    void setFadeLevel(double newLevel);
    void setFadeLength(double newLength);
    void onErrorMessage(const QString &newMessage);

protected:
    void initializeEnvelope(ff_envelope *env);

protected:
    ff_effect m_effect;
    ForceEnvelope *m_envelope;
    QPointer<GameController> m_gameController;
};

typedef QList<QPointer<ForceEffect> > ForceEffectList;

#endif // FORCEEFFECT_H
