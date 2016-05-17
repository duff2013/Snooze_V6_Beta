# Snooze V6.0.0 - Beta1

<h4>The new api allows me to add new functionality and Teensy much easier. This is still beta so it might be broken at times. I'll update here once it becomes stable.<br>

![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide1.png "Snooze Class Layout")
<br>
Basic Example:
```c++
#include <Snooze.h>
// Load drivers
SnoozeTimer timer;

// install driver to a configuration block
SnoozeBlock config(timer);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    /********************************************************
     * Set Low Power Timer wake up in milliseconds.
     ********************************************************/
    timer.setTimer(5000);// milliseconds
}

void loop() {
    /********************************************************
     * feed the sleep function its wakeup parameters. Then go 
     * to deepSleep.
     ********************************************************/
    int who = Snooze.deepSleep( config );// return module that woke processor
    digitalWrite(LED_BULITIN, HIGH);
    delay(100);
    digitalWrite(LED_BULITIN, LOW);
}
```
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide2.png "Drivers inherit SnoozeBlock")
