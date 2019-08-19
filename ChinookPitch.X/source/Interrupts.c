//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Interrupts.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file declares all interrupt subroutines used.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : All priorities must be set and verified by the developper. All
//           priorities and subpriorities used are there as example. Also, note
//           that interrupt can be defined as iplnauto, iplnsrs and iplnsoft,
//           where n can be from 0 to 7. 7 is the highest priority, while 1 is
//           the lowest. A priority of 0 will disable the interrupt.
//           Subpriorities can go from (lowest to highest) 0 to 3.
//           SRS mode uses the shadow registers, soft uses the software
//           registers and auto is an automatic assignment.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include "Interrupts.h"
#include "StateMachine.h"
#include "Setup.h"
#include "ChinookSteeringWheelUtils.h"
#include "dual_driver_vnh5019.h"


extern volatile sButtonStates_t buttons;

volatile BOOL oTimer1 = 0;



/*******************************************************************************
 ***********************                               *************************
 ********************           TIMER INTERRUPTS          **********************
 ***********************                               *************************
 ******************************************************************************/

//=============================================
// Configure the Timer 1 interrupt handler
//=============================================
void __ISR(_TIMER_1_VECTOR, T1_INTERRUPT_PRIORITY) Timer1InterruptHandler(void)
{
  oTimer1 = 1;
  
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[0]++;
  
  mT1ClearIntFlag();
}

//=============================================
// Configure the Timer 2 interrupt handler
//=============================================
void __ISR(_TIMER_2_VECTOR, T2_INTERRUPT_PRIORITY) Timer2InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */
    
  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[1]++;
  
  mT2ClearIntFlag();
}

//=============================================
// Configure the Timer 3 interrupt handler
//=============================================
void __ISR(_TIMER_3_VECTOR, T3_INTERRUPT_PRIORITY) Timer3InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[2]++;

  mT3ClearIntFlag();
}

//=============================================
// Configure the Timer 4 interrupt handler
//=============================================
void __ISR(_TIMER_4_VECTOR, T4_INTERRUPT_PRIORITY) Timer4InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[3]++;

  mT4ClearIntFlag();
}

//=============================================
// Configure the Timer 5 interrupt handler
//=============================================
void __ISR(_TIMER_5_VECTOR, T5_INTERRUPT_PRIORITY) Timer5InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Increment the number of overflows from this timer. Used primarily by Input Capture
  Timer.Var.nOverflows[4]++;

  mT5ClearIntFlag();
}


/*******************************************************************************
 ***********************                               *************************
 ********************           UART INTERRUPTS           **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the UART 6 interrupt handler
//=============================================

void __ISR(_UART_6_VECTOR, U6_INTERRUPT_PRIORITY) Uart6InterruptHandler(void)
{
  UINT8 i,
        iMax,   // Read/write max 8 bytes/interrupt
        data;   // used in UartFifoWrite/Read functions

  //===========================================================         
  // TX interrupt handling
  //===========================================================
  if (INTGetEnable(INT_SOURCE_UART_TX(UART6)))               // If TX interrupts enabled
  {
    if (INTGetFlag(INT_SOURCE_UART_TX(UART6)))               // If TX interrupt occured
    {
      if (UARTTransmitterIsReady(UART6) && !Uart.Var.uartTxFifo[UART6].bufEmpty)  // If TX buffer is ready to receive data and the user's TX buffer is not empty
      {
        if (Uart.Var.uartTxFifo[UART6].lineBuffer.length < 8)     // Write max 8 bytes/interrupt
        {
          iMax = Uart.Var.uartTxFifo[UART6].lineBuffer.length;
        }
        else
        {
          iMax = 8;
        }

        for (i = 0; i < iMax; i++)
        {
          UartFifoRead((void *) &Uart.Var.uartTxFifo[UART6], &data);  // Copy from user
          U6TXREG = data;                                         // Put data in PIC32's TX buffer
        }
      }

      if (Uart.Var.uartTxFifo[UART6].bufEmpty)                    // If User's TX buffer is empty
      {
        Uart.DisableTxInterrupts(UART6);                          // Disable TX interrupts
      }

      INTClearFlag(INT_SOURCE_UART_TX(UART6));                    // Clear the TX interrupt Flag
    }
  }
  
  //===========================================================
  // RX interrupt handling
  //===========================================================
  if (INTGetEnable(INT_SOURCE_UART_RX(UART6)))                // If RX interrupts enabled
  {
    if (INTGetFlag(INT_SOURCE_UART_RX(UART6)))               // If RX interrupt occured
    {
      i = 0;
      iMax = 8;                                                   // Read max 8 bytes/interrupt
      while (UARTReceivedDataIsAvailable(UART6) &&                // While RX data available
            !Uart.Var.uartRxFifo[UART6].bufFull &&                // and user's RX buffer not full
            (i < iMax))                                           // and under 8 bytes read
      {
        data = UARTGetDataByte(UART6);                            // Get data for PIC32's RX FIFO buffer and copy it to user (next line)
        if (UartFifoWrite((void*)&Uart.Var.uartRxFifo[UART6], &data) < 0) // If copy to user did not work
        {
          break;                                                  // Exit while loop
        }
        i++;
      }

      if (!Uart.Var.uartRxFifo[UART6].bufEmpty)                   // If there is data in the user's RX buffer
      {
        Uart.Var.oIsRxDataAvailable[UART6] = 1;                   // Set according flag
      }

      INTClearFlag (INT_SOURCE_UART_RX(UART6) );                  // Clear the RX interrupt Flag

    }
  }
  //===========================================================
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************           ADC INTERRUPT             **********************
 ***********************                               *************************
 *******************************************************************************/

