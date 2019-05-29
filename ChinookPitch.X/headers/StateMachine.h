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


#ifndef __STATE_MACHINE_H__
#define	__STATE_MACHINE_H__

#include "Setup.h"
#include "StateFunctions.h"


//==============================================================================
// State Machine public function prototypes
//==============================================================================
void StateInit      (void);   // Initialization state of the system
void StateAcq       (void);   // First state. User can rename it as needed
//void State2         (void);   // Second state. User can rename it as needed
void StateError     (void);   // Error state. User should assess and corret errors in this state
void StateScheduler (void);   // State Scheduler. Decides which state is next
                              // depending on current state and flags. Used as a function



void StateIdlePitch(void);           // State where we stay when nothing is happening on the system.
void StateAcqPitch(void);            // Acquisition state 
void StateBrakePitch(void);          // Brake state (electrical brake)
void StateSendDataPitch(void);       // Send Data on CAN state
void StateMotorMotion(void);         // Move motor
void StateRegPitch(void);             // Regualtes the pitch to maintain position.
void StateCalibPitch(void);
void setUpPitch(void);
void setDownPitch(void);

//==============================================================================
// Variable declarations
//==============================================================================
void (*pStatePitch)(void);      // State pointer, used to navigate between states
volatile  INT8  errorFlag       // Flag indicating an error
               ; 

volatile float pitchValue,
        turbineValue,
                      setPitch;

//==============================================================================
// Macro definitions
//==============================================================================

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// State scheduler flags
// The state scheduler is at the end of each state and
// decides which state is next. Following flags
// are used in this decision. The names used can and
// should be renamed to improve readability. Also, the
// conditions tested in the defines should be changed
// to proper tests
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






#define INIT_2_IDLE          1

/***********************************
 * TRANSITION CONDITION INITIALIZATION
 **********************************/
#define IDLE_2_ACQ          oFlagAcq    
#define IDLE_2_DATA         oSendPitchValue
#define IDLE_2_MOTION       (oCmdDownPitch || oCmdUpPitch)
#define IDLE_2_CALIB        oCmdCalib
#define IDLE_2_BRAKE        oCmdBrake //|| (windTurbine > 1500.0)
#define IDLE_2_REG          oMaintainPitch

/***********************************
 * TRANSITION CONDITION ACQUISSITON
 **********************************/
#define ACQ_2_BRAKE         oCmdBrake// || (windTurbine > 1500.0)
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
#define MOTION_2_IDLE        (!oCmdDownPitch && !oCmdUpPitch)

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




#define PITCH_UP_CMD            oCmdUpPitch
#define PITCH_DOWN_CMD          oCmdDownPitch

#define PITCH_RESOLUTION        0.125








//==============================================================================
// Variable declarations
//==============================================================================
void (*pState)(void);       // State pointer, used to navigate between states
volatile  INT8  breakFlag   // Flag indicating if the emergency break has been pressed
               ,errorFlag   // Flag indicating an error
               ;

// Typedef for mapping the steering wheel switches and the buttons on the board
// All the buttons used : 3 on the steering wheel, 3 on the board
typedef union
{
  struct
  {
    UINT8  steerWheelSw1  : 1
          ,steerWheelSw3  : 1
          ,steerWheelSw10 : 1
          ,boardSw1       : 1
          ,boardSw2       : 1
          ,boardSw3       : 1
          ,               : 2
          ;
  } bits;

  UINT8 byte;

} Buttons_t;

typedef union
{
  struct
  {
    UINT8  steerWheelSw1  : 1
          ,steerWheelSw3  : 1
          ,steerWheelSw10 : 1
          ,boardSw1       : 1
          ,boardSw2       : 1
          ,boardSw3       : 1
          ,               : 2
          ;
  } bits;

  UINT8 byte;

} Chng_t;

typedef struct sButtonStates
{
  Buttons_t buttons;
  Chng_t    chng;
} sButtonStates_t;

#endif	/* __STATE_MACHINE_H__ */

volatile extern float windTurbine;