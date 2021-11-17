#ifndef led_included
#define led_included

#define RED_LED BIT0        //0000 0001 P1.0
#define GREEN_LED BIT6      //0100 0000 P1.6
#define LEDS (BIT0 | BIT6)  //0100 0001

#define SW1 BIT0           //0000 0001 P2.0
#define SW2 BIT1           //0000 0010
#define SW3 BIT2           //0000 0100
#define SW4 BIT3           //0000 1000
#define SWITCHES 0xF       //1111

#endif
