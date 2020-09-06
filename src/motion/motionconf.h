#ifndef MOTIONCONF_H
#define MOTIONCONF_H

#define STEP_PER_TURN 200
#define WHEEL_DIAMETER_MM 60
#define WHEEL_TO_CENTER_MM 110.0f // distance between a wheel contact point and center of all 4 contact points
#define MAGIC_TURN_COEFF 1.017 // not clear why but DEG_TO_USTEP needs an additionnal coeff ~~black magic~~

// mm to halfticks conversion factor
#define MM_TO_USTEP (STEP_PER_TURN*256/(WHEEL_DIAMETER_MM*3.1416f))
// deg to halfticks conversion factor (rotation around robot's center)
#define DEG_TO_USTEP (WHEEL_TO_CENTER_MM*6.2632f*MM_TO_USTEP*MAGIC_TURN_COEFF/360.0f)

#define DEFAULT_MAX_ACCELERATION (220*MM_TO_USTEP) // in ustep/s2
#define DEFAULT_MIN_SPEED (20*256) // in ustep/s
#define DEFAULT_RECAL_SPEED (170*MM_TO_USTEP) // in ustep/s
#define DEFAULT_RECAL_DISTANCE (50*MM_TO_USTEP) // in ustep

#define DEFAULT_TURN_SPEED (90*DEG_TO_USTEP) // in ustep/s

#endif
