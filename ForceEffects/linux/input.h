#pragma once

typedef struct ff_envelope
{
    unsigned short attack_length;
    unsigned short attack_level;
    unsigned short  fade_length;
    unsigned short  fade_level;
}ff_envelope;

typedef struct ff_constant_effect
{
    short level;
    struct ff_envelope envelope;
}ff_constant_effect;

typedef struct ff_condition_effect
{
    unsigned short right_saturation;
    unsigned short left_saturation;
    short right_coeff;
    short left_coeff;
    unsigned short deadband;
    short center;
}ff_condition_effect;

typedef struct ff_periodic_effect
{
    unsigned short waveform;
    unsigned short period;
    short magnitude;
    short offset;
    unsigned short phase;

    struct ff_envelope envelope;

    unsigned int custom_len;
    short *custom_data;
}ff_periodic_effect;

typedef struct ff_ramp_effect
{
    short start_level;
    short end_level;
    struct ff_envelope envelope;
}ff_ramp_effect;

typedef struct ff_rumble_effect {
    unsigned short strong_magnitude;
    unsigned short weak_magnitude;
}ff_rumble_effect;

typedef struct ff_replay {
    unsigned short length;
    unsigned short delay;
}ff_replay;

typedef struct ff_trigger {
    unsigned short button;
    unsigned short interval;
}ff_trigger;

typedef struct ff_effect
{
    unsigned short type;
    short id;
    unsigned short direction;
    struct ff_trigger trigger;
    struct ff_replay replay;

    union {
        struct ff_constant_effect constant;
        struct ff_ramp_effect ramp;
        struct ff_periodic_effect periodic;
        struct ff_condition_effect condition[2]; /* One for each axis */
        struct ff_rumble_effect rumble;
    } u;
}ff_effect;



/*
* Force feedback effect types
*/

#define FF_RUMBLE       0x50
#define FF_PERIODIC     0x51
#define FF_CONSTANT     0x52
#define FF_SPRING       0x53
#define FF_FRICTION     0x54
#define FF_DAMPER       0x55
#define FF_INERTIA      0x56
#define FF_RAMP         0x57

#define FF_EFFECT_MIN   FF_RUMBLE
#define FF_EFFECT_MAX   FF_RAMP

/*
 * Force feedback periodic effect types
 */

#define FF_SQUARE       0x58
#define FF_TRIANGLE     0x59
#define FF_SINE         0x5a
#define FF_SAW_UP       0x5b
#define FF_SAW_DOWN     0x5c
#define FF_CUSTOM       0x5d

#define FF_WAVEFORM_MIN FF_SQUARE
#define FF_WAVEFORM_MAX FF_CUSTOM

/*
 * Set ff device properties
 */

#define FF_GAIN         0x60
#define FF_AUTOCENTER   0x61

#define FF_MAX          0x7f
#define FF_CNT          (FF_MAX + 1)

#define _USE_MATH_DEFINES
#include <math.h>
