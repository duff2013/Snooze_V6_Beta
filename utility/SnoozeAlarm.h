/***********************************************************************************
 *  SnoozeAlarm.h
 *  Teensy 3.x/LC
 *
 * Purpose: RTC Driver
 *
 ***********************************************************************************/
#ifndef SnoozeAlarm_h
#define SnoozeAlarm_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SnoozeAlarm : public SnoozeBlock {
private:
    virtual void disableDriver( void );
    virtual void enableDriver( void );
    virtual void clearIsrFlags( void );
    static void isr( void );
    //time_t rtc_set_sync_provider( void );
    void ( * return_rtc_irq ) ( void );
    time_t alarm;
    uint8_t return_priority;
    uint8_t return_isr_enabled;
    uint32_t TAR;
    uint32_t IER;
    bool SIM_SCGC6_clock_active;
public:
    SnoozeAlarm(void) : TAR( false ), IER( false ),
                  SIM_SCGC6_clock_active( false )
    {
        isDriver = true;
    }
    void setAlarm( uint8_t hours, uint8_t minutes, uint8_t seconds );
};
#endif /* defined(SnoozeAlarm_h) */
