/*
 * File:   relayonpress.c
 * Author: Benoit
 *
 * Created on 5/02/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "header.h"

volatile uint16_t tick_count;
uint16_t tmp;
   
void InitTimer0(void)
{
    // Timer0 is a 8bit timer, select T0CS and PSA to be one
    OPTION_REG &=0xC1; //Make Prescaler 1:4
    
    T0IE = 1;
    GIE = 1;
}

// Initialization
void main(void) {
   ANSEL = 0; // no analog GPIOs
   CMCON = 0x07; // comparator off
   ADCON0 = 0; // Analog to Digital and Digital to Analog convertors off
   TRISIO0 = 0; // output LED
   TRISIO1 = 1; // input footswtich
   TRISIO2 = 0; // output TGP222A
   TRISIO5 = 0; // output activation relay
   TRISIO4 = 0; // output ground relay
   TRISIO3 = 1; // input temporary switch

   GPIO = 0; // set outputs as low level (0V)
   
   tick_count = 0;
   
   tmp = 0;
   
   // Variables definition
   uint8_t state; // on-off state of the pedal (1 = on, 0 = off)
   state = 0;

   uint8_t temporary; // define the mode of the pedal : classic of temporary activation
   temporary = 0;

   uint8_t press_switch; // variable to detect if the switch is pressed
   press_switch = 0;

   uint8_t change_state; // to change status of the pedal
    if( eeprom_read(0) == 0xFF ) { // Read from 0 address location
      change_state = 1;			// If previous state was on
    }
    else {
      change_state = 0;			// If previous state was off
    }

   InitTimer0();
   
   // Main loop
   while(1) {
       if(tick_count == 1500) {
           tick_count = 0;
           ++tmp;
           GP0 = ~GP0;
       }
       
       
//       if(GP3 == 0) { // By default: normal mode
//          temporary = 0;
//      }
//      else { // If the "hold" mode is activated through the switch
//          temporary = 1;
//      }
//
//       // Normal mode
//       if(temporary == 0) {
//           if(GP1 == 0) { // footswitch pressed
//               __delay_ms(15); // debouncing
//               if(GP1 == 0) {
//                  press_switch = press_switch + 1; // switch is on
//                  if(press_switch>10) {
//                      press_switch = 10; // max value for press_switch
//                  }
//               }
//           }
//           if(press_switch == 1) { // switch is pressed : lets turn the pedal on or off
//               change_state = 1;
//               press_switch = 2; // avoid bugs if press_switch stays at 1
//           }
//           if(GP1 == 1) {
//               __delay_ms(15); // debouncing
//               if(GP1 == 1) {
//                  press_switch = 0;
//               }
//           }
//       }
//
//       // Temporary mode
//       if(temporary == 1) {
//           if(GP1 == 0) { // if switch is pressed : temporary activation
//               __delay_ms(15); // debouncing
//               if(GP1 == 0) {
//                  press_switch = press_switch + 1; // switch is on
//                  if(press_switch>10) {
//                      press_switch = 10; // max value for press_switch
//                  }
//               }
//           }
//           if(GP1 == 1) { // if switch is not pressed, turn it off
//               __delay_ms(15); // debouncing
//               if(GP1 == 1) {
//                   state = 0;
//                   press_switch = 0;
//               }
//           }
//           if(press_switch == 1) {
//               change_state = 1; // if switch is pressed, turn the pedal on
//               press_switch = 2; // avoids bug
//           }
//       }
//
//       // Changing state of the pedal
//       if(change_state == 1) {
//           if(state == 0) { // change to on
//               GP2 = 1; // photoFET on
//               __delay_ms(20);
//               GP0 = 1; // LED on
//               GP5 = 1; // relay on
//               GP4 = 0;
//               __delay_ms(20);
//               GP2 = 0; // photoFET off
//               state = 1;
//               eeprom_write(0, 0xFF);		// Write 0xFF at 0 address location
//           }
//          else { // change to off
//               GP2 = 1;
//               __delay_ms(20);
//               GP0 = 0; // LED off
//               GP5 = 0; // relay off
//               GP4 = 0;
//               __delay_ms(20);
//               GP2 = 0;
//               state = 0;
//               eeprom_write(0, 0x00);		// Write 0x00 at 0 address location
//           }
//          __delay_ms(10);
//          change_state=0; // reset change_state
//       }
//
//       // To let the pedal in the good state (on or off)
//       if (state == 1) { // effect on
//            GP0 = 1; // LED on
//            GP5 = 1; // relay on
//            GP4 = 0;
//       }
//        else { // effect off
//            GP0 = 0; // LED off
//            GP5 = 0; // relay off
//            GP4 = 0;
//       }
   }
}

void interrupt ISR(void)
{
    if(T0IF) // If Timer0 interrupt
    {
        ++tick_count;
        T0IF = 0; // Clear the interrupt
    }
}
