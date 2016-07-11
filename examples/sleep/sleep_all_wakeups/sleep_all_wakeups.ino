/***************************************
 * This shows all the wakeups for sleep
 * Expect IDD of  around 1.2mA (Teensy 3.x)
 * and IDD of around 900uA for (Teensy LC).
 *
 * Sleep is the most flexable and any
 * interrupt can wake the processor.
 ****************************************/
#include <Snooze.h>
// Load drivers
SnoozeDigital digital;
SnoozeCompare compare;
SnoozeTimer timer;
SnoozeTouch touch;
#ifdef KINETISK
SnoozeAlarm	alarm;
#endif

/***********************************************************
 * Teensy 3.5/LC can't use Timer Driver with either Touch or
 * Compare Drivers and Touch can't be used with Compare.
 ***********************************************************/
#ifdef KINETISK
SnoozeBlock config_teensy35(touch, digital, alarm);
#endif
SnoozeBlock config_teensyLC(compare, digital, timer);
/***********************************************************
 * Teensy 3.2 can use any Core Drivers together.
 ***********************************************************/
SnoozeBlock config_teensy32(touch, compare, digital, timer);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    /********************************************************
     * Define digital pins for waking the teensy up. This
     * combines pinMode and attachInterrupt in one function.
     *
     * Teensy 3.x
     * Digtal pins: 2,4,6,7,9,10,11,13,16,21,22,26,30,33
     *
     * Teensy LC
     * Digtal pins: 2,6,7,9,10,11,16,21,22
     ********************************************************/
    digital.pinMode(21, INPUT_PULLUP, RISING);//pin, mode, type
    digital.pinMode(22, INPUT_PULLUP, RISING);//pin, mode, type
    
    /********************************************************
     * Teensy 3.x only currently.
     *
     * Set RTC alarm wake up in (hours, minutes, seconds).
     ********************************************************/
#ifdef KINETISK
    alarm.setAlarm(0, 0, 10);// hour, min, sec
#endif
    /********************************************************
     * Set Low Power Timer wake up in milliseconds.
     ********************************************************/
    timer.setTimer(5000);// milliseconds
    
    /********************************************************
     * Values greater or less than threshold will trigger CMP
     * wakeup. Threshold value is in volts (0-3.3v) using a 64
     * tap resistor ladder network at 0.0515625v per tap.
     *
     * parameter "type": LOW & FALLING are the same.
     * parameter "type": HIGH & RISING are the same.
     *
     * Teensy 3.x/LC
     * Compare pins: 11,12
     ********************************************************/
    // trigger at threshold values greater than 1.65v
    //config.pinMode(11, CMP, HIGH, 1.65);//pin, mode, type, threshold(v)
    // trigger at threshold values less than 1.65v
    compare.pinMode(11, LOW, 1.65);//pin, mode, type, threshold(v)
    
    /********************************************************
     * Values greater than threshold will trigger TSI wakeup.
     * Threshold value is in capacitance. Only one pin can be
     * used while sleeping.
     *
     * Teensy 3.x
     * Touch Sense pins: 0,1,15,16,17,18,19,22,23,25,32,33
     *
     * Teensy LC
     * Touch Sense pins: 0,1,3,4,15,16,17,18,19,22,23
     ********************************************************/
    touch.pinMode(0, touchRead(0) + 250); // pin, threshold
}

void loop() {
    int who;
    /********************************************************
     * feed the sleep function its wakeup parameters. Then go
     * to deepSleep.
     ********************************************************/
#if defined(__MK66FX1M0__)
    who = Snooze.sleep( config_teensy35 );// return module that woke processor
#elif defined(__MK20DX256__)
    who = Snooze.sleep( config_teensy32 );// return module that woke processor
#elif defined(__MKL26Z64__)
    who = Snooze.sleep( config_teensyLC );// return module that woke processor
#endif
    
    if (who == 21) { // pin wakeup source is its pin value
        for (int i = 0; i < 1; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    if (who == 22) { // pin wakeup source is its pin value
        for (int i = 0; i < 2; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    if (who == 34) { // compare wakeup value
        for (int i = 0; i < 3; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    if (who == 35) { // rtc wakeup value
        for (int i = 0; i < 4; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    if (who == 36) { // lptmr wakeup value
        for (int i = 0; i < 5; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
    
    if (who == 37) { // tsi wakeup value
        for (int i = 0; i < 6; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    }
}