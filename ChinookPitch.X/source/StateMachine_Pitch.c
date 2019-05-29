//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// StateMachine_Pitch.c
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine_Pitch.c
// Author  : Gabriel Jean-Louis
// Date    : 2017-07-08
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

#include "..\headers\StateMachine_Pitch.h"

//==============================================================================
//	Variables declaration
//==============================================================================
volatile sCmdValue_t pitchAngle = {0};

volatile BOOL direction = PITCH_UP;

volatile BOOL firstRun = 0 ; 
// Potentiometer variables
sPotValues_t potValues = 
{
  .nSamples         = 0
 ,.potSamples       = 
  {
    .bufEmpty       = 1
   ,.bufFull        = 0
   ,.inIdx          = 0
   ,.lineBuffer     = {0}
   ,.maxBufSize     = 256
   ,.outIdx         = 0
  }
 ,.lastAverage      = 0
 ,.angle            = NULL
 ,.speed            = NULL
  
 ,.potStepSamples   = 
  {
    .bufEmpty       = 1
   ,.bufFull        = 0
   ,.inIdx          = 0
   ,.lineBuffer     = {0}
   ,.maxBufSize     = 256
   ,.outIdx         = 0
  }
 ,.potStepValue     = 0
 ,.lastBits         = 0
    
 ,.zeroInBits       = 0
    
 ,.deadZoneDetect   = 500
    
 ,.stepZero         = 0
};

// Skadi function pointer. Used for loop.
void (*pSkadiFunction)(sSkadi_t *, sSkadiArgs_t);      // Skadi Function pointer, used to loop a skadi function.
sSkadi_t *skadiLoop;
sSkadiArgs_t argsLoop;


//==============================================================================
//	STATES OF STATE MACHINE
//==============================================================================
//===============================================================
// Name     : StateSchedulerPitch
// Purpose  : Decides which state is next depending on current
//            state and flags. Used as a function
//===============================================================
void StateSchedulerPitch(void)
{
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateInit
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (pStatePitch == &StateInitPitch)
  {
    if (INIT_2_IDLE)
    {
      pStatePitch = &StateIdlePitch;        // Idle state
    }
    else
    {
      pStatePitch = &StateInitPitch;        // Go to Error state by default
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
      pStatePitch = &StateAcqPitch;       // Acquisition state
    }
//    else if (IDLE_2_DATA)
//    {
//      pStatePitch = &StateSendDataPitch;  // SendData state
//    }
    else if (IDLE_2_CALIB)
    {
      pStatePitch = &StateCalibPitch;     // Calib state
    }
    else
    {
      pStatePitch = &StateIdlePitch;      // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateAcquisition
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateAcqPitch)
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
      pStatePitch = &StateAcqPitch;       // Go to Error state by default
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
      pStatePitch = &StateAcqPitch;       // Acquisition state
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
      pStatePitch = &StateAcqPitch;       // Brake Aero state
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
      pStatePitch = &StateAcqPitch;       // Idle state
    }
    else if(oFlagAcq)
    {
      pStatePitch = &StateAcqPitch;       // Acq state  
    }
    else
    {
      pStatePitch = &StateRegPitch;      // Go to Error state by default
    }
  }
  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Current state = StateError
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else if (pStatePitch == &StateErrorPitch)
  {
    if (ERROR_2_IDLE)
    {
      pStatePitch = &StateInitPitch;       // Initialization state
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
      pStatePitch = &StateErrorPitch;      // Go to Error state by default
    }
  }
  
//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//   Current state = undetermined
//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else
  {
    pStatePitch = &StateErrorPitch;       // Go to Error state by default
  }
}

