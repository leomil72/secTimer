**************
** secTimer **
**************

Simple seconds counter for Arduino boards and Atmel microcontrollers.
The library can count from 0 to (2^32)-1 seconds. This is a really HUGE
interval of time since (2^32)-1 seconds are about 136 years! So your
device will cease to function before the library will overflow!

********************************************************************
VERSION HISTORY

2013/09/05 - v. 0.4.1:	fixed a severe bug - now the library computes the correct time again
2012/11/29 - v. 0.4:	added support for new MCUs, new clock speed and bug fixing
2012/11/27 - v. 0.3:	fixed a bug that affected interrupts after using stopTimer()
2012/04/18 - v. 0.2:	name changed
2012/04/18 - v. 0.1:	first release

********************************************************************
REQUIREMENTS

* The secTimer library needs Arduino IDE version 002x or 1.0x to work with Atmega48/88/168/328/328P, Atmega640/1280/1281/2560/2561 and Atmega8/8A.
* To work with Attiny24/44/84, Attiny25/45/85, and Attiny2313/4313 you need the Tiny Core libraries that you can download from:
http://code.google.com/p/arduino-tiny/
* To work with Atmega344/644/1284/P you need a speciale core that you can download from:
http://www.leonardomiliani.com/?page_id=374

Directions to install and use the Tiny and 344/644/1284 cores are provided from their respective authors.

********************************************************************
INSTALLATION

1) Uncompress the archive and copy the secTimer folder into the "libraries" folder that is inside your sketchbooks' folders. If the "libraries" folder doesn't exist, you have to create it.
To know where the sketchbooks folder is located, open the Arduino IDE, then select "Preferences" from the menu "File" and look at the path in "Sketchbook location".
At the end, you should have something similar to this (for Linux): /home/your_username/sketchbook/libraries/secTimer

2) After that, you have to replace a file of the Tiny core. Make a backup copy in case you need to roll back to the original one.
To do this, locate the file named core_build_options.h that is located in your-arduino-IDE-folder/hardware/tiny/cores/tiny/ and rename it to core_build_options.bck (or a name you like).
Then, copy the core_build_options.h that comes from the swRTC into the previous path.
This will permit to have both the secTimer library and the delay/millis functions from Arduino IDE up and running on Tinyx5/x4/2313 microcontrollers.

********************************************************************
USAGE & METHODS

Include the library in your sketch with
#include <secTimer.h>

To use the library, first you have to create an instance of the library, i.e.:
secTimer myTimer;

Then you can start the timer by giving the following command:
myTimer.startTimer();

To stop the timer, simply give the following command:
myTimer.stopTimer();
These methods are useful i.e. if you have to do for a while some stuff with the hardware timer used by the Timer library, so you can stop the timer of the library, use the hardware timer for you needs and then restart the seconds counter.

To get the numbers of seconds since the timer has been started give the following command:
myTimer.readTimer();
It will return an unsigned long that you can store and/or use in your own computations.

********************************************************************
SUPPORTED FREQUENCIES, MICROS AND LIMITATIONS

The library is interrupt driven and uses an internal hardware timer.
This are the pins that you can use for PWM output:

1) Atmega48/88/168/328/P (Arduino UNO/2009): timer 2 - pins 5 & 17 (pins D3 & D11 on Arduino UNO)
2) Atmega344/644/1284/P: timer 2 - pins 20 & 21 (pins D14 & D15 on Maniacbug core)
3) Atmega640/1280/1281/2560/2561: timer 2 - pins 18 & 23 (pins D9 & D10 an Arduino MEGA)
4) Attiny25/45/85: timer 0 - pins 5 & 6 (pins D0 & D1 on Tiny core)
5) Attiny2313/4313: timer 0 - pins 9 & 14 (pins D7 & D11 on Tiny core)
6) Attiny24/44/84: timer 0 - pins 5 & 6 (pins D2 & D3 on Tiny core)
7) Atmega8/A: timer 2 - pins 17 (pin D11 on older Arduino boards)
8) Atmega32U4 (Arduino Leonardo): timer 3 - pin 32 (pin D5 on Arduino Leonardo)

** I M P O R T A N T **:
At the moment, the library only supports frequencies of 1, 4, 8, and 16 MHz due to float rounding problems.

********************************************************************
HOW IT WORKS

The library sets an internal timer in counter mode to have 1 overflow interrupt exactly every 1 millisecond.
Every 1000 ms it increments an internal register to keep the running of the seconds.

To do this, is calculates the correct starting value of the counter and the prescaler of the timer, depending on the system frequency. It also chooses the correct timer because this trick only works with 8-bit counters. On Attiny micros, the library must move the delay/millis functions on timer 1 because these micros have only 2 timers, and the 8-bit timer is timer 0.

********************************************************************
LICENCE

This library is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

********************************************************************
AUTHOR

Written by Leonardo Miliani <leonard AT leonardomiliani DOT com>

********************************************************************
Document revision: 4th revision - 2013/09/05

********************************************************************
