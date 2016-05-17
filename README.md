# Snooze V6.0.0 - Beta1

<h4>The new api allows me to add new functionality and new Teensy's much easier. This is still beta so it might be broken at times. I'll update here once it becomes stable.<br>

The reason for the change is that new Teensy's are in the pipeline and updating this fairly extensive library became to difficult because each new processor introduced has its own set of Low Power functionality. Now the library is divided into three classes:<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide1.png "Snooze Class Layout")<br>
As you can see the Drivers are now separated from the SnoozeBlock Class. Before these drivers where part of the SnoozeBlock which made it difficult to add new functionalities. Each Teensy can now have drivers classes designed specifically for them much easier. Now users have a way to install their own drivers also if they wish. An added benefit is now only the drivers that are installed are called, before all drivers where called just not used if not configured, this should add some performance benefits. 
<br>
<br>

Here is a basic example of the new api. As you can see in this example only the timer and digital drivers are installed for the SnoozeBlock. Even though the Touch driver is loaded it is not installed so it won't get called.
```c++
#include <Snooze.h>
// Load drivers
SnoozeTouch touch;
SnoozeDigital digital;
SnoozeTimer timer;

// install drivers to a SnoozeBlock
SnoozeBlock config(timer, digital);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    /********************************************************
     * Set Low Power Timer wake up in milliseconds.
     ********************************************************/
    timer.setTimer(5000);// milliseconds
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
<b>Lets break each part out so we can see what is going on:</b><br>
1. Include the Snooze library header. This brings in Snooze, SnoozeBlock, Core Driver Classes.<br>
```#include <Snooze.h>```<br>
2. Next we load the drivers classes we want visable to install. We can have different instances of the same driver class.<br>
```
SnoozeTouch touch;
SnoozeDigital digital;
SnoozeTimer timer;
```
<br>
3. Here we install only the Timer and Digital drivers to the SnoozeBlockWe can have more than one SnoozeBlock.<br>
```
SnoozeBlock config(timer, digital);
```
<br>
4. Now we configure the Drivers, typically drivers have some way of changing certian parameters.<br>
```
timer.setTimer(5000);// milliseconds
digital.pinMode(21, INPUT_PULLUP, RISING);//pin, mode, type
digital.pinMode(22, INPUT_PULLUP, RISING);//pin, mode, type
```
<br>
5. Lastly we supply the SnoozeBlock to whatever sleep function you want to use.<br>
```
Snooze.deepSleep( config );
```
<br>
Now that was easy enough lets take deeper look at how the Drivers inherit from the SnoozeBlock.
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide2.png "Drivers inherit SnoozeBlock")
