#ifndef MOTIONCONF_H
#define MOTIONCONF_H

#define MOTOR_ACTIVE_LEVEL HIGH
#define MOTOR_IDLE_LEVEL (MOTOR_ACTIVE_LEVEL == LOW ? HIGH : LOW)

#define STEP_PER_TURN 400
#define WHEEL_DIAMETER_MM 100
#define WHEEL_TO_CENTER_MM 138.5f // distance between a wheel contact point and center of all 4 contact points
#define MAGIC_TURN_COEFF 1.395 // not clear why but DEG_TO_SUBSTEP needs an additionnal coeff ~~black magic~~

// mm to halfticks conversion factor
#define MM_TO_HALFTICK (STEP_PER_TURN*2/(WHEEL_DIAMETER_MM*3.1416f))
// deg to halfticks conversion factor (rotation around robot's center)
#define DEG_TO_HALTICK (WHEEL_TO_CENTER_MM*6.2632f*MM_TO_HALFTICK*MAGIC_TURN_COEFF/360.0f)
// speed time unit to second conversion factor
// speed time unit is an arbitrary unit used to scale speed to avoid overflow or excessive precision loss in integer computations
#define SPEEDTU_TO_S 10

// speed conversion factor
#define MM_PER_S_TO_HALFTICK_PER_SPEEDTU (MM_TO_HALFTICK*SPEEDTU_TO_S)
#define DEG_PER_S_TO_HALFTICK_PER_SPEEDTU (DEG_TO_HALTICK*SPEEDTU_TO_S)
// acceleration conversion factor
#define MM_PER_S2_TO_HALFTICK_PER_SPEEDTU2 (MM_TO_HALFTICK*SPEEDTU_TO_S*SPEEDTU_TO_S)

#define DEFAULT_MAX_ACCELERATION (300*MM_PER_S2_TO_HALFTICK_PER_SPEEDTU2)
#define DEFAULT_MIN_SPEED (30*MM_PER_S_TO_HALFTICK_PER_SPEEDTU)

#endif