//===============================================================
// Name     : StateInitPitch
// Purpose  : Initialization of the system.
//===============================================================
void StateInitPitch(void)
{
  INIT_PORTS;
#ifdef USE_WDT
  INIT_WDT;
#endif
  INIT_TIMER;
#ifdef USE_ADC
  INIT_ADC;
#endif
#ifdef USE_UART
  INIT_UART;
#endif
#ifdef USE_SPI
  INIT_SPI;
#endif
#ifdef USE_PWM
  INIT_PWM;
#endif
#ifdef USE_I2C
  INIT_I2C;
#endif
#ifdef USE_SKADI
  INIT_SKADI;
#endif
  INIT_CAN;
  
  LED_STATUS  = 1;
  LED_CAN     = 1;
  LED_ERROR   = 1;
  LED_DEBG4   = 1;
  LED_DEBG3   = 1;
  LED_DEBG2   = 1;
  LED_DEBG1   = 1;
  LED_DEBG0   = 1;
  
  // Set default drive used.
  USE_DRIVE_A = 0;
  USE_DRIVE_B = 1;
  
  InitRegPitch();
  
  if (USE_DRIVE_A)
  {
    DRVA_RESET = 0;
    DRVA_SLEEP = 1;
    
    WriteDrive(DRVA, CONTROL_PITCHw);
    WriteDrive(DRVA, TORQUE_PITCHw);
    WriteDrive(DRVA, BLANK_PITCHw);
    WriteDrive(DRVA, DECAY_PITCHw);
    WriteDrive(DRVA, STATUS_PITCHw);
    DRVA_SLEEP = 0;
  }
  else if (USE_DRIVE_B)
  {
    DRVB_RESET = 0;
    DRVB_SLEEP = 1;
    
    WriteDrive(DRVB, CONTROL_PITCHw);
    WriteDrive(DRVB, TORQUE_PITCHw);
    WriteDrive(DRVB, BLANK_PITCHw);
    WriteDrive(DRVB, DECAY_PITCHw);
    WriteDrive(DRVB, STATUS_PITCHw);
    DRVB_SLEEP = 0;
  }
  else 
  {
    // Init Drive A
    DRVA_RESET = 0;
    DRVA_SLEEP = 1;
    
    WriteDrive(DRVA, CONTROL_PITCHw);
    WriteDrive(DRVA, TORQUE_PITCHw);
    WriteDrive(DRVA, BLANK_PITCHw);
    WriteDrive(DRVA, DECAY_PITCHw);
    WriteDrive(DRVA, STATUS_PITCHw);
    DRVA_SLEEP = 0;
    
    // Init Drive B
    DRVB_RESET = 0;
    DRVB_SLEEP = 1;
    
    WriteDrive(DRVB, CONTROL_PITCHw);
    WriteDrive(DRVB, TORQUE_PITCHw);
    WriteDrive(DRVB, BLANK_PITCHw);
    WriteDrive(DRVB, DECAY_PITCHw);
    WriteDrive(DRVB, STATUS_PITCHw);
    DRVB_SLEEP = 0;
  }
  
  PotAddFirstSample(&potValues);
  
  potValues.angle = &pitchAngle;
  
  oCmdUpPitch = 0;
  oCmdDownPitch = 0;
  oFlagAcq = 0;
  oCmdBrake = 0;
  oCmdCalib = 0;
  oAdcReady = 0;
  oMotorStop = 1;
  debugEn = 1;
  LIMIT_SW_HI = 0;
  LIMIT_SW_LOW = 0;
  
  SEND_ID_TO_BACKPLANE;
  
  START_INTERRUPTS;
  
  // Get last known position of the pitch
//  ReadPitchPosFromEeprom();
//  if (AbsFloat(pitchAngle.currentValue) > 360)  // Error
//  {
//    pitchAngle.previousValue = 0;
//    pitchAngle.currentValue  = 0;
//  }
//  
//  if (potValues.lastAverage > ADC_TOTAL_BITS)
//  {
//    potValues.lastAverage = ADC_TOTAL_BITS << 1;
//  }
//  if (potValues.zeroInBits > ADC_TOTAL_BITS)
//  {
//    potValues.zeroInBits = ADC_TOTAL_BITS << 1;
//  }
//  if (potValues.potStepValue > POT_TO_MOTOR_RATIO)
//  {
//    potValues.potStepValue = POT_TO_MOTOR_RATIO << 1;
//  }
//  if (potValues.lastBits > (ADC_BITS_PER_REVOLUTION - 1))
//  {
//    potValues.lastBits = 0;
//  }
  
  Pitch.pos = 0;
  Pitch.posMaintain = 0;
  Pitch.error = 1;
  firstRun = 1;
}

