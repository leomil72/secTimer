/* This file is part of the secTimer library.
   Please check the README file and the notes
   inside the secTimer.h file to get more info
   
   This example will blink the led on pin 13 every second.
   
   Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 3.0 of the License, or (at your option) any later version.
*/

//include the library
#include <secTimer.h>

//create a new istance of the library
secTimer myTimer;

//variable definitions
const byte LED_PIN=13; //change this to wathever you want
byte state=1; //state of the LED
unsigned long seconds=0; //seconds passed after the sketch has started


//setup routine
void setup() {
    myTimer.startTimer(); //start the timer
    pinMode(LED_PIN, OUTPUT); //LED pin set to output
    digitalWrite(LED_PIN, state); //write the initial state of the LED
    seconds=myTimer.readTimer();
}


//loop routine
void loop() {
    //every second change the LED state
    if (seconds!=myTimer.readTimer()) {
        seconds=myTimer.readTimer();
        state^=1;
        digitalWrite(LED_PIN, state);
    }
    //here the sketch can do something else
    //
    //
}
