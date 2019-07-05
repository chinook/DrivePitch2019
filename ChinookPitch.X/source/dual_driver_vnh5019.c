/*
 * dual_driver_vnh5019.c
 *
 *  Created on: Mar 24, 2018
 *      Author: Jerome Pare-Lepine
 */
#include "dual_driver_vnh5019.h"
#include "ChinookLib.h"

#include "StateMachine.h"

//	Private Functions
int bankMask(int, int);

//	Public Functions
void initShieldPins(){
    SET_M1INA();       //M1INA HI
    SET_M1INB();      //M1INB LO
    SET_M2INA();      //M2INA LO
	SET_M2INB();       //M2INB HI
    
    SET_M1EN();
    SET_M2EN();
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
    
    current_pitch += (dir == FWD ? 0.25 * stepdiv : -0.25 * stepdiv);

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
        updateDriver(71, 71, FWD, FWD);

        Pwm.SetDutyCycle(PWM_2, fullstepPwm); // test a 25 %
        Pwm.SetDutyCycle(PWM_3, fullstepPwm);

        break;
    case 1:	// quarter step (coil 1 at 38% and coil 2 at 92%)
        updateDriver(38, 92, FWD, FWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_a);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_b);

        break;
    case 2: // half step (coil 1 at 0% and coil 2 at 100%)
        updateDriver(0, 100, FWD ,FWD);

        Pwm.SetDutyCycle(PWM_2, halfstepPwm_a);
        Pwm.SetDutyCycle(PWM_3, halfstepPwm_b);

        break;
    case 3: // quarter step
        updateDriver(38, 92, BWD, FWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_a);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_b);

        break;
    case 4: // full step
        updateDriver(71, 71, BWD, FWD);

        Pwm.SetDutyCycle(PWM_2, fullstepPwm);
        Pwm.SetDutyCycle(PWM_3, fullstepPwm);

        break;
    case 5: // quarter step
        updateDriver(92, 38, BWD, FWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_a);

        break;
    case 6: // half step
        updateDriver(100, 0, BWD, FWD);

        Pwm.SetDutyCycle(PWM_2, halfstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, halfstepPwm_a);

        break;
    case 7: // quarter step
        updateDriver(92, 38, BWD, BWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_a);  

        break;
    case 8: // full step
        updateDriver(71, 71, BWD, BWD);

        Pwm.SetDutyCycle(PWM_2, fullstepPwm);
        Pwm.SetDutyCycle(PWM_3, fullstepPwm);

        break;
    case 9: // quarter step
        updateDriver(38, 92, BWD, BWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_a);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_b);

        break;
    case 10: // half step
        updateDriver(0, 100, FWD, BWD);

        Pwm.SetDutyCycle(PWM_2, halfstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, halfstepPwm_a);

        break;
    case 11: // quarter step
        updateDriver(38, 92, FWD, BWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_a);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_b);

        break;
    case 12: // full step
        updateDriver(71, 71, FWD, BWD);

        Pwm.SetDutyCycle(PWM_2, fullstepPwm);
        Pwm.SetDutyCycle(PWM_3, fullstepPwm);

        break;
    case 13: // quarter step
        updateDriver(92, 38, FWD, BWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_a);

        break;
    case 14: // half step
        updateDriver(100, 0, FWD, FWD);

        Pwm.SetDutyCycle(PWM_2, halfstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, halfstepPwm_a);

        break;
    case 15: // quarter step
        updateDriver(92, 38, FWD, FWD);

        Pwm.SetDutyCycle(PWM_2, qrtstepPwm_b);
        Pwm.SetDutyCycle(PWM_3, qrtstepPwm_a);

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
}

void setPins( int phase, int dir){
    
	if(phase == PHA){
		if(dir == FWD){
            SET_M1INA();
            CLR_M1INB();
		}
		else if(dir == BWD){
            CLR_M1INA();
            SET_M1INB();
		}
		else{
            CLR_M1INA(); 
            CLR_M1INB();
		}
	}
	else{	//phase b
		if(dir == FWD){
           SET_M2INA();
           CLR_M2INB();
		}
		else if(dir == BWD){
           CLR_M2INA();
           SET_M2INB();
		}
		else{
           CLR_M2INA();
           CLR_M2INB();
		}
	}
}

int bankMask(int rread, int mask){
	rread |= mask;
	rread &= mask;
	return rread;
}



