#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include <xc.h>
#include "dkp.h"


volatile IndicatorStatus prev_ind_status, cur_ind_status;  
// Global variables to store previous and current indicator states (used for state tracking)

uint16_t get_rpm()
{
    // Read analog value from ADC channel connected to RPM sensor (e.g., potentiometer)
    unsigned int value = read_adc(CHANNEL4);
    
    // Convert ADC value (0?1023) to RPM scale (approx mapping to 0?6000 RPM)
    return ((value / 10.23)*60);
}

unsigned char key,prev = SWITCH4;  
// 'key' stores current keypad input, 'prev' stores last valid key (initialized to OFF state)

IndicatorStatus process_indicator()
{
    // Read current key from digital keypad (LEVEL detection mode)
    key = read_dkp(LEVEL);

    // Update previous key only if a new valid key is pressed (not same and not idle 0x0F)
    if(key != prev && key != 0X0f)
    {
        prev = key;
    }

    // Map stored key value to corresponding indicator state
    if (prev == SWITCH1) 
    {
        return e_ind_left;     // Left indicator
    } 
    else if (prev == SWITCH2) 
    {
        return e_ind_right;    // Right indicator
    } 
    else if (prev == SWITCH3) 
    {
        return e_ind_hazard;   // Hazard indicator
    } 
    else if (prev == SWITCH4) 
    {
        return e_ind_off;      // Indicator OFF
    } 
}