/* 
 * File:   dash_board_main.c
 * Author: ASUS
 *
 * Created on 2 April, 2026, 12:31 PM
 */

#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "message_handler.h"

static void  init_leds() 
{
    TRISB = 0x08; // Set RB2 as output, RB3 as input, remaining as output
    PORTB = 0x00;
    
}

static void init_config(void) {
    // Initialize CLCD and CANBUS
    init_clcd();
    init_can();
    init_leds();
    // Enable Interrupts
    PEIE = 1;
    GIE = 1;
}

void main(void) {
    // Initialize peripherals
    init_config();
    CLEAR_DISP_SCREEN;
    clcd_print("RPM IND SPD GEAR",LINE1(0));
    while (1) 
    {
        // Read CAN Bus data and handle it
        process_canbus_data();
    }

    return;
}

