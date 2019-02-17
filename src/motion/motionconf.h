#ifndef MOTIONCONF_H
#define MOTIONCONF_H

#include "Motor.hpp"

#define STEP_PER_TURN 400
#define WHEEL_DIAMETER_MM 100
#define WHEEL_TO_CENTER_MM 138.5f // distance between a wheel contact point and center of all 4 contact points
#define MOTORLOOP_HALFPERIOD 125 // us
#define MAGIC_TURN_COEFF 1.395 // not clear why but DEG_TO_SUBSTEP needs an additionnal coeff ~~black magic~~

#define MM_TO_SUBSTEP (STEP_PER_TURN*MAX_SPEED/(WHEEL_DIAMETER_MM*3.1416f)) // mm to substep conversion factor
#define DEG_TO_SUBSTEP (WHEEL_TO_CENTER_MM*6.2632f*MM_TO_SUBSTEP*MAGIC_TURN_COEFF/360.0f) // deg to substep conversion factor (rotation around robot's center)
#define MM_PER_S_TO_SUBSTEP_PER_PERIOD (MM_TO_SUBSTEP*MOTORLOOP_HALFPERIOD*0.000001f*2)
#define DEG_PER_S_TO_SUBSTEP_PER_PERIOD (DEG_TO_SUBSTEP*MOTORLOOP_HALFPERIOD*0.000001f*2)
#define MM_PER_S2_TO_ACCUNIT (MM_TO_SUBSTEP*MOTORLOOP_HALFPERIOD*0.000001f*2*MOTORLOOP_HALFPERIOD*0.000001f*2*MAX_SUB_SPEED)

#define DEFAULT_MAX_ACCELERATION (600*MM_PER_S2_TO_ACCUNIT)
#define DEFAULT_MIN_SPEED (30*MM_PER_S_TO_SUBSTEP_PER_PERIOD)

#endif
