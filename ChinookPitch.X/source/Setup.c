//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Setup.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the setup of the system. It contains the
//           initialization functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//           Function names can and should be renamed by the user to improve the
//           readability of the code. Also, the LED used for testing errors in
//           TimerInit is a LED on the MAX32 development board. Developpers
//           should test for errors by the means (hardware of software) they
//           judge are the best.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Setup.h"
#include "Interrupts.h"
#include "SkadiFunctions.h"
#include <stdlib.h>
#include "ChinookCanUtils.h"

//===========================
//	INIT TIMERS
//===========================
void InitTimer(void)
{

  INT32 timerCounterValue = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Open timers
//%%%%%%%%%%%%%%%%%%%%%%%%%%%
  timerCounterValue = Timer.Open(TIMER_1, 100, SCALE_MS);
  if (timerCounterValue < 0)
  {
    Port.A.SetBits(BIT_3);    // LED4 on MAX32
  }
  timerCounterValue = Timer.Open(TIMER_2, 500, SCALE_US);   // Open Timer 2 with a period of 500 us
  if (timerCounterValue < 0)
  {
    Port.A.SetBits(BIT_3);    // LED4 on MAX32
  }
  timerCounterValue = Timer.Open(TIMER_3, 100, SCALE_US);
  if (timerCounterValue < 0)
  {
    Port.A.SetBits(BIT_3);    // LED4 on MAX32
  }
  timerCounterValue = Timer.Open(TIMER_4, 500, SCALE_MS);   // Open Timer 4 with a period of 500 ms
  if (timerCounterValue < 0)
  {
    Port.A.SetBits(BIT_3);    // LED4 on MAX32
  }
  timerCounterValue = Timer.Open(TIMER_5, 500, SCALE_US);   // Open Timer 5 with a period of 500 us
  if (timerCounterValue < 0)
  {
    Port.A.SetBits(BIT_3);    // LED4 on MAX32
  }

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Configure timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Timer.ConfigInterrupt(TIMER_1, TIMER1_INTERRUPT_PRIORITY, TIMER1_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_1 to the values specified in Interrupt.h
  Timer.ConfigInterrupt(TIMER_2, TIMER2_INTERRUPT_PRIORITY, TIMER2_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_2 to the values specified in Interrupt.h
  Timer.ConfigInterrupt(TIMER_3, TIMER3_INTERRUPT_PRIORITY, TIMER3_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_3 to the values specified in Interrupt.h
  Timer.ConfigInterrupt(TIMER_4, TIMER4_INTERRUPT_PRIORITY, TIMER4_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_4 to the values specified in Interrupt.h
  Timer.ConfigInterrupt(TIMER_5, TIMER5_INTERRUPT_PRIORITY, TIMER5_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_5 to the values specified in Interrupt.h

}


//===========================
//	INIT SPI
//===========================
void InitSpi(void)
{
  INT8 err = 0;
  SpiOpenFlags_t oMasterFlags =   SPI_MASTER_MODE
                                | SPI_MASTER_SS
                                | SPI_16_BITS_CHAR
                                | SPI_ENHANCED_BUFFER_MODE
                                | SPI_TX_EVENT_BUFFER_EMPTY
                                | SPI_RX_EVENT_BUFFER_NOT_EMPTY

                ,oSlaveFlags  =   SPI_SLAVE_MODE
                                | SPI_SLAVE_SS
                                | SPI_16_BITS_CHAR
                                | SPI_ENHANCED_BUFFER_MODE
                                | SPI_TX_EVENT_BUFFER_EMPTY
                                | SPI_RX_EVENT_BUFFER_NOT_EMPTY
                ;

#ifdef __32MX795F512L__   // SPI1 only on this device

  err = Spi.Open(SPI1, oSlaveFlags, 5e6);   // Open the SPI1 as a slave at a bitrate of 5 MHz
  if (err < 0)                // Check for errors
  {
    Port.C.SetBits(BIT_1);    // Turn on the LD5 on MAX32
  }

  // SPI interrupts not functionnal as of yet
//  Spi.ConfigInterrupt(SPI1, SPI1_INTERRUPT_PRIORITY, SPI1_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI1
#endif

  err = Spi.Open(SPI2, oSlaveFlags, 5e6);   // Open the SPI2 as a slave at a bitrate of 5 MHz
  if (err < 0)                // Check for errors
  {
    Port.C.SetBits(BIT_1);    // Turn on the LD5 on MAX32
  }
  err = Spi.Open(SPI3, oMasterFlags, 5e6);   // Open the SPI3 as a master at a bitrate of 5 MHz
  if (err < 0)                // Check for errors
  {
    Port.C.SetBits(BIT_1);    // Turn on the LD5 on MAX32
  }
  err = Spi.Open(SPI4, oMasterFlags, 5e6);   // Open the SPI4 as a master at a bitrate of 5 MHz
  if (err < 0)                // Check for errors
  {
    Port.C.SetBits(BIT_1);    // Turn on the LD5 on MAX32
  }

  // SPI interrupts not functionnal as of yet
//  Spi.ConfigInterrupt(SPI2, SPI2_INTERRUPT_PRIORITY, SPI2_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI2
//  Spi.ConfigInterrupt(SPI3, SPI3_INTERRUPT_PRIORITY, SPI4_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI3
//  Spi.ConfigInterrupt(SPI4, SPI3_INTERRUPT_PRIORITY, SPI4_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI4
}


//===========================
//	INIT PWM
//===========================
void InitPwm(void)
{

  Pwm.Open(PWM_3);
  Pwm.Open(PWM_2);
//  Pwm.SetDutyCycle  (PWM_3, 10);
  Pwm.SetDutyCycle  (PWM_3, 10);
  Pwm.SetPulseOffset(PWM_3, 0);

  Pwm.SetDutyCycle  (PWM_2, 10);
  Pwm.SetPulseOffset(PWM_2, 500);
  
}

void SetPwm(int a,int b )
{
    //LED_DEBUG4_OFF();
    
    int a_pourcent = (a*10)-1 ;
    int b_pourcent = (b*10)-1 ;
    
    Pwm.SetDutyCycle  (PWM_2,a-1 );
    Pwm.SetPulseOffset(PWM_2,0);
    
    Pwm.SetDutyCycle  (PWM_3,b-1);
    Pwm.SetPulseOffset(PWM_3,0);
}
//===========================
//	INIT PORTS
//===========================
void InitPorts(void)
{
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set unused pins as input to protect the pins of the microcontroller
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Port.A.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RA8, RA11-13 non existent
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_9  | BIT_10 | BIT_14 | BIT_15 );

  Port.B.CloseBits  (/* BIT_0  | BIT_1  | BIT_2  | BIT_3
                    |*/ BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.C.CloseBits  ( BIT_1  | BIT_2  | BIT_3  | BIT_4      // RC0, RC5-11 non existent
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.D.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.E.CloseBits  ( BIT_0  | BIT_1  | BIT_2  |/* BIT_3      // RE10-15 non existent
                    | BIT_4  |*/ BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  );

  Port.F.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RF6-7, RF9-11, RF14-15 non existent
                    | BIT_4  | BIT_8  | BIT_12
                    | BIT_13 );

  Port.G.CloseBits  ( BIT_0  | BIT_1  /*| BIT_2  | BIT_3  */    // RG4-5, RG10-11 non existent
                    | BIT_6  | BIT_7  | BIT_8  | BIT_9
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

   /* Setup IO LED */
  Port.B.SetPinsDigitalOut(BIT_8);      // LED_DEBG0
  Port.B.SetPinsDigitalOut(BIT_9);      // LED_DEBG1
  Port.B.SetPinsDigitalOut(BIT_10);     // LED_DEBG2
  Port.B.SetPinsDigitalOut(BIT_11);     // LED_DEBG3
  Port.B.SetPinsDigitalOut(BIT_12);     // LED_DEBG4
  Port.B.SetPinsDigitalOut(BIT_13);     // LED_CAN
  Port.B.SetPinsDigitalOut(BIT_15);     // LED_ERROR
  Port.F.SetPinsDigitalOut(BIT_3);      // LED_STATUS

  /* Setup  IO switch */
  Port.E.SetPinsDigitalIn(BIT_5);      // SW1
  Port.E.SetPinsDigitalIn(BIT_6);      // SW2
  Port.E.SetPinsDigitalIn(BIT_7);      // SW3
  
  /* Setup functions */
  Port.D.SetPinsDigitalOut(BIT_2);      // AP7 --> P12_13 --> PWM_3 (10)
  Port.D.SetPinsDigitalOut(BIT_1);      // AP6 --> P12_11 --> PWM_2 (9)
  Port.D.SetPinsDigitalOut(BIT_3);      // MP3 Player
  Port.F.SetPinsDigitalIn (BIT_0);      // CAN1 RX
  Port.F.SetPinsDigitalOut(BIT_1);      // CAN1 TX
  
//  Port.D.ClearBits(BIT_3);
  Port.D.SetBits(BIT_3);

  LED_STATUS_OFF();
  LED_ERROR_OFF();
  LED_CAN_OFF();
  LED_DEBUG4_OFF();
  LED_DEBUG3_OFF();
  LED_DEBUG2_OFF();
  LED_DEBUG1_OFF();
  LED_DEBUG0_OFF();
  
  /* P12 Board Pins usability */
  
  Port.B.SetPinsDigitalOut(BIT_0);  // AP2--> P12_3 -->  M1EN/DIAG (6)
  Port.B.SetPinsDigitalOut(BIT_1);  // AP3 --> P12_7 --> 
  Port.B.SetPinsDigitalOut(BIT_2);  // AP1 --> P12_1 --> M1INA (2)
  Port.B.SetPinsDigitalOut(BIT_3);  // AP10 --> P12_2 --> M1INB (4)
  
  Port.E.SetPinsDigitalOut(BIT_3);  // AP4 --> P12_8  
  Port.E.SetPinsDigitalOut(BIT_4);  // AP11 --> P12_4 --> M2INA (7)
  
  Port.F.SetPinsDigitalOut(BIT_3);  // AP12 --> P12_5 --> M2INB (8)
  Port.F.SetPinsDigitalOut(BIT_4);  // AP13 --> P12_6 --> M2EN/DIAG (12)
  Port.F.SetPinsDigitalOut(BIT_5);  // AP14 --> P12_9
  
  Port.G.SetPinsAnalogIn(BIT_2);    //A0 (m1cs)
  Port.G.SetPinsAnalogIn(BIT_3);    //A1 (m2cs)
}


//===========================
//	INIT UART
//===========================
void InitUart (void)
{
  UartConfig_t       oConfig      = UART_ENABLE_PINS_TX_RX_ONLY;
  UartFifoMode_t     oFifoMode    = UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_NOT_EMPTY;
  UartLineCtrlMode_t oLineControl = UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1;

  Uart.Open(UART1, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 1 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
  Uart.Open(UART2, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 2 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
#ifndef __32MX320F128H__  // Uno32 doesn't have UART3-6
  Uart.Open(UART3, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 3 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
  Uart.Open(UART4, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 4 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
  Uart.Open(UART5, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 5 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
  Uart.Open(UART6, BAUD9600, oConfig, oFifoMode, oLineControl);   // Open UART 6 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
#endif

  Uart.EnableRx(UART1);
  Uart.EnableRx(UART2);
#ifndef __32MX320F128H__  // Uno32 doesn't have UART3-6
  Uart.EnableRx(UART3);
  Uart.EnableRx(UART4);
  Uart.EnableRx(UART5);
  Uart.EnableRx(UART6);
#endif

  Uart.EnableTx(UART1);
  Uart.EnableTx(UART2);
#ifndef __32MX320F128H__  // Uno32 doesn't have UART3-6
  Uart.EnableTx(UART3);
  Uart.EnableTx(UART4);
  Uart.EnableTx(UART5);
  Uart.EnableTx(UART6);
#endif

  Uart.ConfigInterrupt(UART1, UART1_INTERRUPT_PRIORITY, UART1_INTERRUPT_SUBPRIORITY);
  Uart.ConfigInterrupt(UART2, UART2_INTERRUPT_PRIORITY, UART2_INTERRUPT_SUBPRIORITY);
#ifndef __32MX320F128H__  // Uno32 doesn't have UART3-6
  Uart.ConfigInterrupt(UART3, UART3_INTERRUPT_PRIORITY, UART3_INTERRUPT_SUBPRIORITY);
  Uart.ConfigInterrupt(UART4, UART4_INTERRUPT_PRIORITY, UART4_INTERRUPT_SUBPRIORITY);
  Uart.ConfigInterrupt(UART5, UART5_INTERRUPT_PRIORITY, UART5_INTERRUPT_SUBPRIORITY);
  Uart.ConfigInterrupt(UART6, UART6_INTERRUPT_PRIORITY, UART6_INTERRUPT_SUBPRIORITY);
#endif
}

//===========================
//	INIT CAN BUS
//===========================
void InitCan(void)
{
  Can.Initialize(CAN1, Can1MessageFifoArea, CAN_NB_CHANNELS, CAN_BUFFER_SIZE, FALSE);

  // Channel 1 receives CAN messages from the steering wheel buttons (only works in manual mode)
  Can.SetChannel(CAN1, CAN_CHANNEL1, 8, RX);
  //Can.SetChannelMask(CAN1, CAN_CHANNEL1, CAN_FILTER0, VOLANT_SW_MSG, CAN_FILTER_MASK0, 0x7FF);
  Can.SetChannelMask(CAN1, CAN_CHANNEL1, CAN_FILTER0, 0x43, CAN_FILTER_MASK0, 0x7FF);
  
  // Channel 2 sets the pitch mode of the drive
  // automatic = accepts inputs from steering wheel (channel 1) AND automatic pitch control (channel 3)
  Can.SetChannel(CAN1, CAN_CHANNEL2, 8, RX);
  Can.SetChannelMask(CAN1, CAN_CHANNEL2, CAN_FILTER1, ID_PITCH_MODE_SID, CAN_FILTER_MASK0, 0x7FF);
  
  // Channel 3 receives the target pitch the drive should aim for as a number of steps (float)
  // The drive will then automatically move 
  Can.SetChannel(CAN1, CAN_CHANNEL3, 8, RX);
  Can.SetChannelMask(CAN1, CAN_CHANNEL3, CAN_FILTER2, ID_PITCH_TARGET, CAN_FILTER_MASK0, 0x7FF);
  
  // Channel 4 is for receiving an interrupt for ROPS
  Can.SetChannel(CAN1, CAN_CHANNEL4, 8, RX);
  Can.SetChannelMask(CAN1, CAN_CHANNEL4, CAN_FILTER3, ID_ROPS, CAN_FILTER_MASK0, 0x7FF);
  
  // Channel 5 is for emergency pitch stop
  Can.SetChannel(CAN1, CAN_CHANNEL5, 8, RX);
  Can.SetChannelMask(CAN1, CAN_CHANNEL5, CAN_FILTER4, ID_EMERGENCY_STOP, CAN_FILTER_MASK0, 0x7FF);
  
  Can.ConfigInterrupt(CAN1, CAN1_INTERRUPT_PRIORITY, CAN1_INTERRUPT_SUBPRIORITY);
}


//===========================
//	INIT I2C
//===========================
void InitI2c(void)
{
  INT8 err;
  I2c.Open(I2C1, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C1, I2C1_INTERRUPT_PRIORITY, I2C1_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
//    LED_ERROR_ON;
  }
#ifndef __32MX795F512H__          // Chinook boards don't have I2C2
  I2c.Open(I2C2, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C2, I2C2_INTERRUPT_PRIORITY, I2C2_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
//    LED_ERROR_ON;
  }
#endif
#ifndef __32MX320F128H__          // Uno32 doesn't have I2C3 and I2C4
  I2c.Open(I2C3, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C3, I2C3_INTERRUPT_PRIORITY, I2C3_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
//    LED_ERROR_ON;
  }
  I2c.Open(I2C4, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C4, I2C4_INTERRUPT_PRIORITY, I2C4_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
//    LED_ERROR_ON;
  }
  I2c.Open(I2C5, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C5, I2C5_INTERRUPT_PRIORITY, I2C5_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
//    LED_ERROR_ON;
  }
#endif
}


//===========================
//	INIT WATCHDOG TIMER
//===========================
void InitWdt(void)
{
  Wdt.Enable();
}


//===========================
//	INIT ADC
//===========================
void InitAdc(void)
{
  // Mode of operation. Uncomment the one you need.
  //================================================
  UINT32 samplingClk = ADC_CLK_TMR;     // Timer3 used for sampling
//  UINT32 samplingClk = ADC_CLK_AUTO;    // Internal counter used for sampling
//  UINT32 samplingClk = ADC_CLK_INT0;    // Active transition on INT0 used for sampling
//  UINT32 samplingClk = ADC_CLK_MANUAL;  // Manual sampling.
  //================================================

  // Hardware config. These are exemples.
  //================================================
  UINT32 configHardware = ADC_VREF_AVDD_AVSS      // Vref+ is AVdd and Vref- is AVss
                        | ADC_SAMPLES_PER_INT_3;  // 3 samples/interrupt (we check 3 channels)
//  UINT32 configHardware = ADC_VREF_EXT_AVSS       // Vref+ is External and Vref- is AVss
//                        | ADC_SAMPLES_PER_INT_1; // 1 sample/interrupt => used in manual mode
  //================================================

  // Port config. These are exemples.
  //================================================
//  UINT32 configPort = ENABLE_ALL_ANA; // Enable AN0-AN15 in analog mode
//  UINT32 configScan = 0; // Scan all ANs

  UINT32 configPort = ENABLE_AN0_ANA
                    | ENABLE_AN4_ANA
                    | ENABLE_AN13_ANA; // Enable AN0, AN4 and AN13 in analog mode
  
  UINT32 configScan = SKIP_SCAN_AN1
                    | SKIP_SCAN_AN2
                    | SKIP_SCAN_AN3
                    | SKIP_SCAN_AN5
                    | SKIP_SCAN_AN6
                    | SKIP_SCAN_AN7
                    | SKIP_SCAN_AN8
                    | SKIP_SCAN_AN9
                    | SKIP_SCAN_AN10
                    | SKIP_SCAN_AN11
                    | SKIP_SCAN_AN12
                    | SKIP_SCAN_AN14
                    | SKIP_SCAN_AN15; // Don't scan the channels that are not enabled by configPort
  //================================================

  // Open ADC with parameters above
  Adc.Open(samplingClk, configHardware, configPort, configScan);

  Adc.ConfigInterrupt(ADC_INTERRUPT_PRIORITY, ADC_INTERRUPT_SUBPRIORITY);
}


//===========================
//	INIT INPUT CAPTURE
//===========================
void InitInputCapture(void)
{
  // Capture every rising edge, 1 interrupt each capture, use the 16 bits Timer 2, capture the first rising edge, Input Capture ON
  UINT16 config = IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_TIMER2_SRC | IC_CAP_16BIT | IC_FEDGE_RISE | IC_ON;

  // Capture every rising edge, 1 interrupt each capture, use the 32 bits Timer 23, capture the first rising edge, Input Capture ON
//  UINT16 config = IC_EVERY_RISE_EDGE | IC_INT_1CAPTURE | IC_CAP_32BIT | IC_FEDGE_RISE | IC_ON;

  InputCapture.Open(IC1, config);
  InputCapture.Open(IC2, config);
  InputCapture.Open(IC3, config);
  InputCapture.Open(IC4, config);
  InputCapture.Open(IC5, config);

  InputCapture.ConfigInterrupt(IC1, IC1_INTERRUPT_PRIORITY, IC1_INTERRUPT_SUBPRIORITY);
  InputCapture.ConfigInterrupt(IC2, IC2_INTERRUPT_PRIORITY, IC2_INTERRUPT_SUBPRIORITY);
  InputCapture.ConfigInterrupt(IC3, IC3_INTERRUPT_PRIORITY, IC3_INTERRUPT_SUBPRIORITY);
  InputCapture.ConfigInterrupt(IC4, IC4_INTERRUPT_PRIORITY, IC4_INTERRUPT_SUBPRIORITY);
  InputCapture.ConfigInterrupt(IC5, IC5_INTERRUPT_PRIORITY, IC5_INTERRUPT_SUBPRIORITY);
  
}


//===========================
//	START INTERRUPTS
//===========================
void StartInterrupts(void)
{
  INT8 err;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Timer.EnableInterrupt(TIMER_1);
//  Timer.EnableInterrupt(TIMER_2);
//  Timer.EnableInterrupt(TIMER_3);
//  Timer.EnableInterrupt(TIMER_4);
//  Timer.EnableInterrupt(TIMER_5);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable RX UART interrupts and disable TX interrupts. 
// TX interrupts are disabled at init and only
// enabled when writing to the user's TX FIFO buffer
// with Uart.PutTxFifoBuffer(...)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  Uart.EnableRxInterrupts (UART6);  // Enable RX Interrupts for UART6
//  Uart.DisableTxInterrupts(UART6);  // Disable TX Interrupts for UART6


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable ADC interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  Adc.EnableInterrupts();   // Works only when not in manual mode


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable InputCapture interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  InputCapture.EnableInterrupt(IC1);
//  InputCapture.EnableInterrupt(IC2);
//  InputCapture.EnableInterrupt(IC3);
//  InputCapture.EnableInterrupt(IC4);
//  InputCapture.EnableInterrupt(IC5);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable CAN interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Can.EnableInterrupt(CAN1);
  

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable multi-vector interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();
}
