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
int i =0 ;
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
 // pStatePitch = &StateInit;
 // InitPwm();
  
  // The tachy generates 2 pulses per revolution of the fan
  
  
  pStatePitch = &StateInit;
  
  /*
  while(1)
  {
      int x = 600;
      for ( n=0; n < 2000000; n++ )
      {
          x ^= 200;
      }
  
      oneStep(FWD	,FULSTEP);
  }
  */
  
  
	while(1)  //infinite loop
	{/*
    // <editor-fold defaultstate="collapsed" desc="LEDs">
    if (oTimer1)
    {
      oTimer1 = 0;
      switch (ledStates)
      {
        case 0 :
          LED_DEBUG4_ON;
          LED_DEBUG3_ON;
          LED_DEBUG2_OFF;
          LED_DEBUG1_OFF;
          LED_DEBUG0_OFF;
          ledStates = 1;
          break;
        case 1 :
          LED_DEBUG4_OFF;
          LED_DEBUG3_ON;
          LED_DEBUG2_ON;
          LED_DEBUG1_OFF;
          LED_DEBUG0_OFF;
          ledStates = 2;
          break;
        case 2 :
          LED_DEBUG4_OFF;
          LED_DEBUG3_OFF;
          LED_DEBUG2_ON;
          LED_DEBUG1_ON;
          LED_DEBUG0_OFF;
          ledStates = 3;
          break;
        case 3 :
          LED_DEBUG4_OFF;
          LED_DEBUG3_OFF;
          LED_DEBUG2_OFF;
          LED_DEBUG1_ON;
          LED_DEBUG0_ON;
          ledStates = 4;
          break;
        case 4 :
          LED_DEBUG4_ON;
          LED_DEBUG3_OFF;
          LED_DEBUG2_OFF;
          LED_DEBUG1_OFF;
          LED_DEBUG0_ON;
          ledStates = 0;
          break;
        default :
          ledStates = 0;
          break;
      }
    }
    // </editor-fold>
    */
        
        
        /*
        
    // <editor-fold defaultstate="collapsed" desc="Check changes on board">
    // <editor-fold defaultstate="collapsed" desc="Change on SW1 on board">
    if (buttons.buttons.bits.boardSw1 != SW1)
    {
      buttons.buttons.bits.boardSw1    = SW1;
      buttons.chng.bits.boardSw1       =   1;
     // LED_DEBUG1_TOGGLE;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Change on SW2 on board">
    if (buttons.buttons.bits.boardSw2 != SW2)
    {
      buttons.buttons.bits.boardSw2    = SW2;
      buttons.chng.bits.boardSw2       =   1;
     // LED_DEBUG2_TOGGLE;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Change on SW3 on board">
    if (buttons.buttons.bits.boardSw3 != SW3)
    {
      buttons.buttons.bits.boardSw3    = SW3;
      buttons.chng.bits.boardSw3       =   1;
     // LED_DEBUG3_TOGGLE;
    }
    // </editor-fold>
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Assess changes">
    if (buttons.chng.bits.steerWheelSw1)
    {
      buttons.chng.bits.steerWheelSw1 = 0;
      
//      if (1 == buttons.buttons.bits.steerWheelSw1)
//      {
//        if (!oFanIsOn)
//        {
//          Pwm.SetDutyCycle(PWM_3, 500);
//          oFanIsOn = 1;
//        }
//        else
//        {
//          Pwm.SetDutyCycle(PWM_3, 10);
//          oFanIsOn = 0;
//        }
//      }
    }
    if (buttons.chng.bits.boardSw1)
    {
      buttons.chng.bits.boardSw1 = 0;
      
      if (0 == buttons.buttons.bits.boardSw1)
      {
        if (!oFanIsOn)
        {
          Pwm.SetDutyCycle(PWM_3, 1000);
          oFanIsOn = 1;
        }
        else
        {
          Pwm.SetDutyCycle(PWM_3, 1000);
          oFanIsOn = 0;
        }
      }
    }
    
    if (buttons.chng.bits.steerWheelSw10)
    {
      buttons.chng.bits.steerWheelSw10 = 0;
      
      if (1 == buttons.buttons.bits.steerWheelSw10)
      {
        Port.D.ClearBits(BIT_3);
        Timer.DelayMs(50);
        Port.D.SetBits(BIT_3);
      }
    }
    if (buttons.chng.bits.boardSw3)
    {
      buttons.chng.bits.boardSw3 = 0;
      
      if (0 == buttons.buttons.bits.boardSw3)
      {
        Port.D.ClearBits(BIT_3);
        Timer.DelayMs(50);
        Port.D.SetBits(BIT_3);
      }
    }
    // </editor-fold>
  
    */
    
    // <editor-fold defaultstate="collapsed" desc="Commented code for state machine">
//  // State machine entry & exit point
//  //===========================================================
	(*pStatePitch)();          // jump to next state
//  //===========================================================
//
//  // State scheduler
//  //===========================================================
  // (*pStatePitch)();           // jump to next state

    StateScheduler();
//	//===========================================================
    // </editor-fold>
//    
//    
//    P12_1_ON ; //M1INA
//    P12_2_ON ; //M1INB
//    P12_3_ON ;//
//    P12_4_ON ;
//    P12_5_ON ; 
//    P12_6_ON ;
//    P12_7_ON ;
//    P12_8_ON ;
//    P12_9_ON ;
//  //  P12_15_ON ;
  // P12_16_ON ;
  
   // initShieldPins();
  
    
//            Pwm.SetDutyCycle  (PWM_2,710); // test a 25 %
//            Pwm.SetPulseOffset(PWM_2,500);
//
//
//            Pwm.SetDutyCycle  (PWM_3,710);
//            Pwm.SetPulseOffset(PWM_3,500);
//            
    
    
   // Pwm.SetDutyCycle  (PWM_2,710);
  //  Pwm.SetDutyCycle  (PWM_3,250);
    //oneStep(FWD,QRTRSTEP);
   
    
    if (1){
    
    //manuel 
        LED_DEBUG0_ON;
        
    }else 
    { 
        //auto
        LED_DEBUG0_OFF;   
    }
    
    

    if (!(READ_SW2) )
    
    {
    
        
        LED_DEBUG2_ON;
       
      for ( n=0; n < 300000; n++ )
      {
      
      }
  
      oneStep(FWD	,FULSTEP);  
        
    }else 
        {
    
    
    // LED_DEBUG2_OFF;
//        setPins(PHA,CUT);
//        setPins(PHB,CUT);
        }
    
        
    if(!(READ_SW3))
    {
        
      for ( n=0; n < 300000; n++ )
      {
      
      }
  
      LED_DEBUG3_ON;
      oneStep(BWD	,FULSTEP); 
                                                                                           
    }else
        {

      //  LED_DEBUG3_OFF;
    ////    setPins(PHA,CUT);
     //   setPins(PHB,CUT);
//            
      
    
        }

    
    
    }
	
} //END MAIN CODE