//===============================================================
// Name     : StateIdlePitch
// Purpose  : Reacts to data gathered by StateAcq.
//===============================================================
void StateIdlePitch(void)
{
  sUartLineBuffer_t buffer;
  
  UINT16 i = 0;
  
  LED_DEBG0 = 1;
  LED_DEBG2 = 0;
  LED_DEBG4 = 1;
  LED_STATUS = 1;
  
  // <editor-fold defaultstate="collapsed" desc="Stop and Sleep Drive">
  if (MOTION_2_IDLE && !oMotorStop)
  {
    if (DEBUG)
    {
      buffer.length = sprintf(buffer.buffer,"MotionStop\n\r Pot Pos is: %d\n\r", potValues.lastAverage);
      Uart.PutTxFifoBuffer(UART6, &buffer);
    }
    
    oCmdUpPitch = 0;
    oCmdDownPitch = 0;
    oPitchDone = 0;
    oMotorStop = 1;
    if(USE_DRIVE_A)
    {
      DRVA_SLEEP = 0;
    }
    else if (USE_DRIVE_B)
    {
      DRVB_SLEEP = 0;
    }
    else
    {
      DRVA_SLEEP = 0;
      DRVB_SLEEP = 0;
    }
  }
} 


//===============================================================
// Name     : StateAcqPitch
// Purpose  : Waits and checks for changes on switches. If there
//            is a turbine overspeed we put the pitch in Brake
//            state. Also checks for messages received from skadi.
//===============================================================
void StateAcqPitch(void)
{
  sUartLineBuffer_t buffer;
    
  UINT16 tempAdcValue   = 0;
  
  UINT32 temp;
  
  oFlagAcq = 0;
  
  LED_DEBG2 = 1;
  LED_STATUS ^= 1;
  
  // <editor-fold defaultstate="collapsed" desc="Get ADC value">
  if (oAdcReady)
  {
    oAdcReady = 0;
    tempAdcValue = Adc.Var.adcReadValues[2];
    
//    if (DEBUG)
//    {
//      buffer.length = sprintf(buffer.buffer,"La valeur de l'ADC est: %d\n\r", tempAdcValue);
//      Uart.PutTxFifoBuffer(UART6, &buffer);
//    }
    
    if (tempAdcValue >= LMT_HI_VALUE)
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Limit High\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      //oFlagSwHi = 1;
      oFlagSwLw  = 1;
      oPitchDone = 1;
      oMotorStop = 0;
    }   
    else if ((tempAdcValue <= LMT_LW_VALUE2)&& tempAdcValue >= LMT_LW_VALUE)
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Limit Low\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }  
      //oFlagSwLw = 1;
      oFlagSwHi  = 1;
      oPitchDone = 1;
      oMotorStop = 0; 
    }
    else 
    {
      oFlagSwHi = 0;
      oFlagSwLw = 0;
      
      PotAddSample(&potValues, tempAdcValue);
      if (potValues.nSamples >= N_SAMPLES_TO_AVERAGE)
      {
        PotAverage(&potValues);
        UpdateAngle(&potValues);
        Pitch.pos = potValues.lastAverage;
        
        if (firstRun)
        {
            Pitch.posMaintain = Pitch.pos;
            firstRun = 0;
            
            if (DEBUG)
            {
              buffer.length = sprintf(buffer.buffer,"First Run\n\r Pos Maintain is: %d \n\r Current pos is: %d\n\r", Pitch.posMaintain, Pitch.pos);
              Uart.PutTxFifoBuffer(UART6, &buffer);
            }
        }
        
        temp = abs(Pitch.posMaintain - Pitch.pos);
        
        if (REG_PITCH && !IDLE_2_MOTION && !ACQ_2_BRAKE)
        {
          oMaintainPitch = 1;
          if (DEBUG && oSendPitchValue)
          {
            buffer.length = sprintf(buffer.buffer,"Maintain is active\n\r Diff value is: %d", temp);
            Uart.PutTxFifoBuffer(UART6, &buffer);
          }
          
          if (USE_DRIVE_A && !DRVA_SLEEP)
          {
            DRVA_SLEEP = 1;
            Timer.DelayMs(1);
          }
          else if (USE_DRIVE_B && !DRVB_SLEEP)
          {
            DRVB_SLEEP = 1;
            Timer.DelayMs(1);
          }
        }
        else
        {
          oMaintainPitch = 0;
         
        }
      }
    }
  }
  // </editor-fold>
  
