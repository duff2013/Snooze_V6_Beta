/***********************************************************************************
 *  SnoozeDigital.h
 *  Teensy 3.x/LC
 *
 * Purpose: Digital Pin Driver
 *
 ***********************************************************************************/
#ifndef SnoozeDigital_h
#define SnoozeDigital_h

#include "Arduino.h"
#include "SnoozeBlock.h"

class SnoozeDigital : public SnoozeBlock {
private:
    virtual void enableDriver( void );
    virtual void disableDriver( void );
    virtual void clearIsrFlags( void );
    static void isr( void );
    static void attachDigitalInterrupt( uint8_t pin, int mode );
    static void detachDigitalInterrupt( uint8_t pin );
#if defined(KINETISK)
    uint64_t pin;
    static uint64_t isr_pin;
    uint8_t  irqType[33];
    void ( * return_porta_irq ) ( void );
    void ( * return_portb_irq ) ( void );
    void ( * return_portc_irq ) ( void );
    void ( * return_portd_irq ) ( void );
    void ( * return_porte_irq ) ( void );
    uint32_t return_isr_a_enabled;
    uint32_t return_isr_b_enabled;
    uint32_t return_isr_c_enabled;
    uint32_t return_isr_d_enabled;
    uint32_t return_isr_e_enabled;
    uint8_t  return_priority_a;
    uint8_t  return_priority_b;
    uint8_t  return_priority_c;
    uint8_t  return_priority_d;
    uint8_t  return_priority_e;
#elif defined(KINETISL)
    uint32_t pin;
    uint8_t  irqType[23];
    void ( * return_porta_irq ) ( void );
    void ( * return_portcd_irq )( void );
    uint32_t return_isr_a_enabled;
    uint32_t return_isr_cd_enabled;
    uint8_t  return_priority_a;
    uint8_t  return_priority_cd;
#endif
    volatile uint32_t return_core_pin_config[CORE_NUM_TOTAL_PINS];
public:
    SnoozeDigital(void) {
        isDriver = true;
    }
    int pinMode( int _pin, int mode, int type );
};
#endif /* defined(SnoozeDigital_h) */
