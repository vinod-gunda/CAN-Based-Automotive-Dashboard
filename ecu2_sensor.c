#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"



uint16_t get_speed()
{
    // Read analog value from ADC channel (e.g., potentiometer representing speed)
    uint16_t value = read_adc(CHANNEL4); 

    // Convert ADC value (0?1023) to scaled speed range (approx 0?100)
    return ((value / 10.23));

    // Implement the speed function
}

unsigned char get_gear_pos()
{
    // Read key from digital keypad using state change detection (edge-based)
    unsigned char key = read_digital_keypad(STATE_CHANGE);

    // If Gear Up switch is pressed
    if(key == GEAR_UP )
    {
        return GEAR_UP ;      // Return Gear Up event
    }

    // If Gear Down switch is pressed
    else if(key == GEAR_DOWN)
    {
        return GEAR_DOWN;     // Return Gear Down event
    }

    // If no key is pressed
    else
    {
        return ALL_RELEASED;  // No gear action
    }  
}