//   <editor-fold defaultstate="collapsed" desc="Check On Board Switch">
  if (!SW2 ^ !SW3 && (pitchStep == 0))
  {
    if (!SW2)
    {
      oCmdDownPitch = 1;
      
      //secu
      
       oCmdUpPitch = 0;
    }
    else
    {
        
      oCmdUpPitch = 1;
      
      //secu
      
      oCmdDownPitch = 0;
    }
  }
  // </editor-fold>

#ifdef USE_SKADI
  Skadi.GetCmdMsgFifo();
#endif
} 

//===============================================================
// Name     : StateCalibPitch
// Purpose  : First step in a calibration pitch is 
//            to search the minimum with the limit switch
//===============================================================
void StateCalibPitch(void)
{  
  SetZeroPitch();
}

//===============================================================
// Name     : StateSendDataPitch
// Purpose  : Sends pitch position on CAN bus and writes
//            position to EEPROM.
//===============================================================
void StateSendDataPitch(void)
{
  if (oCmdCalib)
  {
    oCmdCalib = 0;
    SEND_CALIB_DONE;
  }
  else
  {
    oSendPitchValue=0;
    SEND_PITCH_ANGLE;
    WritePitchPos2Eeprom();    
  }
  
  if (oLoopSkadi)
  {
    (*pSkadiFunction)(skadiLoop, argsLoop);
  }
}

