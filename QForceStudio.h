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

#ifndef QFORCESTUDIO_H
#define QFORCESTUDIO_H

#include <math.h>

// our program version (should go into a build script, actually)
#define _VERSION_STR_     "0.1"

// I prefer tau over pi, honestly.
#ifndef M_TAU
#define M_TAU             6.2831853071795864769252867665590057683943387987502116
#define M_TAU_F           6.28318530717958647693f
#endif

// sizeof for arrays
#ifndef array_size
#define array_size(a)    (sizeof(a) / sizeof(a[0]))
#endif

// delete of a pointer (if set)
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)   if (x) { delete x; x = NULL; }
#endif

// Clip a value within the min/max given
#define CLIP(min,x,max)  qMax(min,qMin(max,x))
#define CLIP11(x)        qMax(-1.0,qMin(1.0,x))
#define CLIP01(x)        qMax(0.0,Qmin(1.0,x))

// make sure the angle is between [0.. M_TAU[
float normalizeAnglef(float angle);
double normalizeAngle(double angle);

// convert between radians and degrees
double convertRad2Deg(double angleInRad);
double convertDeg2Rad(double angleInDeg);

// to access the main window from almost anywhere...
class MainWindow;
MainWindow *mainWindowInstance(void);


#endif // QFORCESTUDIO_H
