/***********************************************************************************
 *  SnoozeTimer.h
 *  Teensy 3.x/LC
 *
 * Purpose: Low Power Timer Driver
 *
 ***********************************************************************************/
#ifndef SnoozeTimer_h
#define SnoozeTimer_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SnoozeTimer : public SnoozeBlock {
private:
    static void ( * return_lptmr_irq ) ( void );
    uint8_t return_priority;
    uint8_t return_isr_enabled;
    virtual void disableDriver( void );
    virtual void enableDriver( void );
    virtual void clearIsrFlags( void );
    static void isr( void );
    uint16_t period;
    uint32_t PSR;
    uint32_t CMR;
    uint32_t CSR;
    bool SIM_SCGC5_clock_active;
    bool OSC_clock_active;
public:
    SnoozeTimer(void) : PSR( 0 ), CMR( 0 ), CSR( 0 ),
                  SIM_SCGC5_clock_active(false ),
                  OSC_clock_active( false )
    {
        isDriver = true;
    }
    void setTimer( uint16_t newPeriod );
};
#endif /* defined(SnoozeTimer_h) */
