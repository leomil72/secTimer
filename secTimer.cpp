/* This file is part of secTimer library.
   Please check the README file and the notes
   inside the timer.h file
*/


//this library is compatible both with Arduino <=0023 and Arduino >=100
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


//include required libraries
#include "secTimer.h"


//declare some global variables
#if defined(ATMEGAxU)
volatile unsigned int _starter = 0;
#else
volatile uint8_t _starter = 0;
#endif
volatile int _counterT;
volatile unsigned long _secondsT;


//class constructor: these instructions are executed when a new istance of the lib is created
secTimer::secTimer(void) {
	_isRunning=false;
	setTimer();
}


// ***********************************
// ********* private methods *********
// ***********************************

//set the timer
void secTimer::setTimer() {
    float prescaler = 0.0;

#if defined (ATMEGAx8) || defined (ATMEGAx4) || defined (ATMEGAx0)
    //during setup, disable all the interrupts based on timer
    TIMSK2 &= ~((1<<TOIE2) | (1<<OCIE2A) | (1<<OCIE2B));
    //prescaler source clock set to internal Atmega clock (asynch mode)
    ASSR &= ~(1<<AS2);
    //this sets the timer to increment the counter until overflow
    TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
    TCCR2B &= ~(1<<WGM22);
    //the following code sets the prescaler depending on the system clock
    if (F_CPU == 16000000UL) {   // prescaler set to 64
        TCCR2B |= (1<<CS22);
        TCCR2B &= ~((1<<CS21) | (1<<CS20));
        prescaler = 64.0;
    } else if ((F_CPU == 8000000UL) || (F_CPU == 4000000UL)) { // prescaler set to 32
        TCCR2B &= ~(1<<CS22);
        TCCR2B |= ((1<<CS21) | (1<<CS20));
        prescaler = 32.0;
    } else if (F_CPU == 1000000UL) { // prescaler set to 8
        TCCR2B &= ~((1<<CS22) | (1<<CS20));
        TCCR2B |= (1<<CS21);
        prescaler = 8.0;
    }
#elif defined (ATTINYx5) || defined (ATTINYx313)
    //during setup, disable all the interrupts based on timer 0
    TIMSK &= ~((1<<TOIE0) | (1<<OCIE0A) | (1<<OCIE0B));
    //normal mode: counter not connected to external pins
    TCCR0A &= ~((1<<COM0A0) | (1<<COM0A1));
    //this sets the timer to increment the counter until overflow
    TCCR0A &= ~((1<<WGM01) | (1<<WGM00));
    TCCR0B &= ~(1<<WGM02);
    //the following code sets the prescaler depending on the system clock
    if ((F_CPU == 16000000UL) || (F_CPU == 8000000UL)) {   // prescaler set to 64
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= ((1<<CS01) | (1<<CS00));
        prescaler = 64.0;
    } else if (F_CPU == 1000000UL) { // prescaler set to 8
        TCCR0B &= ~((1<<CS02) | (1<<CS00));
        TCCR0B |= (1<<CS01);
        prescaler = 8.0;
    }
#elif defined (ATTINYx4)
    //on Attinyx4 we must use the timer 0 because timer1 is a 16 bit counter

    //during setup, disable all the interrupts based on timer 0
    TIMSK0 &= ~((1<<TOIE0) | (1<<OCIE0A) | (1<<OCIE0B));
    //normal mode: increment counter until overflow & disconnect timer from pins
    TCCR0B &= ~(1<<WGM02);
    TCCR0A &= ~((1<<WGM01) | (1<<WGM00) | (1<<COM0A0) | (1<<COM0A1));
    //the following code sets the prescaler depending on the system clock
    if ((F_CPU == 16000000UL) || (F_CPU == 8000000UL)) {   // prescaler set to 64
        TCCR0B &= ~(1<<CS02);
        TCCR0B |= ((1<<CS01) | (1<<CS00));
        prescaler = 64.0;
    } else if (F_CPU == 1000000UL) { // prescaler set to 8
        TCCR0B &= ~((1<<CS02) | (1<<CS00));
        TCCR0B |= (1<<CS01);
        prescaler = 8.0;
    }
#elif defined (ATMEGA8)
    //during setup, disable all the interrupts based on timer2
    TIMSK &= ~((1<<TOIE2) | (1<<OCIE2));
    //normal mode: counter incremented until overflow
    TCCR2 &= ~((1<<WGM21) | (1<<WGM20));
    //prescaler source clock set to internal Atmega clock (synch mode)
    ASSR &= ~(1<<AS2);

    if (F_CPU == 1600000UL) {	// prescaler set to 64
        TCCR2 |= (1<<CS22);
        TCCR2 &= ~((1<<CS21) | (1<<CS20));
        prescaler = 64.0;
    } else if ((F_CPU == 8000000UL) || (F_CPU == 4000000UL)) {	// prescaler set to 32
        TCCR2 &= ~(1<<CS22);
        TCCR2 |= ((1<<CS21) | (1<<CS20));
        prescaler = 32.0;
    } else if (F_CPU == 1000000L) { // prescaler set to 8
        TCCR2 |= (1<<CS21);
        TCCR2 &= ~((1<<CS22) | (1<<CS20));
        prescaler = 8.0;
    }
#elif defined (ATMEGAxU)
    //during setup, disable all the interrupts based on timer3
    TIMSK3 &= ~((1<<TOIE3) | (1<<OCIE3A) | (1<<OCIE3B) | (1<<OCIE3C) | (1<<ICIE3));
    //normal mode: counter incremented until overflow, prescaler set to /1
    TCCR3A &= ~((1<<WGM31) | (1<<WGM30));
    TCCR3B &= ~((1<<WGM33) | (1<<WGM32) | (1<<CS32) | (1<<CS31));
    TCCR3B |= (1<<CS30);
    //TCCR3B = 1;
    prescaler = 1.0;
#endif

    //set the initial value of the counter depending on the prescaler
#if defined (ATMEGAxU)
	_starter = 65536 - (uint16_t)((float)F_CPU * 0.001 / prescaler); //for 16 MHz: 49536
#else
    _starter = 256 - (int)((float)F_CPU * 0.001 / prescaler); //for 16 MHz: 6
#endif
    //SREG |= (1<<SREG_I);
}


