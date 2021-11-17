#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"

void switch_init () {
  P2REN |= SWITCHES;          // Enables resistors for switches
  P2IE |= SWITCHES;           // Enable interrupts from switches
  P2OUT |= SWITCHES;          // Pull-ups for switches
  P2DIR &= ~SWITCHES;         // Switch bits to input
}

void led_init () {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void wdt_init () {
  configureClocks ();
  enableWDTInterrupts ();
}

int main (void) {
  WDTCTL = WDTPW | WDTHOLD;
  
  led_init ();
  switch_init ();
  buzzer_init ();
  wdt_init ();
  
  or_sr (0x18); //CPU off, GIE on
}

int seconds = 0;
static int sw1Down;
static int sw2Down;
static int sw3Down;
static int sw4Down;

void switch_interrupt_handler () {
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);

  sw1Down = (p2val & SW1) ? 0 : 1;
  sw2Down = (p2val & SW2) ? 0 : 1;
  sw3Down = (p2val & SW3) ? 0 : 1;
  sw4Down = (p2val & SW4) ? 0 : 1;
}

void __interrupt_vec (PORT2_VECTOR) Port_2 () {
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler ();
  }
}

void tone_generator (int state) {
  if (state == 0) {
    buzzer_set_period (6810.5973);
    
  } else if (state == 1) {
    buzzer_set_period (6067.4089);
    
  } else if (state == 2) {
    buzzer_set_period (5405.5515);
    
  } else if (state == 3) {
    buzzer_set_period (5102.0408);
    
  } else if (state == 4) {
    buzzer_set_period (4545.4545);
  }
}

void state1 () {
  P1OUT |= RED_LED;
  buzzer_set_period (4545.4545);
}

void state2 () {
  P1OUT |= GREEN_LED;
  buzzer_set_period (2702.7027);
}

void state3 () {
  if (seconds % 124 <= 62) { // Loop every 124 counts; execute outer ifs half of the time
    P1OUT &= ~GREEN_LED;    
    
    if (seconds % 5 == 0) {
      P1OUT |= RED_LED;
      
    } else {
      P1OUT &= ~RED_LED;
    }
  } else {
    P1OUT &= ~RED_LED;
    
    if (seconds % 5 == 0) {
      P1OUT |= GREEN_LED;
      
    } else {
      P1OUT &= ~GREEN_LED;
    }
  }
  seconds++;
  if (seconds >= 496) {
    seconds = 0;
  }

  if (seconds == 0) {
    tone_generator (0);

  } else if (seconds == 63 || seconds == 435) {
    tone_generator (1);

  } else if (seconds == 125 || seconds == 373) {
    tone_generator (2);

  } else if (seconds == 187 || seconds == 311) {
    tone_generator (3);

  } else if (seconds == 249) {
    tone_generator (4);

  }
}

void state4 () {
  
}

void __interrupt_vec (WDT_VECTOR) WDT () { // 250 interrupts/sec
  if (sw1Down) {
    state1 ();
    
  } else if (sw2Down) {
    state2 ();
    
  } else if (sw3Down) {
    state3 ();
    
  } else if (sw4Down) {
    state4 ();
    
  } else {
    P1OUT &= ~LEDS;
    buzzer_set_period (0);
    seconds = 0;
  }
}