//=============================================
// Configure the ADC interrupt handler
//=============================================
void __ISR(_ADC_VECTOR, ADC_INTERRUPT_PRIO) AdcInterruptHandler(void)
{
  Adc.Read();               // Read the enabled channels and puts them in Adc.Var.adcReadValues[]
  INTClearFlag(INT_AD1);    // Clear the ADC conversion done interrupt Flag
}
//=============================================


/*******************************************************************************
 ***********************                               *************************
 ********************      INPUT CAPTURE INTERRUPTS       **********************
 ***********************                               *************************
 ******************************************************************************/

//================================================
// Configure the InputCapture 1 interrupt handler
//================================================
void __ISR(_INPUT_CAPTURE_1_VECTOR, IC1_INT_PRIORITY) InputCapture1InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Get the timer used by this Input Capture
  TimerNum_t numTimer = InputCapture.Var.timerUsed[IC1];

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC1)));

  // Update values of timer overflows
  InputCapture.Var.previousTimerOverflows[IC1] = InputCapture.Var.currentTimerOverflows[IC1];
  InputCapture.Var.currentTimerOverflows[IC1]  = Timer.ReadOverflows(numTimer);

  // Store the timer value of the capture event
  InputCapture.Var.previousCaptureCountValue[IC1] = InputCapture.Var.currentCaptureCountValue[IC1];
  InputCapture.Var.currentCaptureCountValue [IC1] = InputCapture.ReadCapture(IC1);

//  oCapture = 1;   // Flag that tells that a new Capture event occured

  // Clear the interrupt flag
  INTClearFlag(INT_IC1);
}
//================================================

//================================================
// Configure the InputCapture 2 interrupt handler
//================================================
void __ISR(_INPUT_CAPTURE_2_VECTOR, IC2_INT_PRIORITY) InputCapture2InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Get the timer used by this Input Capture
  TimerNum_t numTimer = InputCapture.Var.timerUsed[IC2];

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC2)));

  // Update values of timer overflows
  InputCapture.Var.previousTimerOverflows[IC2] = InputCapture.Var.currentTimerOverflows[IC2];
  InputCapture.Var.currentTimerOverflows[IC2]  = Timer.ReadOverflows(numTimer);

  // Store the timer value of the capture event
  InputCapture.Var.previousCaptureCountValue[IC2] = InputCapture.Var.currentCaptureCountValue[IC2];
  InputCapture.Var.currentCaptureCountValue [IC2] = InputCapture.ReadCapture(IC2);

//  oCapture = 1;   // Flag that tells that a new Capture event occured

  // Clear the interrupt flag
  INTClearFlag(INT_IC2);
}
//================================================

//================================================
// Configure the InputCapture 3 interrupt handler
//================================================
void __ISR(_INPUT_CAPTURE_3_VECTOR, IC3_INT_PRIORITY) InputCapture3InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Get the timer used by this Input Capture
  TimerNum_t numTimer = InputCapture.Var.timerUsed[IC3];

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC3)));

  // Update values of timer overflows
  InputCapture.Var.previousTimerOverflows[IC3] = InputCapture.Var.currentTimerOverflows[IC3];
  InputCapture.Var.currentTimerOverflows[IC3]  = Timer.ReadOverflows(numTimer);

  // Store the timer value of the capture event
  InputCapture.Var.previousCaptureCountValue[IC3] = InputCapture.Var.currentCaptureCountValue[IC3];
  InputCapture.Var.currentCaptureCountValue [IC3] = InputCapture.ReadCapture(IC3);

