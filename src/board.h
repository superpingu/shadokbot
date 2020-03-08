// defines pin connections

#ifndef BOARD_H
#define BOARD_H

#define TMC_ENABLEN 52 // motor driver enable (active low)
#define TMC_INTN 48 // motor driver interrupt (active low)

// inverse rotation direction of motors
#define F_INVDIR false
#define RR_INVDIR false
#define RL_INVDIR false
// SPI chipselect pin of the motor drivers (active low)
#define F_CHIPSELECT 42
#define RL_CHIPSELECT 46
#define RR_CHIPSELECT 44

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
