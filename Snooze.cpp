/*******************************************************************************
 *  Snooze.cpp
 *  Teensy 3.x/LC
 *
 * Purpose:    Provides routines for configuring the Teensy for low power.
 *
 * NOTE:       None
 *******************************************************************************/
#include "Snooze.h"
#include "SnoozeBlock.h"
#include "utility/wake.h"
#include "utility/sleep.h"
#include "utility/clocks.h"

volatile uint32_t SnoozeClass::PCR3 = 0;
/**
 *  SnoozeClass Constructor
 */
SnoozeClass::SnoozeClass( void ) {
    SIM_SOPT1CFG |= SIM_SOPT1CFG_USSWE;
    SIM_SOPT1 &= ~SIM_SOPT1_USBSSTBY;
}
/*******************************************************************************
 *  source - returns the wakeup source.
 *
 *  @return Digital pins return pin number, CMP - 34, RTC - 35, LPTMR - 36, TSI - 37.
 *******************************************************************************/
int SnoozeClass::source( SNOOZE_BLOCK ) {
    SnoozeBlock *p = &configuration;
    return p->source;
}
/*******************************************************************************
 *  idle - puts processor into wait mode until next interrupt typically systick.
 *******************************************************************************/
void SnoozeClass::idle( SNOOZE_BLOCK ) {
    SnoozeBlock *p = &configuration;
    p->mode = WAIT;
    wait( );
    p->mode = RUN;
}
/*******************************************************************************
 *  sleep - most versatile sleep mode. SnoozeBlock configuration or any interrupt 
 *          can wake the processor.
 *
 *  @param configuration SnoozeBlock class config.
 *
 *  @return wakeup source
 *******************************************************************************/
int SnoozeClass::sleep( SNOOZE_BLOCK ) {
    SnoozeBlock *p = &configuration;
    p->mode = VLPW;
    //p->mode = VLPS;
    p->enableDriver( );
    pee_blpi( );
    enter_vlpr( );
    vlpw( );
    exit_vlpr( );
    blpi_pee( );
    p->disableDriver( );
    p->mode = RUN;
    return p->source;
}
/*******************************************************************************
 *  deepSleep - LLWU is used to handle interrupts that wake. USB regulator is 
 *              enabled and 3.3V output pin can supply full current (100mA MAX).
 *
 *  @param configuration SnoozeBlock class config.
 *  @param mode          |LLS|VLLS3|VLLS2|VLLS1|
 *
 *  @return wakeup source
 *******************************************************************************/
int SnoozeClass::deepSleep( SNOOZE_BLOCK, SLEEP_MODE mode ) {
    SnoozeBlock *p = &configuration;
    p->mode = LLS;
    p->enableDriver( );
    llwu_set( );
    switch ( mode ) {
        case LLS:
            lls( );
            break;
        case VLLS3:
            vlls3( );
            break;
        case VLLS2:
            vlls2( );
            break;
        case VLLS1:
            vlls1( );
            break;
        case VLLS0:
            vlls0( );
            break;
        default:
            break;
    }
    p->source = llwu_disable( );
    p->disableDriver( );
    p->mode = RUN;
    return p->source;
}
/*******************************************************************************
 *  hibernate - LLWU is used to handle interrupts that wake. USB regulator is 
 *              disabled and 3.3V output pin can only supply limited current and 
 *              voltage drops to ~2.7V.
 *
 *  @param configuration SnoozeBlock class config.
 *  @param mode          |LLS|VLLS3|VLLS2|VLLS1|
 *
 *  @return wakeup source
 *******************************************************************************/
int SnoozeClass::hibernate( SNOOZE_BLOCK, SLEEP_MODE mode ) {
    SIM_SOPT1CFG |= SIM_SOPT1CFG_USSWE;
    SIM_SOPT1 |= SIM_SOPT1_USBSSTBY;
    PCR3 = PORTA_PCR3;
    PORTA_PCR3 = PORT_PCR_MUX( 0 );
    SnoozeBlock *p = &configuration;
    p->mode = mode;
    p->enableDriver( );
    llwu_set( );
    switch ( mode ) {
        case LLS:
            lls( );
            break;
        case VLLS3:
            vlls3( );
            break;
        case VLLS2:
            vlls2( );
            break;
        case VLLS1:
            vlls1( );
            break;
        case VLLS0:
            vlls0( );
            break;
        default:
            break;
    }
    p->source = llwu_disable( );
    p->disableDriver( );
    p->mode = RUN;
    SIM_SOPT1CFG |= SIM_SOPT1CFG_USSWE;
    SIM_SOPT1 &= ~SIM_SOPT1_USBSSTBY;
    PORTA_PCR3 = PCR3;
    return p->source;
}