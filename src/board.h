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

// Ranging sensors I2C port
#define RANGER_I2C Wire1

// Ranging sensors XSHUT pins
#define RANGER_0_XSHUT 9
#define RANGER_60_XSHUT 10
#define RANGER_120_XSHUT 11
#define RANGER_180_XSHUT 12
#define RANGER_240_XSHUT 13
#define RANGER_300_XSHUT 8

// roof connections
#define TEAM_SWITCH 53
#define MATCH_SWITCH 51
#define MODE_SWITCH 49
#define GREEN_LED 47
#define YELLOW_LED 45
#define RED_LED 43
#define START_JACK 41

#endif
