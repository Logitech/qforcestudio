/*
 * Emulation of linux/input.h on OSX. Allows compiling and running of
 * QForceStudio on OSX, although it won't actually work with any
 * attached devices.
 */

#ifndef INPUT_H
#define INPUT_H

typedef unsigned short __u16;
typedef   signed short __s16;
typedef unsigned   int __u32;

#define __user

struct ff_replay {
	__u16 length;
	__u16 delay;
};

struct ff_trigger {
	__u16 button;
	__u16 interval;
};

struct ff_envelope {
	__u16 attack_length;
	__u16 attack_level;
	__u16 fade_length;
	__u16 fade_level;
};

struct ff_constant_effect {
	__s16 level;
	struct ff_envelope envelope;
};

struct ff_ramp_effect {
	__s16 start_level;
	__s16 end_level;
	struct ff_envelope envelope;
};

struct ff_condition_effect {
	__u16 right_saturation;
	__u16 left_saturation;

	__s16 right_coeff;
	__s16 left_coeff;

	__u16 deadband;
	__s16 center;
};

struct ff_periodic_effect {
	__u16 waveform;
	__u16 period;
	__s16 magnitude;
	__s16 offset;
	__u16 phase;

	struct ff_envelope envelope;

	__u32 custom_len;
	__s16 __user *custom_data;
};

struct ff_rumble_effect {
	__u16 strong_magnitude;
	__u16 weak_magnitude;
};

struct ff_effect {
	__u16 type;
	__s16 id;
	__u16 direction;
	struct ff_trigger trigger;
	struct ff_replay replay;

	union {
		struct ff_constant_effect constant;
		struct ff_ramp_effect ramp;
		struct ff_periodic_effect periodic;
		struct ff_condition_effect condition[2]; /* One for each axis */
		struct ff_rumble_effect rumble;
	} u;
};

#define FF_RUMBLE	0x50
#define FF_PERIODIC	0x51
#define FF_CONSTANT	0x52
#define FF_SPRING	0x53
#define FF_FRICTION	0x54
#define FF_DAMPER	0x55
#define FF_INERTIA	0x56
#define FF_RAMP		0x57

#define FF_EFFECT_MIN	FF_RUMBLE
#define FF_EFFECT_MAX	FF_RAMP

#define FF_SQUARE	0x58
#define FF_TRIANGLE	0x59
#define FF_SINE		0x5a
#define FF_SAW_UP	0x5b
#define FF_SAW_DOWN	0x5c
#define FF_CUSTOM	0x5d

#define FF_WAVEFORM_MIN	FF_SQUARE
#define FF_WAVEFORM_MAX	FF_CUSTOM

#define FF_GAIN		0x60
#define FF_AUTOCENTER	0x61

#define FF_MAX		0x7f
#define FF_CNT		(FF_MAX+1)

#endif
