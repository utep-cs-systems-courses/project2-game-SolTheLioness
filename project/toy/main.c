#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"
#include "state4.s"

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

static int state = 0;
static int count = 0;
static int substate = -1;

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

extern void state4AS (int count, int state);

void state1 () {
  if (count % 6 == 0) {
    P1OUT |= RED_LED;
    
  } else {
    P1OUT &= ~LEDS;
  }
  buzzer_set_period (4545.4545);
  count++;
  if (count >= 7) {
    count = 0;
  }
}

void state2 () {
  P1OUT |= GREEN_LED;
  buzzer_set_period (2702.7027);
}

void cal_ss (int dir) {
  if (count % 125 == 0 || count == 0) {
    if (dir == 0) {
      substate++;
    } else {
      substate--;
      if (substate == 0 && dir == 1) {
	count = 1;
	substate--;
	}
    }
    tone_generator (substate);
  }
}

void state3 (int dir) {
  cal_ss (dir);
  if (dir == 0) { // Direction 0, incrementing
    if (count % 5 == 0) {
      P1OUT |= RED_LED;
      
    } else {
      P1OUT &= ~LEDS;
    }
    count++;
  }

  else { // Direction 1, decrementing
    P1OUT |= GREEN_LED;
    count--;
  }
  
}

void state4 () {
  state4AS (count, state);
  /*if (count == 0) {
    if (state == 0) {
      P1OUT |= 1;
      buzzer_set_period (5405.5515);
    } else if (state == 1) {
      P1OUT |= 1000000;
      buzzer_set_period (7644.3833);
    }
    }*/
  
  count++;
  if (count >= 251) {
    P1OUT &= ~1000001;
    count = 0;
    state ^= 1;
  }
}

void __interrupt_vec (WDT_VECTOR) WDT () { // 250 interrupts/sec
  if (sw1Down) {
    state1 ();
    
  } else if (sw2Down) {
    state2 ();
    
  } else if (sw3Down) {
    if (count <= 0) {
      state = 0;
      P1OUT &= ~GREEN_LED;
    }
    if (count >= 625) {
      state = 1;
      P1OUT &= ~RED_LED;
    }
    state3 (state);
    
  } else if (sw4Down) {
    state4 ();
    
  } else {
    P1OUT &= ~LEDS;
    buzzer_set_period (0);
    count = 0;
    state = 0;
    substate = -1;
  }
}
