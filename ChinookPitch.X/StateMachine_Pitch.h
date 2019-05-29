//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine.h
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//////////////
// Revision //
//////////////
//
// Author  : Gabriel Jean-Louis
// Date    : 2017-07-08
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the state machine of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : State names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//#ifndef STATE_MACHINE_PITCH_H
//#define	STATE_MACHINE_PITCH_H
#pragma once

#include "..\headers\Setup.h"
#include "..\headers\StateFunctions.h"
#include "..\headers\Potentiometer.h"


//==============================================================================
// State Machine public function prototypes
//==============================================================================
void StateInitPitch(void);           // Initialization state of the system
void StateIdlePitch(void);           // State where we stay when nothing is happening on the system.
void StateAcqPitch(void);            // Acquisition state 
void StateCalibPitch(void);          // Calibration state                 
void StateBrakePitch(void);          // Brake state (electrical brake)
void StateSendDataPitch(void);       // Send Data on CAN state
void StateMotorMotion(void);         // Move motor
void StateRegPitch(void);             // Regualtes the pitch to maintain position.
void StateErrorPitch(void);          // Error state. User should assess and correct errors in this state
void StateSchedulerPitch(void);      // State Scheduler. Decides which state is next

//==============================================================================
// Variable declarations
//==============================================================================
void (*pStatePitch)(void);      // State pointer, used to navigate between states
volatile  INT8  errorFlag       // Flag indicating an error
               ; 

//==============================================================================
// Macro definitions
//==============================================================================
/***********************************
 * PITCH STATE MACHINE PARAMETERS
 **********************************/
#define PITCH_UP                0
#define PITCH_DOWN              1
#define TURB_OVERSPEED          windTurbine > 1500.0    // Wind turbine speed over 1700RPM
#define RELEASE_BRAKE           windTurbine < 1000.0    // Release aerobrake when turbine 1000RPM

#define AERO_BRAKE              oCmdBrake 
#define LIMIT_SW_FLAG           oFlagLmtSw
#define LIMIT_SW_HI             oFlagSwHi
#define LIMIT_SW_LOW            oFlagSwLw
#define PITCH_UP_CMD            oCmdUpPitch
#define PITCH_DOWN_CMD          oCmdDownPitch
#define CALIB                   oCmdCalib
#define REG_PITCH_PLUS          (Pitch.error > (Pitch.posMaintain - Pitch.pos))
#define REG_PITCH_MINUS         (Pitch.error > (Pitch.pos - Pitch.posMaintain))
#define REG_PITCH               abs(Pitch.posMaintain - Pitch.pos) > Pitch.error //|| (Pitch.error < Pitch.pos - Pitch.posMaintain)
#define PITCH_OK                (Pitch.pos == Pitch.posMaintain - Pitch.error) || (Pitch.pos == Pitch.posMaintain + Pitch.error)

/***********************************
 * BASIC CONDITION
 **********************************/
/***********************************
 * TRANSITION CONDITION INITIALIZATION
 **********************************/
#define INIT_2_IDLE          1

/***********************************
 * TRANSITION CONDITION INITIALIZATION
 **********************************/
#define IDLE_2_ACQ          oFlagAcq    
#define IDLE_2_DATA         oSendPitchValue
#define IDLE_2_MOTION       (oCmdDownPitch || oCmdUpPitch)
#define IDLE_2_CALIB        oCmdCalib
#define IDLE_2_BRAKE        oCmdBrake || (windTurbine > 1500.0)
#define IDLE_2_REG          oMaintainPitch

/***********************************
 * TRANSITION CONDITION ACQUISSITON
 **********************************/
#define ACQ_2_BRAKE         oCmdBrake || (windTurbine > 1500.0)
#define ACQ_2_IDLE          1

/***********************************
 * TRANSITION CONDITION CALIBRATION
 **********************************/
#define CALIB_2_SEND         oCmdCalib
#define CALIB_2_IDLE         0

/***********************************
 * TRANSITION CONDITION MOTION
 **********************************/
#define MOTION_2_ACQ         oFlagAcq
#define MOTION_2_IDLE        oPitchDone //(!oCmdDownPitch && !oCmdUpPitch)

/***********************************
 * TRANSITION CONDITION BRAKE AERO
 **********************************/
#define BRAKE_2_IDLE         0
#define BRAKE_2_ACQ          oFlagAcq

/***********************************
 * TRANSITION CONDITION PITCH REGULATOR
 **********************************/
#define REG_2_IDLE          !oMaintainPitch
#define REG_2_ACQ           oFlagAcq

/***********************************
 * TRANSITION CONDITION SENDDATA
 **********************************/
#define DATA_2_IDLE          1

/***********************************
 * TRANSITION CONDITION  ERROR
 **********************************/
#define ERROR_2_IDLE         1
//#define ERROR_2_BRAKE        0 
//#define ERROR_2_CALIB        0 
//#define ERROR_2_DATA         0 
//#define ERROR_2_UP           0 
//#define ERROR_2_BRAKE        0 

//==============================================================================
// Variable declarations
//==============================================================================
volatile extern float windTurbine;

volatile extern UINT32 pitchStep,
                       maintPitch;

volatile extern BOOL  oCmdUpPitch,
                      oCmdDownPitch,
                      oCmdBrake,
                      oCmdCalib,
                      oSendIdPitch,
                      oSendPitchValue,
                      oAdcReady,
                      oMotorStop,
                      oFlagAcq,
                      oFlagSwHi,
                      oFlagSwLw,
                      oLoopSkadi,
                      debugEn,
                      driveAEn,
                      driveBEn,
                      oMaintainPitch,
                      oPitchDone,
                      oFlagStep;

//volatile BOOL firstRun = 0;

 //BOOL firstRun = 0;

typedef struct {
    UINT32  step,
            backlash,
            posMaintain,
            pos,
            error;
}sPitch;

sPitch Pitch;

//#endif	/* STATE_MACHINE_PITCH_H */

    