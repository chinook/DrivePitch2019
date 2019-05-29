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
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateInit
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pStatePitch == &StateInit)
  {
    if (INIT_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;        // Idle state
    }
    else
    {
      pStatePitch = &StateInit;        // Go to Error state by default
    }
  }
 
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateIdlePitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pStatePitch == &StateIdlePitch)
  {
    if (IDLE_2_BRAKE)
    {
      pStatePitch = &StateBrakePitch;     // AeroBrake state
    }
    else if (IDLE_2_DATA)
    {
      pStatePitch = &StateSendDataPitch;  // SendData state
    }
    else if(IDLE_2_MOTION)
    {
      pStatePitch = &StateMotorMotion;    // Motion state
    }   
    else if(IDLE_2_REG)
    {
      pStatePitch = &StateRegPitch;       // Regulator state
    }
    else if (IDLE_2_ACQ)
    {
      pStatePitch = &StateAcq;       // Acquisition state
    }
//    else if (IDLE_2_DATA)
//    {
//      pStatePitch = &StateSendDataPitch;  // SendData state
//    }
//    else if (IDLE_2_CALIB)
//   {
//      pStatePitch = &StateCalibPitch;     // Calib state
//    }
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
    if (ACQ_2_BRAKE)
    {
      pStatePitch = &StateBrakePitch;     // AeroBrake state
    }  
    else if (ACQ_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;      // Idle state
    }
    else
    {
      pStatePitch = &StateAcq;       // Go to Error state by default
    }
  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateCalibPitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateCalibPitch)
  {
    if (CALIB_2_SEND)
    {
      pStatePitch = &StateSendDataPitch;   // Send Data state
    }
    else if (CALIB_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;      // Idle state
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
    if (MOTION_2_ACQ)
    {
      pStatePitch = &StateAcq;       // Acquisition state
    }
    else if (MOTION_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;      // Idle state
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
    if (DATA_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;        // Idle state
    }
    else
    {
      pStatePitch = &StateSendDataPitch;    // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateBrakePitch
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateBrakePitch)
  {
    if (BRAKE_2_ACQ)
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
    if (REG_2_IDLE)
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
    if (ERROR_2_IDLE)
    {
      pStatePitch = &StateInit;       // Initialization state
    }
//    else if (ERROR_2_STOP)
//    {
//      pStatePitch = &StateStopPitch;       // Stop state
//    }
//    else if (ERROR_2_CALIB)
//    {
//      pStatePitch = &StateCalibPitch;      // Calibration state
//    }
//    else if (ERROR_2_UP)
//    {
//      pStatePitch = &StateUpPitch;         // Up state
//    }
//    else if (ERROR_2_BRAKE)
//    {
//      pStatePitch = &StateBrakePitch;      // Aero brake state
//    }
    else
    {
      pStatePitch = &StateError;      // Go to Error state by default
    }
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
  INIT_PORTS;
//  INIT_SPI;
  INIT_TIMER;
//  INIT_INPUT_CAPTURE;
  INIT_PWM;
//  INIT_ADC;
//  INIT_UART;
//  INIT_SKADI;
  INIT_CAN;
//  INIT_I2C;
//  INIT_WDT;
  START_INTERRUPTS;
  
 
   firstRun = 1 ;
   oPitchMode =1 ; 
   oFlagAcq=1;
}

//===============================================================
// Name     : State1
// Purpose  : TBD.
//===============================================================
void StateAcq(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // VARIABLE DECLARATIONS
  
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */
    
    
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // FIRST PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

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

  
  ///////////////
 
  
  
  
  
  
  //oFlagAcq  = 0 ; 
  
  ///////////
}

//===============================================================
// Name     : State2
// Purpose  : TBD.
//===============================================================
void State2(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // VARIABLE DECLARATIONS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */
    
    
    
    
    

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // FIRST PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

}

//===============================================================
// Name     : StateError
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateError(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // VARIABLE DECLARATIONS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // FIRST PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // SECOND PART OF STATE
  // Developper should add a small description of expected behavior
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  /*
   * DEVELOPPER CODE HERE
   */

}

void StateMotorMotion(void)
{
    
      int n = 0 ; 
      for ( n=0; n < 9000; n++ )
      {
      
      }
    
    
    
    if ( oPitchMode){
  

        if (oCmdUpPitch )
        {

        LED_DEBUG0_ON;
        oneStep(FWD,QRTRSTEP);
       
         }else{

         LED_DEBUG0_OFF;
      
         }


     if (oCmdDownPitch )
    {
        LED_DEBUG1_ON;
        oneStep(BWD,QRTRSTEP);
     }  else 
        {
         LED_DEBUG1_OFF;
        }  
    }else
        
        
         {
        
        if(!oPitchMode){
                
        if (oCmdUpPitch  )
          {
            
            while ( pitchValue<=setPitch){
                    for ( n=0; n < 20000; n++ ) // 200000 pour le 300
                    {}
    
            LED_DEBUG1_TOGGLE;
            oneStep(FWD,FULSTEP);
            // pitchValue= pitchValue+0.01;
            }
            oCmdUpPitch=0;  
            oPitchDone =1; 
           // SEND_PITCH_DONE;
          }
        
         if (oCmdDownPitch  )
             
            while ( pitchValue>=setPitch){
                for ( n=0; n < 20000; n++ )
                    {}
            
            oneStep(BWD,FULSTEP);
           // pitchValue= pitchValue-0.01;
            }
            oCmdDownPitch = 0 ; 
            oPitchDone =1 ; 
            //SEND_PITCH_DONE;
          }
    }
    
    oFlagAcq =1;
    SEND_PITCH_DONE;
}

void StateCalibPitch(void)
{  
  SetZeroPitch();
}


void StateIdlePitch(void){}

void StateBrakePitch(void)
{

    while ( pitchValue >10)
    {
        
        
        int n = 0 ; 
      for ( n=0; n < 100000; n++ )
      {
      
      }
    
        if ( pitchValue>=180)
        {
        oneStep(FWD,FULSTEP);
        }else
        {
        oneStep(BWD,FULSTEP);
        
        } 
    }



}

//void StateAcqPitch(void){}

void StateSendDataPitch(void){}

void StateRegPitch(void){}

void setUpPitch(void)
{
    
    int n;
    
        
        for (n=0;n<200000;n++){}
        LED_DEBUG1_TOGGLE;
      oneStep(FWD,QRTRSTEP);
    
}

void setDownPitch(void){
    int j;
   while ( pitchValue>setPitch ){
        
        for (j=0;j<200000;j++){}
        
      oneStep(BWD,QRTRSTEP);
    }

}