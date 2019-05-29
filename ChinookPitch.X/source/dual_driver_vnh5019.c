/*
 * dual_driver_vnh5019.c
 *
 *  Created on: Mar 24, 2018
 *      Author: Jerome Pare-Lepine
 */
#include "dual_driver_vnh5019.h"
#include "ChinookLib.h"

// Non-ultrascale Zynq SoCs have 4 I/O banks; 0&1 are MIO and 2&3 are EMIO.
// In the case of the z7007s, the MIO bank pin numbers range from [53:0] while theions
// EMIO pins range from [MAX:54]. Knowing this, the developer can access its EMIO
// pins declared in Vivado using the xgpiops driver by calling pin 54, for atomical functions
// ie: XGpioPs_SetDirectionPin, or the whole EMIO bank starting with bank 2, for the bank
// functions ie: XGpioPs_SetDirection.

//	Private Functions
int bankMask(int, int);

//	Public Functions
void initShieldPins(){
//	Make sure initSSIpins have already been called...
//	XGpioPs_SetDirection(bankPins, FPGA_BANK, (0x00000005U | OUT_REG));
//	XGpioPs_SetOutputEnable(bankPins, FPGA_BANK, (0x00000005U | OUT_REG));
//
////	TODO: Integrate to motor ctrl function
//	XGpioPs_WritePin(bankPins, PIN_M1INA, HI);
//	XGpioPs_WritePin(bankPins, PIN_M1INB, LO);
//
//	XGpioPs_WritePin(bankPins, PIN_M2INA, LO);
//	XGpioPs_WritePin(bankPins, PIN_M2INB, HI);

    
    
    P12_1_ON;       //M1INA HI
    P12_2_OFF;      //M1INB LO
    P12_4_OFF;      //M2INA LO
	P12_5_ON;       //M2INB HI
    
    
    P12_3_ON;
    P12_6_ON;
    
   
}

//void disDriver(XGpioPs* bankPins){
//	XGpioPs_WritePin(bankPins, M1EN, LO);
//}

void getFault(){
	//XGpioPs_ReadPin(bankPins, PIN_M1EN);
    
    READ_P12_3;
    
}

void oneStep(int dir, int stepdiv){ 
	// this static variable lets us remember what step we're on so we
	// can change to the appropriate next state in the sequence
	static unsigned char step = 0;

	// compute the next step based on the direction argument dir
	// and the step mode.  Full stepping skips half and quarter steps,
	// and half stepping skips quarter steps.  Quarter stepping cycles
	// through all 16 steps.
	if (dir == 1)
		step += stepdiv;
	else
		step -= stepdiv;
    
	switch (step & 15)
	{
		case 0:	// full step (both coils energized at 71%)
			updateDriver( 71, 71,FWD ,FWD );
             
            Pwm.SetDutyCycle(PWM_2,fullstepPwm); // test a 25 %
         

            Pwm.SetDutyCycle(PWM_3,fullstepPwm);
         
//            LED_DEBUG0_ON;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//          
            
			break;
		case 1:	// quarter step (coil 1 at 38% and coil 2 at 92%)
			updateDriver(38, 92,FWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_a);
        


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_b);
          
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
			break;
		case 2: // half step (coil 1 at 0% and coil 2 at 100%)
			updateDriver( 0, 100,FWD ,FWD);
            
            Pwm.SetDutyCycle(PWM_2,halfstepPwm_a );
       
    
    
            Pwm.SetDutyCycle(PWM_3,halfstepPwm_b);
           
//            LED_DEBUG0_ON;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
           int pwm2 = Pwm.ReadDutyCycle(PWM_2);
           int pwm3 = Pwm.ReadDutyCycle(PWM_3);
            //peak here
           pwm2++;
           pwm3++;
           
           
           
			break;
		case 3: // quarter step
			updateDriver(38,92,BWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_a);
     


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_b);
          
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
			break;
		case 4: // full step
			updateDriver( 71, 71,BWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2,fullstepPwm);
        


            Pwm.SetDutyCycle(PWM_3,fullstepPwm);
           
//            LED_DEBUG0_ON;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
			break;
		case 5: // quarter step
			updateDriver( 92, 38,BWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_b);
          

            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_a);
//         
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
			break;
		case 6: // half step
			updateDriver( 100, 0,BWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2, halfstepPwm_b);
           


            Pwm.SetDutyCycle(PWM_3,halfstepPwm_a );
       
            
//            LED_DEBUG0_ON;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_OFF;
//            
            //peak here
            
			break;
		case 7: // quarter step
			updateDriver( 92, 38,BWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_b);
           


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_a);
           
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
//            
            
			break;
		case 8: // full step
			updateDriver( 71, 71,BWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,fullstepPwm);
            


            Pwm.SetDutyCycle(PWM_3,fullstepPwm);
           
            
//            LED_DEBUG0_ON;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            
			break;
		case 9: // quarter step
			updateDriver( 38, 92,BWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_a);
          


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_b);
          
//            
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            
			break;
		case 10: // half step
			updateDriver( 0, 100,FWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,halfstepPwm_b);
          


            Pwm.SetDutyCycle(PWM_3,halfstepPwm_a);
           
            