//  oCapture = 1;   // Flag that tells that a new Capture event occured

  // Clear the interrupt flag
  INTClearFlag(INT_IC3);
}
//================================================

//================================================
// Configure the InputCapture 4 interrupt handler
//================================================
void __ISR(_INPUT_CAPTURE_4_VECTOR, IC4_INT_PRIORITY) InputCapture4InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Get the timer used by this Input Capture
  TimerNum_t numTimer = InputCapture.Var.timerUsed[IC4];

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC4)));

  // Update values of timer overflows
  InputCapture.Var.previousTimerOverflows[IC4] = InputCapture.Var.currentTimerOverflows[IC4];
  InputCapture.Var.currentTimerOverflows[IC4]  = Timer.ReadOverflows(numTimer);

  // Store the timer value of the capture event
  InputCapture.Var.previousCaptureCountValue[IC4] = InputCapture.Var.currentCaptureCountValue[IC4];
  InputCapture.Var.currentCaptureCountValue [IC4] = InputCapture.ReadCapture(IC4);

//  oCapture = 1;   // Flag that tells that a new Capture event occured

  // Clear the interrupt flag
  INTClearFlag(INT_IC4);
}
//================================================

//================================================
// Configure the InputCapture 5 interrupt handler
//================================================
void __ISR(_INPUT_CAPTURE_5_VECTOR, IC5_INT_PRIORITY) InputCapture5InterruptHandler(void)
{
  /*
   * DEVELOPPER CODE HERE
   */

  // Get the timer used by this Input Capture
  TimerNum_t numTimer = InputCapture.Var.timerUsed[IC5];

  // Wait for capture data to be ready
  while(!(InputCapture.IsCaptureReady(IC5)));

  // Update values of timer overflows
  InputCapture.Var.previousTimerOverflows[IC5] = InputCapture.Var.currentTimerOverflows[IC5];
  InputCapture.Var.currentTimerOverflows[IC5]  = Timer.ReadOverflows(numTimer);

  // Store the timer value of the capture event
  InputCapture.Var.previousCaptureCountValue[IC5] = InputCapture.Var.currentCaptureCountValue[IC5];
  InputCapture.Var.currentCaptureCountValue [IC5] = InputCapture.ReadCapture(IC5);

//  oCapture5 = 1;   // Flag that tells that a new Capture event occured

  // Clear the interrupt flag
  INTClearFlag(INT_IC5);
}
//================================================


/*******************************************************************************
 ***********************                               *************************
 ********************           CAN INTERRUPTS            **********************
 ***********************                               *************************
 ******************************************************************************/

//================================================
// Configure the CAN1 interrupt handler
//================================================

