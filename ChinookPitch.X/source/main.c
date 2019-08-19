//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : main.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the main C file of the template project used by all
//           developpers of Chinook. It uses ChinookLib, which is another
//           repository on Github.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include "Setup.h"
#include "SkadiFunctions.h"
#include "Interrupts.h"
#include "StateMachine.h"
#include "HardwareProfile.h"

#include "dual_driver_vnh5019.h"

//==============================================================================
// FUNCTION PROTOTYPES
//==============================================================================


//==============================================================================
// VARIABLE DECLARATIONS
//==============================================================================
int i = 0;
int n = 0; 
// All the buttons used. 3 on the steering wheel, 3 on the board
volatile sButtonStates_t buttons =
{
  .chng   .byte = 0
 ,.buttons.byte = 0

 // switches on board are at 1 when not pressed
 ,.buttons.bits.boardSw1 = 1
 ,.buttons.bits.boardSw2 = 1
 ,.buttons.bits.boardSw3 = 1
};

//BOOL oFanIsOn = 0;
BOOL oFanIsOn = 1;
UINT8 ledStates = 0;

extern volatile BOOL oTimer1;

//==============================================================================
// MAIN CODE
//==============================================================================
void main(void)
{
//==============================================================================
// Following memcopies are used to be able to use the form 
// Chinook.LibraryX.FunctionX in case the developer wants to see the available
// functions.
// IMPORTANT NOTE : the variables structures (example : Adc.Var...) WON'T be
//                  updated. Only use Chinook.Lib.Function format for the
//                  functions.
//==============================================================================
    memcpy( &Chinook.Port  , &Port  , sizeof ( struct sChinookPort  ) );
    memcpy( &Chinook.Uart  , &Uart  , sizeof ( struct sChinookUart  ) );
    memcpy( &Chinook.Pwm   , &Pwm   , sizeof ( struct sChinookPwm   ) );
    memcpy( &Chinook.Timer , &Timer , sizeof ( struct sChinookTimer ) );
    memcpy( &Chinook.Spi   , &Port  , sizeof ( struct sChinookSpi   ) );
    memcpy( &Chinook.Wdt   , &Wdt   , sizeof ( struct sChinookWdt   ) );
    memcpy( &Chinook.Adc   , &Adc   , sizeof ( struct sChinookAdc   ) );
    memcpy( &Chinook.Can   , &Can   , sizeof ( struct sChinookCan   ) );
    memcpy( &Chinook.I2c   , &I2c   , sizeof ( struct sChinookI2c   ) );
    memcpy( &Chinook.Skadi , &Skadi , sizeof ( struct sChinookSkadi ) );
    memcpy( &Chinook.InputCapture , &InputCapture , sizeof ( struct sChinookInputCapture ) );
//==============================================================================


//==============================================================================
// The next line disables the JTAG for the PIC. If the JTAG is enabled, pins
// RB10-13 can't be used as I/Os. If you want to use the JTAG, comment or
// remove this line.
//==============================================================================
    DDPCONbits.JTAGEN = 0;
//==============================================================================


//==============================================================================
// Configure the device for maximum performance but do not change the PBDIV
// Given the options, this function will change the flash wait states, RAM
// wait state and enable prefetch cache but will not change the PBDIV.
// The PBDIV value is already set via the pragma FPBDIV option in HardwareProfile.h.
//==============================================================================
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
//==============================================================================


// State machine init
//=================================================================
//  if (Wdt.ReadEvent())    // If a system reset has occured
//  {
//    Wdt.Disable();        // Disable WDT and
//    Wdt.ClearEvent();     // the WDT event bit
//    pState = &StateError; // Go to StateError
//  }
//  else
//  {
//    pState = &StateInit;  // Else, initialize the system as usual
//  }
//=================================================================
  
    StateInit();
  
    pStatePitch = &StateInit;
    
    oPitchMode = PITCH_MODE_AUTOMATIC;
    
    for(i = 0; i < 10000; ++i);
    SEND_PITCH_MODE;
    
    Can.SendData(CAN1, 0x35, 1);
  
	while(1)  //infinite loop
	{
        static int last_mode = PITCH_MODE_AUTOMATIC;
        if(last_mode != oPitchMode)
        {
            last_mode = oPitchMode;
            SEND_PITCH_MODE;
        }
        // If the ROPS bool is active, disregard everything and BRAKE MARIO !
        if(bROPS)
            LED_DEBUG3_ON();
        else
            LED_DEBUG3_OFF();
        if(oPitchMode == PITCH_MODE_MANUAL)
            LED_DEBUG4_ON();
        else
            LED_DEBUG4_OFF();
        
        
        
        while(bROPS)
        {
            int n;
            int direction;
            if(current_pitch < drapeau_pitch)
                direction = FWD;
            else
                direction = BWD;
            
            float delta = current_pitch - drapeau_pitch;
            if(abs(delta) >= 1.01)
            {
                // Quarter step in the direction
                oneStep(direction, FULSTEP);

                // RPM delay  --  frequency = 4 MHZ
                for(n = 0; n < 100000; ++n);
            }
        }
        
        (*pStatePitch)(); // jump to next state
        StateScheduler();
    }
} //END MAIN CODE