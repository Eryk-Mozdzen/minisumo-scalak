#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "RC5.h"
#include "WS2812B.h"
#include "Scalak_lib.h"
#include "PID_Controller.h"

#define ROBOT_STATE_INITIALIZE	0
#define ROBOT_STATE_READY		1
#define ROBOT_STATE_FIGHT		2
#define	ROBOT_STATE_SEARCH		3
#define ROBOT_STATE_WAIT		4

Stream serial;
DecoderRC5 decoder;
PID pid(127.5, 0, 0);

ISR(INT0_vect) {
	decoder.INT_ISR();
}

ISR(TIMER2_OVF_vect) {
	decoder.TIMER_ISR();
}

drv8838 motors;
IR38kHz dist;
QTR1A edge;
Buzzer beep;
Switch panel;
StartModule remote;
Leds led(1);

bool lastEnemy = false;
uint8_t presentState;
uint16_t timerCounter = 0;

void fight(double processVarible, uint8_t targetPower) {
	lastEnemy = (processVarible>0);

	double turn = pid.calculate(processVarible, 0);

	uint8_t powerL = drv8838::fix((double)(targetPower)+turn);
	uint8_t powerR = drv8838::fix((double)(targetPower)-turn);

	motors.setSpeeds(powerL, powerR);
}

void search() {
	uint8_t powerL = 0;
	uint8_t powerR = 0;

	switch(edge.getHorizon()) {
		case 0:                                     break;
		case 1: lastEnemy = false;  powerL = -150;  break;
		case 2: lastEnemy = true;   powerR = -150;  break;
		case 3:                                     break;
	}

	if(lastEnemy) powerL = 255;
	else          powerR = 255;

	pid.reset();
	motors.setSpeeds(powerL, powerR);
}

int main() {
	
	presentState = ROBOT_STATE_INITIALIZE;
	
	serial.begin(9600);
	decoder.initialize();
	motors.initialize(FORWARD, BACKWARD);
	dist.initialize();
	edge.initialize(AUTO, 500);
	beep.initialize();
	panel.initialize();
	//remote.initialize(MANUAL);
	remote.initialize(MODULE);
	//remote.initialize(OFF);
	
	//remote.print();
	
	presentState = ROBOT_STATE_READY;

    while(true) {
		
		remote.wait(&serial, &decoder, []() {
			motors.stop();
			beep.stop();
			switch(remote.getState()) {
				case STATE_PROGRAMING:	led.set(255, 255, 0);	break;
				case STATE_POWER_ON:	led.set(0, 0, 255);		break;
				case STATE_STOPPED:		led.set(255, 0, 0);		break;
			}
			led.show();
			if(remote.getState()==STATE_PROGRAMING) _delay_ms(200);
		}, []() {
			lastEnemy = panel.getState();
			
			//if(remote.getMode()==MANUAL) _delay_ms(5000);

			motors.setSpeeds(255);
			_delay_ms(150);
			
			presentState = ROBOT_STATE_SEARCH;
		});
		
		led.set(0, 255, 0);
		
		double processVarible = dist.getError(SINGLE);

		if(isnan(processVarible)) {
			timerCounter++;
			presentState = ROBOT_STATE_SEARCH;
		} else {
			timerCounter = 0;
			presentState = ROBOT_STATE_FIGHT;
		}
		
		if(timerCounter>30) {
			presentState = ROBOT_STATE_WAIT;
		}
		
		switch(presentState) {
			case ROBOT_STATE_READY: {
				
			} break;
		}
		
		//motors.print(&serial);
		//dist.print(&serial);
		//edge.print(&serial);
		//panel.print(&serial);
		
		led.show();
		_delay_ms(50);
    }
}