//            LED_DEBUG0_ON;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            
            //peak here
            
			break;
		case 11: // quarter step
			updateDriver( 38, 92,FWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_a);
          


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_b);
           
            
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
//            
			break;
		case 12: // full step
			updateDriver( 71, 71,FWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,fullstepPwm);
           


            Pwm.SetDutyCycle(PWM_3,fullstepPwm);
           
//            LED_DEBUG0_ON;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            
            
			break;
		case 13: // quarter step
			updateDriver(92, 38,FWD,BWD);
            
            Pwm.SetDutyCycle(PWM_2,qrtstepPwm_b);
           


            Pwm.SetDutyCycle(PWM_3,qrtstepPwm_a);
          
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            
            
			break;
		case 14: // half step
			updateDriver( 100, 0,FWD,FWD);
            
            Pwm.SetDutyCycle(PWM_2,250 );
          


            Pwm.SetDutyCycle  (PWM_3,halfstepPwm_a);
          
            
//            LED_DEBUG0_ON;
//            LED_DEBUG1_ON;
//            LED_DEBUG2_ON;
//            LED_DEBUG3_ON;
//            LED_DEBUG4_OFF;
            //peak
			break;
		case 15: // quarter step
			updateDriver( 92, 38,FWD,FWD);
            
            Pwm.SetDutyCycle  (PWM_2,qrtstepPwm_b);
           


            Pwm.SetDutyCycle  (PWM_3,qrtstepPwm_a);
            
//            LED_DEBUG0_OFF;
//            LED_DEBUG1_OFF;
//            LED_DEBUG2_OFF;
//            LED_DEBUG3_OFF;
//            LED_DEBUG4_ON;
            
            
			break;
	}
    
    
}

void updateDriver(int phA,int phB,int dirA ,int dirB ){
	
    
    
	if(phA > 0){
		setPins( PHA, dirA);
	}
	else if(phA == 0){
		setPins( PHA, CUT);
	}
    
    
	if(phB > 0){
		setPins(PHB, dirB);        
        
	}

	else if(phB == 0){
		setPins( PHB, CUT);
	}


	//Update register
//	reg |= ( (XGpioPs_Read(bankPins, 0x02U) & ~PHX_MASK) | pins);
//	XGpioPs_Write(bankPins, 0x02U, reg);
	//Update PWM values
 //SetPwm(phA, phB); 
}

void setPins( int phase, int dir){
    
        
    
    
   
          
   
	if(phase == PHA){
		if(dir == FWD){
			//XGpioPs_WritePin(bankPins, PIN_M1INA, HI);
            P12_1_ON;
          //  P12_3_ON;
			//XGpioPs_WritePin(bankPins, PIN_M1INB, LO);
            P12_2_OFF;
          //  P12_6_OFF;
            
		}
		else if(dir == BWD){
			//XGpioPs_WritePin(bankPins, PIN_M1INA, LO);
            P12_1_OFF;
          //  P12_3_OFF;
			//XGpioPs_WritePin(bankPins, PIN_M1INB, HI);
            P12_2_ON;
          //  P12_6_ON;
		}
		else{
		//	XGpioPs_WritePin(bankPins, PIN_M1INA, LO);
            P12_1_OFF; 
        //    P12_3_OFF;
            
		//	XGpioPs_WritePin(bankPins, PIN_M1INB, LO);
            P12_2_OFF;
          //  P12_6_OFF;
		}
	}
	else{	//phase b
		if(dir == FWD){
			//XGpioPs_WritePin(bankPins, PIN_M2INA, HI);
           P12_4_ON;
          //P12_3_ON;
			//XGpioPs_WritePin(bankPins, PIN_M2INB, LO);
           P12_5_OFF;
           //P12_6_OFF;
		}
		else if(dir == BWD){
			//XGpioPs_WritePin(bankPins, PIN_M2INA, LO);
           P12_4_OFF;
         //  P12_3_OFF;
			//XGpioPs_WritePin(bankPins, PIN_M2INB, HI);
           P12_5_ON;
         // P12_6_ON;
		}
		else{
			//XGpioPs_WritePin(bankPins, PIN_M2INA, LO);
           P12_4_OFF;
         //  P12_3_OFF;
			//XGpioPs_WritePin(bankPins, PIN_M2INB, LO);
           P12_5_OFF;
          // P12_6_OFF;
		}
	}
    
    
    
}


// TODO: Integrate the polulu example
void stepCmd( int* stpstate){
/*
	int pins = 0;
//	TODO: Extract Gpio operations to a separate src & h
//	pins = XGpioPs_Read(bankPins, 0x02U);

	switch (*stpstate){
	case 0: // Evaluate both of the phases' directions
		pins = bankMask(pins, STEP1);
		XGpioPs_Write(bankPins, 0x2U, pins);	// Write to the correct banks
		// Update PWM
		break;
	case 1:
		pins = bankMask(pins, STEP2);
		XGpioPs_Write(bankPins, 0x2U, pins);
		break;
	case 2:
		pins = bankMask(pins, STEP3);
		XGpioPs_Write(bankPins, 0x2U, pins);
		break;
	case 3:
		pins = bankMask(pins, STEP4);
		XGpioPs_Write(bankPins, 0x2U, pins);
	}

//	TODO: Increment the state counter
	*stpstate += 1;
	if((*stpstate >= 4) | (*stpstate < 0)){
		*stpstate = 0;
	}*/
}

int bankMask(int rread, int mask){
	rread |= mask;
	rread &= mask;
	return rread;
}