//interrupt-driven routine to update the timer
#if defined (ATMEGAx8) || defined (ATMEGA8) || defined (ATMEGAx4) || defined (ATMEGAx0)
ISR(TIMER2_OVF_vect) {
    TCNT2 = _starter;
#elif defined (ATTINYx313)
ISR(TIMER0_OVF_vect) {
    TCNT0 = _starter;
#elif defined (ATTINYx4) || defined (ATTINYx5)
ISR (TIM0_OVF_vect) {
    TCNT0 = _starter;
#elif defined (ATMEGAxU)
ISR (TIMER3_OVF_vect) {
    TCNT3 = _starter;
#endif
	_counterT++;
	if (_counterT>999) { //1000 ms are 1 s
		_counterT=0;
		_secondsT++;
	}
}


// ***********************************
// ********* private methods *********
// ***********************************


//start the timer
void secTimer::startTimer() {
	SREG &= ~(1<<SREG_I); //halt all the interrupts
	setTimer(); //sets the timer
	_counterT = 0;
	_secondsT = 0;
	//starting value for the timer's counter
#if defined (ATMEGAx8) || defined (ATMEGAx4) || defined (ATMEGAx0)
    TCNT2 = _starter;
    TIMSK2 |= (1<<TOIE2);
#elif defined (ATMEGA8)
    TCNT2 = _starter;
    TIMSK |= (1<<TOIE2);
#elif defined (ATTINYx5) || defined (ATTINYx313)
    TCNT0 = _starter;
    TIMSK |= (1<<TOIE0);
#elif defined (ATTINYx4)
    TCNT0 = _starter;
    TIMSK0 |= (1<<TOIE0);
#elif defined (ATMEGAxU)
    TCNT3 = _starter;
	TIMSK3 |= (1<<TOIE3);
#endif
	SREG |= (1<<SREG_I); //reactivates global interrupts
	_isRunning = true;
}


//stop the timer
void secTimer::stopTimer() {
	SREG &= ~(1<<SREG_I); //halt all the interrupts
	//disable the timer's overflow interrupt
#if defined (ATMEGAx8) || defined (ATMEGAx4) || defined (ATMEGAx0)
    TIMSK2 &= ~(1<<TOIE2);
#elif defined (ATMEGA8)
    TIMSK &= ~(1<<TOIE2);
#elif defined (ATTINYx5) || defined (ATTINYx313)
    TIMSK &= ~(1<<TOIE0);
#elif defined (ATTINYx4)
    TIMSK0 &= ~(1<<TOIE0);
#elif defined (ATMEGAxU)
	TIMSK3 &= ~(1<<TOIE3);
#endif
	SREG |= (1<<SREG_I); //reactivates global interrupts
	_isRunning=false;
}


//returns the seconds
unsigned long secTimer::readTimer() {
	SREG &= ~(1<<SREG_I);
	unsigned long tempTimer = _secondsT;
	SREG |= (1<<SREG_I);
	return tempTimer;
}
