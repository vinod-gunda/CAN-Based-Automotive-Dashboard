/*
 * File:   ECU1_main.c
 * Author: ASUS
 *
 * Created on 31 March, 2026, 11:33 AM
 */

#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "dkp.h"
#include <xc.h>


void init_config() 
{
    // Initialize CAN peripheral (bit timing, mode, buffers)
    init_can();        
    // Initialize ADC for RPM sensing
    init_adc(); 
    // Initialize digital keypad for indicator input
    init_dkp();        
}


// Stores previous indicator state
volatile IndicatorStatus prev_ind_status = e_ind_off;  
// Stores current indicator state
volatile IndicatorStatus cur_ind_status = e_ind_off;   


char* my_copy(char *str2,char *str1)
{
    int i=0;
    while(str1[i])     // Copy until NULL terminator
    {
        str2[i]=str1[i];  // Copy character-by-character
        i++;
    }
    str2[i]='\0';      // Append NULL terminator
    return str2;       // Return destination string pointer
}


int main() 
{
    // Buffers for RPM (ASCII)
    unsigned char digit[5] = {0}; 
    // Stores RPM value (numeric)
    uint16_t value;  
    // Indicator states encoded as ASCII strings
    unsigned char *ind_arr[] = {"1","2","3","4"}; 
    // Index to select indicator state
    unsigned char indicator = 1;   
    // Initialize all peripherals
    init_config();         
    while (1) 
    {
        // Read RPM from ADC and convert to scaled value
        value = get_rpm();   

        // Convert numeric RPM to ASCII digits
        digit[0] = (value / 1000) + '0';
        digit[1] = ((value / 100) % 10) + '0';
        digit[2] = ((value / 10) % 10) + '0';
        digit[3] = (value % 10) + '0';
        digit[4] = '\0';     // Null terminate string

        can_transmit(RPM_MSG_ID, digit, 4); // Send RPM data over CAN (4 bytes)
        for(int i = 3000;i--;);             // Software delay to control transmission rate
        
        cur_ind_status = process_indicator(); // Read keypad and determine indicator state

        if(cur_ind_status == e_ind_off)
        {   
            indicator = 0;   // Map OFF state to index 0 ("1")
        }
        else if(cur_ind_status == e_ind_left)
        {
            indicator = 1;   // Map LEFT state to index 1 ("2")
        }
        else if(cur_ind_status == e_ind_right)
        {
            indicator = 2;   // Map RIGHT state to index 2 ("3")
        }
        else if(cur_ind_status == e_ind_hazard)
        {
            indicator = 3;   // Map HAZARD state to index 3 ("4")
        }  

        can_transmit(INDICATOR_MSG_ID,ind_arr[indicator],1); // Send indicator status (1 byte)
        for(int i = 3000;i--;);                              // Delay to avoid bus flooding
    }
}