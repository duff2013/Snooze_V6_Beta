/***********************************************************************************
 *  SnoozeTimer.h
 *  Teensy 3.x/LC
 *
 * Purpose: Touch (TSI) Driver
 *
 ***********************************************************************************/
#ifndef SnoozeTouch_h
#define SnoozeTouch_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SnoozeTouch : public SnoozeBlock {
private:
    virtual void enableDriver( void );
    virtual void disableDriver( void );
    virtual void clearIsrFlags( void );
    static void isr( void );
    void ( * return_tsi0_irq ) ( void );
    uint8_t  pin;
    uint16_t threshold;
    uint32_t GENCS;
    uint32_t SCANC;
    uint32_t PEN;
    uint32_t THRESHOLD;
    //uint8_t return_priority;
    //uint8_t return_isr_enabled;
    bool SIM_SCGC5_clock_active;
    volatile uint32_t return_core_pin_config;
    //bool OSC_clock_active;
public:
    SnoozeTouch( void ) : pin( 0x03 ) {
        isDriver = true;
    }
    void pinMode( int _pin, int thresh );
};
#endif /* defined(SnoozeTouch_h) */
