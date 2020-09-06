#ifndef TMC5130_CONSTANTS_H
#define TMC5130_CONSTANTS_H

#define TMC_GCONF      	0x00
#define TMC_GSTAT      	0x01
#define TMC_IFCNT      	0x02 // disabled when using SPI
#define TMC_SLAVECONF  	0x03 // unused when using SPI
#define TMC_INP_OUT    	0x04 // unused when using SPI
#define TMC_X_COMPARE  	0x05
#define TMC_IHOLD_IRUN 	0x10
#define TMC_TPOWERDOWN  0x11
#define TMC_TSTEP       0x12
#define TMC_TPWMTHRS    0x13
#define TMC_TCOOLTHRS  	0x14
#define TMC_THIGH      	0x15
// ramp generator related registers
#define TMC_RAMPMODE   	0x20
#define TMC_XACTUAL    	0x21
#define TMC_VACTUAL    	0x22
#define TMC_VSTART     	0x23
#define TMC_A1         	0x24
#define TMC_V1         	0x25
#define TMC_AMAX       	0x26
#define TMC_VMAX       	0x27
#define TMC_DMAX       	0x28
#define TMC_D1         	0x2A
#define TMC_VSTOP      	0x2B
#define TMC_TZEROWAIT 	0x2C
#define TMC_XTARGET    	0x2D

#define TMC_VDCMIN     	0x33
#define TMC_SWMODE     	0x34
#define TMC_RAMPSTAT   	0x35
#define TMC_XLATCH     	0x36

// encoder related registers
#define TMC_ENCMODE    	0x38
#define TMC_XENC       	0x39
#define TMC_ENC_CONST  	0x3A
#define TMC_ENC_STATUS 	0x3B
#define TMC_ENC_LATCH  	0x3C
// microstepping LUT programmation registers
#define TMC_MSLUT0     	0x60
#define TMC_MSLUT1     	0x61
#define TMC_MSLUT2     	0x62
#define TMC_MSLUT3     	0x63
#define TMC_MSLUT4     	0x64
#define TMC_MSLUT5     	0x65
#define TMC_MSLUT6     	0x66
#define TMC_MSLUT7     	0x67
#define TMC_MSLUTSEL   	0x68
#define TMC_MSLUTSTART 	0x69
#define TMC_MSCNT      	0x6A
#define TMC_MSCURACT   	0x6B
// driver configuration registers
#define TMC_CHOPCONF   	0x6C
#define TMC_COOLCONF   	0x6D
#define TMC_DCCTRL     	0x6E
#define TMC_DRVSTATUS  	0x6F
#define TMC_PWMCONF  		0x70
#define TMC_PWMSTATUS   0x71

#define TMC_EN_CTRL 		0x72 // not for normal use
#define TMC_LOST_STEPS 	0x73 // only when SD_MODE=1

// Ramp modes (Register TMC_RAMPMODE)
#define TMC_MODE_POSITION   0
#define TMC_MODE_VELPOS     1
#define TMC_MODE_VELNEG     2
#define TMC_MODE_HOLD       3

// switch mode bits (Register TMC_SWMODE)
#define TMC_SW_STOPL_ENABLE   0x0001
#define TMC_SW_STOPR_ENABLE   0x0002
#define TMC_SW STOPL_POLARITY 0x0004
#define TMC_SW_STOPR_POLARITY 0x0008
#define TMC_SW_SWAP_LR        0x0010
#define TMC_SW_LATCH_L_ACT    0x0020
#define TMC_SW_LATCH_L_INACT  0x0040
#define TMC_SW_LATCH_R_ACT    0x0080
#define TMC_SW_LATCH_R_INACT  0x0100
#define TMC_SW_LATCH_ENC      0x0200
#define TMC_SW_SG_STOP        0x0400
#define TMC_SW_SOFTSTOP       0x0800

// Status bits (Register TMC_RAMPSTAT)
#define TMC_RS_STOPL          0x0001
#define TMC_RS_STOPR          0x0002
#define TMC_RS_LATCHL         0x0004
#define TMC_RS_LATCHR         0x0008
#define TMC_RS_EV_STOPL       0x0010
#define TMC_RS_EV_STOPR       0x0020
#define TMC_RS_EV_STOP_SG     0x0040
#define TMC_RS_EV_POSREACHED  0x0080
#define TMC_RS_VELREACHED     0x0100
#define TMC_RS_POSREACHED     0x0200
#define TMC_RS_VZERO          0x0400
#define TMC_RS_ZEROWAIT       0x0800
#define TMC_RS_SECONDMOVE     0x1000
#define TMC_RS_SG             0x2000

#endif