//===============================================================
// Name     : StateMotorMotion
// Purpose  : Checks what direction to move. Also stops the pitch
//            if we have reached the limits.
//===============================================================
void StateMotorMotion(void)
{
  sUartLineBuffer_t buffer;
  
  // <editor-fold defaultstate="collapsed" desc="Stop motor">
  if (LIMIT_SW_HI || LIMIT_SW_LOW)
  {
    if (PITCH_UP_CMD && LIMIT_SW_HI)
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"LimitSwitch High\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      oMotorStop = 1;
      oPitchDone = 1;
      DRVB_SLEEP = 0;
    }
    else if (PITCH_DOWN_CMD && LIMIT_SW_LOW)
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"LimitSwitch Low\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      oMotorStop = 1;
      oPitchDone = 1;
      DRVB_SLEEP = 0;
    }
    else
    {
      oMotorStop = 0;
      pitchStep += STEP_1DEG;
     
      if (USE_DRIVE_A && !DRVA_SLEEP)
      {
        DRVA_SLEEP = 1;
        Timer.DelayMs(1);
      }
      else if (USE_DRIVE_B && !DRVB_SLEEP)
      {
        DRVB_SLEEP = 1;
        Timer.DelayMs(1);
      }
    }
  }
  else
  {
    if (!oPitchDone && oMotorStop)
    {
      oMotorStop = 0;
      pitchStep += STEP_1DEG;
      
      if (USE_DRIVE_A && !DRVA_SLEEP)
      {
        DRVA_SLEEP = 1;
        Timer.DelayMs(1);
      }
      else if (USE_DRIVE_B && !DRVB_SLEEP)
      {
        DRVB_SLEEP = 1;
        Timer.DelayMs(1);
      }
    }
  }
  // </editor-fold>
  
  // <editor-fold defaultstate="collapsed" desc="Change drive direction">
  if (USE_DRIVE_A)
  {
    if (PITCH_UP_CMD && (direction != PITCH_UP))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Up\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_UP;
      DRVA_DIR = PITCH_UP;
    }
    else if (PITCH_DOWN_CMD && (direction != PITCH_DOWN))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch DOWN\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_DOWN;
      DRVA_DIR = PITCH_DOWN;
    }
  }
  else if (USE_DRIVE_B)
  {
    if (PITCH_UP_CMD && (direction != PITCH_UP))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Up\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_UP;
      DRVB_DIR = PITCH_UP;
    }
    else if (PITCH_DOWN_CMD && (direction != PITCH_DOWN))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Down\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_DOWN;
      DRVB_DIR = PITCH_DOWN;
    }
  }
  else
  {
    if (PITCH_UP_CMD && (direction != PITCH_UP))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Up Drive A\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_UP;
      DRVA_DIR = PITCH_UP;
    }
    else if (PITCH_DOWN_CMD && (direction != PITCH_DOWN))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Down Drive A\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_DOWN;
      DRVA_DIR = PITCH_DOWN;
    }
    
    if (PITCH_UP_CMD && (direction != PITCH_UP))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Up Drive B\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_UP;
      DRVB_DIR = PITCH_UP;
    }
    else if (PITCH_DOWN_CMD && (direction != PITCH_DOWN))
    {
      if (DEBUG)
      {
        buffer.length = sprintf(buffer.buffer,"Pitch Down Drive B\n\r");
        Uart.PutTxFifoBuffer(UART6, &buffer);
      }
      pitchStep += BACKLASH;
      direction = PITCH_DOWN;
      DRVB_DIR = PITCH_DOWN;
    }
  }
  
  // </editor-fold>
    
}

//===============================================================
// Name     : StateRegPitch
// Purpose  : Maintain the pitch position.
//===============================================================
void StateRegPitch(void)
{
    sUartLineBuffer_t buffer;
    
    if (DEBUG && oSendPitchValue)
    {
        buffer.length = sprintf(buffer.buffer,"StateReg\n\r Pos Maintain is: %d\n\r Current Pos is: %d\n\r", Pitch.posMaintain, Pitch.pos);
        Uart.PutTxFifoBuffer(UART6, &buffer);
    }
    
    if (!(LIMIT_SW_HI || LIMIT_SW_LOW))
    {
        if (Pitch.pos < Pitch.posMaintain)
        {
            direction = PITCH_UP;
            
            if (USE_DRIVE_A)
            {
                DRVA_DIR = PITCH_UP;
            }
            else if (USE_DRIVE_B)
            {
                DRVB_DIR = PITCH_UP;
            }
            else
            {
                DRVA_DIR = PITCH_UP;
                DRVB_DIR = PITCH_UP;
            }
        }
        else
        {
            direction = PITCH_DOWN;
            
            if (USE_DRIVE_A)
            {
                DRVA_DIR = PITCH_DOWN;
            }
            else if (USE_DRIVE_B)
            {
                DRVB_DIR = PITCH_DOWN;
            }
            else
            {
                DRVA_DIR = PITCH_DOWN;
                DRVB_DIR = PITCH_DOWN;
            }
        }
          
        if (oFlagStep)
        {
            if (USE_DRIVE_A && oMaintainPitch)
            {
              DRVA_SLEEP = 1;
              WriteDrive(DRVA, STATUS_PITCHw);
              DRVA_STEP ^= 1;
              LED_DEBG0 ^= 1;
            }
            else if (USE_DRIVE_B && oMaintainPitch)
            {
              DRVB_SLEEP = 1;
              WriteDrive(DRVB, STATUS_PITCHw);
              DRVB_STEP ^= 1;
              LED_DEBG0 ^= 1;
            }
            else
            {
              DRVA_SLEEP = 1;
              WriteDrive(DRVA, STATUS_PITCHw);
              DRVA_STEP ^= 1;
              LED_DEBG0 ^= 1;

              DRVB_SLEEP = 1;
              WriteDrive(DRVB, STATUS_PITCHw);
              DRVB_STEP ^= 1;
              LED_DEBG0 ^= 1;
            }
        }
//        if (USE_DRIVE_A && oMaintainPitch)
//        {
//          DRVA_SLEEP = 1;
//          WriteDrive(DRVA, STATUS_PITCHw);
//          DRVA_STEP ^= 1;
//          LED_DEBG0 ^= 1;
//        }
//        else if (USE_DRIVE_B && oMaintainPitch)
//        {
//          DRVB_SLEEP = 1;
//          WriteDrive(DRVB, STATUS_PITCHw);
//          DRVB_STEP ^= 1;
//          LED_DEBG0 ^= 1;
//        }
//        else
//        {
//          DRVA_SLEEP = 1;
//          WriteDrive(DRVA, STATUS_PITCHw);
//          DRVA_STEP ^= 1;
//          LED_DEBG0 ^= 1;
//
//          DRVB_SLEEP = 1;
//          WriteDrive(DRVB, STATUS_PITCHw);
//          DRVB_STEP ^= 1;
//          LED_DEBG0 ^= 1;
//        }
    }
    else
    {
      oMaintainPitch = 0;
    }
    
    if (!oMaintainPitch) 
    {
        if (USE_DRIVE_A)
        {
          DRVA_SLEEP = 0;
        }
        else if (USE_DRIVE_B)
        {
          DRVB_SLEEP = 0;
        }
        else
        {
          DRVA_SLEEP = 0;

          DRVB_SLEEP = 0;
        }
    }  
    
    oFlagStep = 0;
}

