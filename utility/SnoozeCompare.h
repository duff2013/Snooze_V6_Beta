/***********************************************************************************
 *  SnoozeCompare.h
 *  Teensy 3.x/LC
 *
 * Purpose: Compare Driver
 *
 ***********************************************************************************/

#ifndef SnoozeCompare_h
#define SnoozeCompare_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SnoozeCompare : public SnoozeBlock {
private:
    virtual void enableDriver( void );
    virtual void disableDriver( void );
    virtual void clearIsrFlags( void );
    static void isr( void );
    void ( * return_cmp0_irq ) ( void );
    uint8_t  return_priority_cmp0;
    float threshold_crossing;
    uint8_t pin;
    uint8_t type;
    volatile uint32_t return_core_pin_config[2];
public:
    SnoozeCompare(void) {
        isDriver = true;
    }
    void pinMode( int _pin, int _type, float val );
};
#endif /* defined(SnoozeCompare_h) */
