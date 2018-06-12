// defines pin connections

#ifndef BOARD_H
#define BOARD_H

#define MOTOR_ACTIVE_LEVEL LOW
#define MOTOR_IDLE_LEVEL (MOTOR_ACTIVE_LEVEL == LOW ? HIGH : LOW)

// rear left motor
#define RL_EN 13
#define RL_CK 11
#define RL_DIR 12
#define RL_INV false // inverse direction

// rear right motor
#define RR_EN 8
#define RR_DIR 9
#define RR_CK 10
#define RR_INV true // inverse direction

// front left motor
#define FL_EN 2
#define FL_DIR 3
#define FL_CK 4
#define FL_INV true // inverse direction

// front right motor
#define FR_EN 7
#define FR_DIR 6
#define FR_CK 5
#define FR_INV false // inverse direction

#endif
