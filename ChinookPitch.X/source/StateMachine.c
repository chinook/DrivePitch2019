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
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state == StateInit
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pStatePitch == &StateInit)
  {
    pStatePitch = &StateIdlePitch;        // Idle state
  }
 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateIdlePitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pStatePitch == &StateIdlePitch)
  {
    if (oCmdBrake)
    {
      pStatePitch = &StateBrakePitch;     // AeroBrake state
    }
    else if (oSendPitchValue)
    {
      pStatePitch = &StateSendDataPitch;  // SendData state
    }
    else if((oCmdDownPitch || oCmdUpPitch || (!IsPitchDone())))
    {
      pStatePitch = &StateMotorMotion;    // Motion state
    }   
    else if(oMaintainPitch)
    {
      pStatePitch = &StateRegPitch;       // Regulator state
    }
    else if (oFlagAcq)
    {
      pStatePitch = &StateAcq;       // Acquisition state
    }
    else
    {
      pStatePitch = &StateIdlePitch;      // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateAcquisition
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateAcq)
  {
    if (oCmdBrake)
    {
      pStatePitch = &StateBrakePitch;     // AeroBrake state
    }  
    else
    {
      pStatePitch = &StateIdlePitch;      // Idle state
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateCalibPitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateCalibPitch)
  {
    if (oCmdCalib)
    {
      pStatePitch = &StateSendDataPitch;   // Send Data state
    }
    else
    {
      pStatePitch = &StateCalibPitch;     // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateMotorMotion
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateMotorMotion)
  {
    if (oFlagAcq)
    {
      pStatePitch = &StateAcq;       // Acquisition state
    }
    else
    {
      pStatePitch = &StateMotorMotion;    // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateSendDataPitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateSendDataPitch)
  {
    pStatePitch = &StateIdlePitch;        // Idle state
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateBrakePitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateBrakePitch)
  {
    if (oFlagAcq)
    {
      pStatePitch = &StateAcq;       // Brake Aero state
    }
    else
    {
      pStatePitch = &StateBrakePitch;     // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateBrakePitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateRegPitch)
  {
    if (!oMaintainPitch)
    {
      pStatePitch = &StateAcq;       // Idle state
    }
    else if(oFlagAcq)
    {
      pStatePitch = &StateAcq;       // Acq state  
    }
    else
    {
      pStatePitch = &StateRegPitch;      // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateError)
  {
      pStatePitch = &StateInit;       // Initialization state
  }
  
//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//   Current state = undetermined
//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else
  {
    pStatePitch = &StateError;       // Go to Error state by default
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
    oPitchMode = 1; 
    oFlagAcq = 1;
}

//===============================================================
// Name     : State1
// Purpose  : TBD.
//===============================================================
//
// TODO : Re-evaluate if still necessary !!
//
void StateAcq(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */
  sUartLineBuffer_t  uart1Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ,uart2Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ,uart3Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ,uart4Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ,uart5Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ,uart6Data =  {
                                    .buffer = {0}
                                   ,.length =  0
                                  }
                    ;
  
  INT32 err = 0;
  
//  Skadi.GetCmdMsg();    // Use if you do not use UART interrupts
  
  if (Uart.Var.oIsRxDataAvailable[UART1])                 // Check if RX interrupt occured
  {
      Skadi.GetCmdMsgFifo();                              // Use this line if you use UART interrupts and update the UART used
  }
  
  if (Uart.Var.oIsRxDataAvailable[UART2])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART2, &uart2Data, FALSE); // put received data in uart2Data
    if (err >= 0)                                         // If no error occured
    {
      /* Do something */
      Uart.PutTxFifoBuffer(UART2, &uart2Data);            // Put data received in TX FIFO buffer
    }
  }
  
  if (Uart.Var.oIsRxDataAvailable[UART3])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART3, &uart3Data, FALSE); // put received data in uart3Data
    if (err >= 0)                                         // If no error occured
    {
      /* Do something */
      Uart.PutTxFifoBuffer(UART3, &uart3Data);            // Put data received in TX FIFO buffer
    }
  }
  
  if (Uart.Var.oIsRxDataAvailable[UART4])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART4, &uart4Data, FALSE); // put received data in uart4Data
    if (err >= 0)                                         // If no error occured
    {
      /* Do something */
      Uart.PutTxFifoBuffer(UART4, &uart4Data);            // Put data received in TX FIFO buffer
    }
  }
  
  if (Uart.Var.oIsRxDataAvailable[UART5])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART5, &uart5Data, FALSE); // put received data in uart5Data
    if (err >= 0)                                         // If no error occured
    {
      /* Do something */
      Uart.PutTxFifoBuffer(UART5, &uart5Data);            // Put data received in TX FIFO buffer
    }
  }
  
  if (Uart.Var.oIsRxDataAvailable[UART6])                 // Check if RX interrupt occured
  {
    err = Uart.GetRxFifoBuffer(UART6, &uart6Data, FALSE); // put received data in uart6Data
    if (err >= 0)                                         // If no error occured
    {
      /* Do something */
      Uart.PutTxFifoBuffer(UART6, &uart6Data);            // Put data received in TX FIFO buffer
    }
  }
}

//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
    
}

void StateMotorMotion(void)
{
    int n = 0;
    for(n = 0; n < 50000; ++n);
    
    if(oPitchMode == PITCH_MODE_MANUAL)
    {
        if (oCmdUpPitch)
        {
            LED_DEBUG0_ON();
            oneStep(FWD, QRTRSTEP);
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
            oneStep(BWD, QRTRSTEP);
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
        int n; 
        for (n = 0; n < 10000; n++);
        
        int direction;
        if(current_pitch < target_pitch)
            direction = FWD;
        else
            direction = BWD;
        
        while(current_pitch != target_pitch)
        {
            if(bROPS)
                break;
            // Quarter step in the direction
            oneStep(direction, FULSTEP);
            
            
            // Check if we're done
            float delta = target_pitch - current_pitch;
            if(abs(delta) <= 1.01f)
            {
                oPitchDone = 1;
                // Send pitch done CAN message
                Can.SendByte(CAN1, 0x35, 1);
                break;
            }
            
            
            for(n = 0; n < 200000; ++n);
        }
    }
}

// TODO : Re-evaluate if necessary
void StateCalibPitch(void)
{  
  //SetZeroPitch();
}

void StateIdlePitch(void)
{
    // Assess buttons
    // Forward stepping if SW2 is pressed
    oCmdDownPitch = 0;
    if (!(READ_SW2()))
    {
        oPitchMode = PITCH_MODE_MANUAL;
        oCmdDownPitch = 1;
        LED_DEBUG4_ON();
    }

    // Backwards stepping if SW3 is pressed
    oCmdUpPitch = 0;
    if(!(READ_SW3()))
    {
        oPitchMode = PITCH_MODE_MANUAL;
        oCmdUpPitch = 1;
        LED_DEBUG4_ON();
    }
}

// TODO : Check if this state is still relevant
// Probably change to simply un-torque the stepper
void StateBrakePitch(void)
{
    // Enlever le holding torque du stepper !
    
}

//void StateAcqPitch(void){}

void StateSendDataPitch(void){}

void StateRegPitch(void){}