void __ISR(_CAN_1_VECTOR, CAN1_INT_PRIORITY) Can1InterruptHandler(void)
{
  // Check if the source of the interrupt is RX_EVENT. This is redundant since
  // only this event is enabled in this example but this shows one scheme for
  // handling events
 
  if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0) {

    // Within this, you can check which channel caused the event by using
    // the CANGetModuleEvent() function which returns a code representing
    // the highest priority pending event.
    CANRxMessageBuffer *message;
    
    // This means that channel 1 caused the event.
    // The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You could either
    // read the channel in the ISR to clear the event condition or as done
    // here, disable the event source, and set an application flag to
    // indicate that a message has been received. The event can be
    // enabled by the application when it has processed one message.
    // Note that leaving the event enabled would cause the CPU to keep
    // executing the ISR since the CAN_RX_CHANNEL_NOT_EMPTY event is
    // persistent (unless the not empty condition is cleared.)
    if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL1_EVENT) 
    {
        // This event fires when the steering wheel buttons
        // are pressed (Pitch up, Pitch down buttons)
        
        CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
        
        message = CANGetRxMessage(CAN1, CAN_CHANNEL1);
        
        LED_CAN_ON();
        
        canVolant((message->data[1]<<8)|message->data[0]);
      
        CANUpdateChannel(CAN1, CAN_CHANNEL1);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    } 
    else if(CANGetPendingEventCode(CAN1) == CAN_CHANNEL2_EVENT) 
    {
        // This event toggles the pitch mode
        // between automatic and manual
        
        CANEnableChannelEvent(CAN1, CAN_CHANNEL2, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);

        message = CANGetRxMessage(CAN1, CAN_CHANNEL2);
        int mode = *(int*)&message->data[0];
        // Mode 0 = manual
        // Mode 1 = automatic
        if(mode == 0)
        {
            oPitchMode = PITCH_MODE_MANUAL;
            //LED_DEBUG4_ON();
        }
        else if(mode == 1)
        {
            oPitchMode = PITCH_MODE_AUTOMATIC;
            //LED_DEBUG4_OFF();
        }
        
        SEND_PITCH_MODE;

        CANUpdateChannel(CAN1, CAN_CHANNEL2);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL2, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
    else if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL3_EVENT)
    {
        // Channel pour recevoir le nbr de steps a effectuer
        CANEnableChannelEvent(CAN1, CAN_CHANNEL3, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
        
        message = CANGetRxMessage(CAN1, CAN_CHANNEL3);
        float nbPitch = *(float*)&message->data[0];
        
        // Update the pitch target value
        target_pitch = current_pitch + nbPitch;
        LED_CAN_ON();
        
        Can.SendData(CAN1, 0x35, 0);
        
        CANUpdateChannel(CAN1, CAN_CHANNEL3);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL3, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
    else if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL4_EVENT)
    {
        // This event reads the Turbine RPM value from the CAN BUS
        // If it detects it is higher than the max threshold, it fires the ROPS
        // auto-stall break
        
        CANEnableChannelEvent(CAN1, CAN_CHANNEL4, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);

        message = CANGetRxMessage(CAN1, CAN_CHANNEL4);
        LED_CAN_ON();
        
        int x = *(int*)&message->data[0];
        rops_steps = *(float*)&message->data[4];
        drapeau_pitch = current_pitch + rops_steps;
        
        // Do we fire the ROPS ?
        bROPS = x;
      
        CANUpdateChannel(CAN1, CAN_CHANNEL4);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL4, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
    else if(CANGetPendingEventCode(CAN1) == CAN_CHANNEL5_EVENT)
    {
        CANEnableChannelEvent(CAN1, CAN_CHANNEL5, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
        
        message = CANGetRxMessage(CAN1, CAN_CHANNEL5);
        LED_CAN_ON();
        
        if(!bROPS)
            target_pitch = current_pitch;
        
        CANUpdateChannel(CAN1, CAN_CHANNEL4);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL5, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
  }

  // The CAN1 Interrupt flag is  cleared at the end of the interrupt routine.
  // This is because the event source that could have caused this interrupt to
  // occur (CAN_RX_CHANNEL_NOT_EMPTY) is disabled. Attempting to clear the
  // CAN1 interrupt flag when the the CAN_RX_CHANNEL_NOT_EMPTY interrupt is
  // enabled will not have any effect because the base event is still present.
  INTClearFlag(INT_CAN1);
}

void canVolant(UINT16 canMessage)
{
    // If we are not in manual mode, discard this command
    //if(oPitchMode != PITCH_MODE_MANUAL)
    //    return;
    
    // Bouton de gauche
    if (oPitchMode == PITCH_MODE_MANUAL && ((canMessage & PITCH_MINUS_BUTTON) == PITCH_MINUS_BUTTON))// && !oPitchDone)
    {
        //target_pitch = current_pitch - 10;//PITCH_RESOLUTION;
        LED_DEBUG2_ON();
        oCmdDownPitch = 1;
    }
    else 
    {
        LED_DEBUG2_OFF();
        LED_DEBUG0_OFF();
        oCmdDownPitch = 0;
    }
        
    // Bouton de droite
    if (oPitchMode == PITCH_MODE_MANUAL && ((canMessage & PITCH_PLUS_BUTTON) == PITCH_PLUS_BUTTON))// && !oPitchDone)
    {
        //target_pitch = current_pitch + 10;//PITCH_RESOLUTION;
        //LED_DEBUG3_ON(); 
        oCmdUpPitch = 1;
    } 
    else
    {
        //LED_DEBUG3_OFF();
        LED_DEBUG0_OFF();
        oCmdUpPitch = 0;
    }
    
    if (oPitchMode == PITCH_MODE_MANUAL && !(((canMessage & PITCH_MINUS_BUTTON) == PITCH_MINUS_BUTTON) && (canMessage & PITCH_PLUS_BUTTON) == PITCH_PLUS_BUTTON))
    {
         oPitchDone = 0;
    }
}


void canPitch(float canMessage) 
{
    pitchValue = canMessage;
}

