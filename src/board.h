// defines pin connections

#ifndef BOARD_H
#define BOARD_H

// front motor
#define F_EN 44
#define F_DIR 48
#define F_CK 52
#define F_INVDIR false

// front right motor
#define FR_EN 33
#define FR_DIR 37
#define FR_CK 41
#define FR_INVDIR true // inverse direction

// rear left motor
#define RL_EN 33
#define RL_DIR 37
#define RL_CK 41
#define RL_INVDIR false

// rear right motor
#define RR_EN 53
#define RR_DIR 49
#define RR_CK 45
#define RR_INVDIR false

// AX12 serial port
#define AX12_SERIALPORT Serial1

// LIDAR connections
#define LIDAR_SERIALPORT Serial2
#define LIDAR_DEV_EN 15
#define LIDAR_M_EN 14
#define LIDAR_M_SCTP 3

// battery probe analog input
#define BATT_PROBE 1

// IMU I2C port
#define IMU_I2C Wire

// roof connections
#define GREEN_LED 10
#define YELLOW_LED 9
#define RED_LED 8
#define TEAM_SWITCH 13
#define MATCH_SWITCH 12
#define MODE_SWITCH 11
#define START_JACK 7

// AX12 IDs
#define ARM_LEFT_ID 198
#define ARM_RIGHT_ID 199

#endif
