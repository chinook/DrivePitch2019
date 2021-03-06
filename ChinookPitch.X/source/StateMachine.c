//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine.c
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the state machine of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : State names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "StateMachine.h"
#include "Interrupts.h"
#include "dual_driver_vnh5019.h"

//==============================================================================
//	STATES OF STATE MACHINE
//==============================================================================

volatile BOOL firstRun = 0 ;


//===============================================================
// Name     : StateScheduler
// Purpose  : Decides which state is next depending on current
//            state and flags. Used as a function
//===============================================================
void StateScheduler(void)
{
  // Clear WDT
  //============================
  Wdt.Clear();
  //============================
  
  if (pStatePitch == &StateInit)
  {
    pStatePitch = &StateIdlePitch;        // Idle state
  }
  
  if (pStatePitch == &StateIdlePitch)
  {
    if((oCmdDownPitch || oCmdUpPitch || (!IsPitchDone())))
    {
      pStatePitch = &StateMotorMotion;    // Motion state
    }
    else
    {
      pStatePitch = &StateIdlePitch;      // Go to Error state by default
    }
  }
  else if (pStatePitch == &StateMotorMotion)
  {
      pStatePitch = &StateIdlePitch;       // Acquisition state
  }
  else
  {
    pStatePitch = &StateIdlePitch;       // Go to Error state by default
  }
}

//===============================================================
// Name     : StateInit
// Purpose  : Initialization of the system.
//===============================================================
void StateInit(void)
{
    INTDisableInterrupts();   // Disable all interrupts of the system.
    initShieldPins();
    InitPorts();
//  InitSpi();
    InitTimer();
//  InitInputCapture();
    InitPwm();
//  InitAdc();
//  InitUART();
//  InitSkadi();
    InitCan();
//  InitI2c();
//  InitWdt();
    StartInterrupts();
    
    current_pitch = 0;
    target_pitch = 0;
    
    firstRun = 1;
    //oPitchMode = 1; 
    oFlagAcq = 1;
}

void StateMotorMotion(void)
{
    // Make sure commands up and down are 0 in automatic mode
    if(oPitchMode == PITCH_MODE_AUTOMATIC)
    {
        oCmdUpPitch = 0;
        oCmdDownPitch = 0;
    }
        
    int n = 0;
    for(n = 0; n < 50000; ++n);
    
    if(oPitchMode == PITCH_MODE_MANUAL)
    {
        if (oCmdUpPitch)
        {
            LED_DEBUG0_ON();
            oneStep(FWD, HALFSTEP);
            oCmdDownPitch = 0;
            oPitchDone = 1;
        }
        else
        {
            LED_DEBUG0_OFF();
        }

        if (oCmdDownPitch)
        {
            LED_DEBUG1_ON();
            oneStep(BWD, HALFSTEP);
            oCmdUpPitch = 0;
            oPitchDone = 1;
        }
        else 
        {
             LED_DEBUG1_OFF();
        }
        
    }
    else if(oPitchMode == PITCH_MODE_AUTOMATIC)
    {
        // Failsafe for "vers l'infini et l'au-dela"
        //if(abs(target_pitch - current_pitch) > 1000)
        //    target_pitch = current_pitch;
        
        int n; 
        for (n = 0; n < 10000; n++);
        
        int direction;
        if(current_pitch < target_pitch)
            direction = FWD;
        else
            direction = BWD;
        
        /*
        static float direction_avg = 0;
        direction_avg += (float)direction;
        static int count = 0;
        ++count;
        if(count >= 9)
        {
            count = 0;
            float avg = direction_avg / 10;
            if(abs(avg - 0.5) >= 0.1f)
            {
                oPitchDone = 1;
                Can.SendData(CAN1, 0x35, 1);
                break;
            }
        }
        */
        
        //while(current_pitch != target_pitch)
        if(current_pitch != target_pitch)
        {
            int l = 0;
            for(l = 0; l < 10; ++l)
            {
                if(bROPS)
                    break;
                // Quarter step in the direction
                oneStep(direction, HALFSTEP);

                // Check if we're done
                float delta = target_pitch - current_pitch;
                if(abs(delta) <= 2.01f)
                {
                    oPitchDone = 1;
                    // Send pitch done CAN message
                    Can.SendData(CAN1, 0x35, 1);
                    break;
                }

                for(n = 0; n < 100000; ++n);
            }
        }
    }
}

void StateIdlePitch(void)
{
    // Disable holding torque on stepper
    if(IsPitchDone() && !oCmdDownPitch && !oCmdUpPitch)
    {
        updateDriver(0, 0, FWD, FWD);
        //Can.SendData(CAN1, 0x35, 1);
    }
    
    // Failsafe in case we missed the pitch done CAN message
    static int mem_pitch_done = 0;
    if(!mem_pitch_done && oPitchDone)
    {
        Can.SendData(CAN1, 0x35, 1);
        mem_pitch_done = 1;
    }
    
    int i;
    for(i = 0; i < 10000; ++i);
    
    //oPitchMode = PITCH_MODE_MANUAL;
    
    // Assess buttons
    // Forward stepping if SW2 is pressed
    
    //oCmdDownPitch = 0;
    static int last_cmd_down = 0;
    if (!(READ_SW2()))
    {
        last_cmd_down = 1;
        oPitchMode = PITCH_MODE_MANUAL;
        oCmdDownPitch = 1;
        //LED_DEBUG4_ON();
    }
    else if(last_cmd_down)
    {
        last_cmd_down = 0;
        oCmdDownPitch = 0;
    }

    // Backwards stepping if SW3 is pressed
    //oCmdUpPitch = 0;
    static int last_cmd_up = 0;
    if(!(READ_SW3()))
    {
        last_cmd_up = 1;
        oPitchMode = PITCH_MODE_MANUAL;
        oCmdUpPitch = 1;
        //LED_DEBUG4_ON();
    }
    else if(last_cmd_up)
    {
        last_cmd_up = 0;
        oCmdUpPitch = 0;
    }
}