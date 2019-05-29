/*
 * dual_driver_vnh5019.h
 *
 *  Created on: Mar 24, 2018
 *      Author: Jerome Pare-Lepine
 */
#ifndef SRC_DUAL_DRIVER_VNH5019_H_
#define SRC_DUAL_DRIVER_VNH5019_H_

#include "Setup.h"


#define	FPGA_BANK	0x2U

#define PIN_M1INA	P12_1
#define PIN_M1INB	P12_2
#define	PIN_M1EN	P12_3
#define PIN_M2INA	P12_4
#define PIN_M2INB	P12_5
//#define	M1PWM	63	// Set elsewhere as TTC waveouts
//#define M2PWM	64
#define	PIN_M2EN	P12_6
#define	PIN_M1CS	P12_15
#define PIN_M2CS	P12_16


// HI means a pin is an output. The four LSBs are an SSI config
#define OUT_M1INA	 P12_1_ON
#define OUT_M1INB	 P12_2_ON
#define	OUT_M1EN	 P12_3_OFF
#define OUT_M2INA	 P12_4_ON
#define OUT_M2INB	 P12_5_ON
#define	OUT_M2EN	 P12_6_OFF
#define	OUT_M1CS	 P12_15_OFF
#define OUT_M2CS	 P12_16_OFF

#define OUT_REG		(OUT_M1INA|OUT_M1INB|OUT_M1EN|OUT_M2INA| \
					OUT_M2INB|OUT_M2EN|OUT_M1CS|OUT_M2CS)

#define HI		1
#define	LO		0

//#define FWD		0
//#define BWD		1 
#define FWD     1
#define BWD     0
#define	CUT		2

#define STEP1	0b000010010000
#define STEP2	0b000010010000
#define	STEP3	0b000010010000
#define STEP4	0b000010010000

// Depending on your EMIO setup in Vivado.
// I have 12 GPIO as EMIO, their offset ranging from 54 to <max>
#define PHA			0
#define PHB			1
#define PHA_OFST	4
#define PHB_OFST	7
#define PHA_FWD		(0x01 << PHA_OFST)
#define PHB_FWD		(0x01 << PHB_OFST)
#define PHA_CUT		(0x00 << PHA_OFST)
#define PHB_CUT		(0x00 << PHB_OFST)
#define PHA_BWD		(0x10 << PHA_OFST)
#define PHB_BWD		(0x10 << PHB_OFST)
#define	PHX_MSK_OFS	4
#define PHX_MASK	(0x1B << PHX_MSK_OFS)
#define SET_A_FWD	XGpioPs_WritePin(bankPins, PIN_M1INA, HI)

#define FULSTEP		4
#define HALFSTEP	2
#define	QRTRSTEP	1

//#define FULLPWMRANGE  1

#ifdef FULLPWMRANGE
#define fullstepPwm     710
#define halfstepPwm_a   25
#define halfstepPwm_b   999

#define qrtstepPwm_a   380
#define qrtstepPwm_b   920

#else


#define fullstepPwm     710/4
#define halfstepPwm_a   25
#define halfstepPwm_b   1000/4

#define qrtstepPwm_a   380/4
#define qrtstepPwm_b   920/4


#endif

void initShieldPins();
void disDriver();
void oneStep(int dir, int stepdiv);
void updateDriver(int phA,int phB,int dirA,int dirB);
void setPins(int phase, int dir);
void stepCmd(int* stpstate);


#endif /* SRC_DUAL_DRIVER_VNH5019_H_ */
