/*******************************************************************************
 *  Snooze.cpp
 *  Teensy 3.x/LC
 *
 * Purpose:    Provides routines for configuring the Teensy for low power.
 *
 * NOTE:       None
 *******************************************************************************/
#ifndef Snooze_h
#define Snooze_h

/***************************************************************************/
#include "Arduino.h"
#include "SnoozeBlock.h"
#include "utility/SnoozeTimer.h"
#include "utility/SnoozeAlarm.h"
#include "utility/SnoozeTouch.h"
#include "utility/SnoozeCompare.h"
#include "utility/SnoozeDigital.h"
/***************************************************************************/
class SnoozeClass {
private:
    static volatile uint32_t PCR3;
public:
    SnoozeClass( void );
    static void idle ( SNOOZE_BLOCK );
    static int source( SNOOZE_BLOCK );
    /* sleep functions */
    static int sleep    (SNOOZE_BLOCK );
    static int deepSleep( SNOOZE_BLOCK, SLEEP_MODE mode = LLS );
    static int hibernate( SNOOZE_BLOCK,  SLEEP_MODE mode = LLS );
};
extern SnoozeClass Snooze;
/***************************************************************************/
#endif /* defined(Snooze_h) */
