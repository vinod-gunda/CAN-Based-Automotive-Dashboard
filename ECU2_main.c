/*
 * File:   ECU1_main.c
 * Author: ASUS
 *
 * Created on 31 March, 2026, 11:33 AM
 */

#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"


void init_config() {
    init_can();               // Initialize CAN peripheral (bit timing, buffers, mode)
    init_adc();               // Initialize ADC for speed input
    init_digital_keypad();    // Initialize digital keypad for gear control
}


char* my_copy(char *str2,char *str1)
{
    int i=0;
    while(str1[i])            // Copy source string until NULL terminator
    {
        str2[i]=str1[i];      // Copy each character
        i++;
    }
    str2[i]='\0';             // Append NULL terminator to destination
    return str2;              // Return destination pointer
}


int main() 
{
    unsigned char *gear_arr[] = {"N","1","2","3","4","5","R"}; // Gear positions mapped to ASCII
    static unsigned char gear = 0;   // Current gear index (0 = Neutral)
    unsigned char digit[4] = {0};    // Buffer to store speed as ASCII string
    uint16_t value;                 // Variable to store speed value
    init_config();                  // Initialize peripherals

    while (1) 
    {  
        value = get_speed();        // Read speed from ADC and scale it

        unsigned char key = get_gear_pos(); // Read gear change input from keypad
        
        // Increment gear if Gear Up key is pressed and within limit
        if(key == GEAR_UP)
        {
            if(gear < 6)
            gear++;
        }
        // Decrement gear if Gear Down key is pressed and above Neutral
        else if(key == GEAR_DOWN)
        {
            if(gear > 0)
            gear--;
        }
        
        // If gear is Neutral, force speed to 0
        if(gear == 0)
        {
            value = 0;
        }

        // Transmit gear information over CAN (1 byte ASCII)
        can_transmit(GEAR_MSG_ID,gear_arr[gear], 1);
        for(int i = 3000;i--;);     // Delay to control CAN transmission rate
       
        // Convert numeric speed to ASCII (3-digit representation)
        digit[0] = (value / 100) + '0';
        digit[1] = ((value / 10) % 10) + '0';
        digit[2] = ((value) % 10) + '0';
        digit[3] = '\0';            // Null terminate string

        // Transmit speed data over CAN (3 bytes)
        can_transmit(SPEED_MSG_ID, digit, 3);
        for(int i = 3000;i--;);     // Delay to avoid CAN bus flooding
    }
}

