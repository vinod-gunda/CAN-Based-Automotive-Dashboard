#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"




volatile unsigned char led_state = LED_OFF, status = e_ind_off; 
// Stores LED state and indicator status (used for control/logic)

uint16_t msg_id;  
// Stores received CAN message ID

uint8_t data_length, recv_data[6], prev[] = "OFF";  
// data_length ? number of bytes received
// recv_data ? buffer to store received CAN data
// prev ? stores previous indicator string (unused here)

extern unsigned char toggle;  
// External variable (likely used for blinking logic via timer)

static uint8_t current_indicator[4] = "OFF";  
// Stores current indicator state (string form)


void handle_speed_data(uint8_t *data, uint8_t len) {
    // Display received speed data on CLCD at LINE2 position 8
    clcd_print(data, LINE2(8));
}

void handle_gear_data(uint8_t *data, uint8_t len) {
    // Display received gear data on CLCD at LINE2 position 12
    clcd_print(data, LINE2(12));
}

void handle_rpm_data(uint8_t *data, uint8_t len) {
    // Display received RPM data on CLCD at LINE2 position 0
    clcd_print(data, LINE2(0));

    // Delay to slow down display update (introduces blocking)
    __delay_ms(500);
    
}

void handle_indicator_data(uint8_t *data, uint8_t len) 
{
    // Display raw indicator data (ASCII: '1','2','3','4')
    clcd_print(data, LINE2(4));
    
    // Decode indicator state based on received value
    if(data[0] == '1')
    {
        RB0 = 0, RB7 = 0;               // Turn OFF both indicator LEDs
        clcd_print("OFF", LINE2(4));    // Display OFF
    }
    else if(data[0] == '2')
    {
        RB0 = 1;                        // LEFT indicator ON
        RB7 = 0;
        clcd_print("LEF", LINE2(4));    // Display LEFT
    }
    else if(data[0] == '3')
    {
        RB0 = 0;
        RB7 = 1;                        // RIGHT indicator ON
        clcd_print("RIG", LINE2(4));    // Display RIGHT
    }
    else if(data[0] == '4')
    {
        RB0 = RB7 = 1;                  // Both indicators ON (Hazard)
        clcd_print("HAZ", LINE2(4));    // Display HAZARD
    }
}

void process_canbus_data() {
    // Receive CAN frame (fills msg_id, recv_data, data_length)
    can_receive(&msg_id, recv_data, &data_length);

    // Process only if valid data received
    if (data_length != 0) 
    {
        recv_data[data_length] = '\0';  // Null-terminate received data for string handling

        // Route message based on CAN message ID
        switch (msg_id) 
        {
            case SPEED_MSG_ID:
            {
                handle_speed_data(recv_data, data_length); // Handle speed data
                break;
            }
            case GEAR_MSG_ID:
            {
                handle_gear_data(recv_data, data_length);  // Handle gear data
                break;
            }
            case RPM_MSG_ID:
            {
                handle_rpm_data(recv_data, data_length);   // Handle RPM data
                break;
            }
            case INDICATOR_MSG_ID:
            {
                handle_indicator_data(recv_data, data_length); // Handle indicator data
                break;
            }
        }
    }
}