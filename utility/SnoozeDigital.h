/***********************************************************************************
 * Low Power Library for Teensy LC/3.x
 * Copyright (c) 2016, Colin Duffy https://github.com/duff2013
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ************************************************************************************
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
    uint8_t  irqType[CORE_NUM_INTERRUPT];
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
    static uint32_t isr_pin;
    uint8_t  irqType[CORE_NUM_INTERRUPT];
    void ( * return_porta_irq ) ( void );
    void ( * return_portcd_irq )( void );
    uint32_t return_isr_a_enabled;
    uint32_t return_isr_cd_enabled;
    uint8_t  return_priority_a;
    uint8_t  return_priority_cd;
#endif
    volatile uint32_t return_core_pin_config[CORE_NUM_INTERRUPT];
public:
    SnoozeDigital( void ) {
        isDriver = true;
    }
    int pinMode( int _pin, int mode, int type );
};
#endif /* defined(SnoozeDigital_h) */