//===============================================================
// Name     : StateBRAKEPitch
// Purpose  : If driver presses AeroBrake or if systems detects
//            wind turbine overspeed. This state will change the
//            pitch to pre-determined value for the AeroBrake.
//===============================================================
void StateBrakePitch(void)
{
  sUartLineBuffer_t buffer;
  
  static BOOL overSpeed = 0;
  
  if (DEBUG)
  {
    buffer.length = sprintf(buffer.buffer,"AeroBrake\n\r");
    Uart.PutTxFifoBuffer(UART6, &buffer);
  }
  
  // <editor-fold defaultstate="collapsed" desc="Limit Switch and set direction">
  if (LIMIT_SW_LOW || LIMIT_SW_HI)
  {
    if (DEBUG)
    {
      buffer.length = sprintf(buffer.buffer,"Limit Reached\n\r");
      Uart.PutTxFifoBuffer(UART6, &buffer);
    }
    if (!overSpeed)
    {
      oCmdBrake = 0;
    }
    oMotorStop = 1;
  }
  else
  {
    if (USE_DRIVE_A && direction != PITCH_DOWN)
    {
      direction = PITCH_DOWN;
      DRVA_DIR = PITCH_DOWN;
    }
    else if (USE_DRIVE_B && direction != PITCH_DOWN)
    {
      direction = PITCH_DOWN;
      DRVB_DIR = PITCH_DOWN;
    }
    oMotorStop = 0;
  }
  // </editor-fold>
  
  if (TURB_OVERSPEED && !overSpeed)
  {
    overSpeed = 1;
    oCmdBrake = 1;
  }
  else if ((RELEASE_BRAKE || LIMIT_SW_LOW) && overSpeed)
  {
    overSpeed = 0;
    oCmdBrake = 0;
    oMotorStop = 1;
    //DRVB_SLEEP = 0;
  }
}

//===============================================================
// Name     : StateErrorPitch
// Purpose  : Error state of the system. Used to assess and
//            correct errors in the system.
//===============================================================
void StateErrorPitch(void)
{
  LED_ERROR ^= 1;
}