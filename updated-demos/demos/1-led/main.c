//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR = 0b01000001;
  P1OUT = 0b00000001;
  while (1) {
    P1OUT ^= 0b01000001;
    __delay_cycles (50000);
  }
  //P1OUT &= ~LED_GREEN;
  //P1OUT |= LED_RED;

  or_sr(0x18);		/* CPU off, GIE on */
}
