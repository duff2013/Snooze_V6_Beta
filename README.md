# Snooze v6.0.0 - Beta1

<h4>The new api. This is still beta so it might be broken at times. I'll update here once it becomes stable. Must have Teensyduino 1.29 beta2 installed.</h4>

---
For those using v5 of Snooze the reason for the change is that new Teensy's are in the pipeline and updating this fairly extensive library became to difficult because each new processor introduced has its own set of Low Power functionality. Now the library is divided into three classes:<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide1.png "Snooze Class Layout")<br>
As you can see the Drivers are now separated from the SnoozeBlock Class. Before these drivers where part of the SnoozeBlock which made it difficult to add new functionalities. Each Teensy can now have driver classes designed specifically for them. Users can install their own drivers also if they wish also, see the skeltonClass example. Snooze uses something similar to the Audio library's conventions for adding drivers. As an added benefit, only the drivers that are installed are called, before all drivers where called just not used if not configured, this should add some performance benefits.
<br>
---
Here is a basic example of the new api. As you can see we load three Driver Classes - touch, digital, timer. Next we see the SnoozeBlock only has the timer and digital drivers installed. Even though the Touch driver is loaded it is not installed so it won't get called. In this example either the timer expires or the digital pin is lifted will wake the Teensy up.
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
     * Digital pins: 2,4,6,7,9,10,11,13,16,21,22,26,30,33
     *
     * Teensy LC
     * Digital pins: 2,6,7,9,10,11,16,21,22
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
<b>Lets break out each part out so we can see what is going on here:</b><br>
1. Include the Snooze library header. This brings in Snooze, SnoozeBlock, Core Driver Classes.<br>
```#include <Snooze.h>```<br>
2. Next we load the drivers classes we want visible to install.<br>
```
SnoozeTouch touch;
SnoozeDigital digital;
SnoozeTimer timer;
```
<br>
3. Here we install only the Timer and Digital drivers to the SnoozeBlock.
<br>
```
SnoozeBlock config(timer, digital);
```
<br>
4. Now we configure the Drivers, typically drivers have some way of changing certain parameters.<br>
```
timer.setTimer(5000);// milliseconds
digital.pinMode(21, INPUT_PULLUP, RISING);//pin, mode, type
digital.pinMode(22, INPUT_PULLUP, RISING);//pin, mode, type
```
<br>
5. Lastly we supply the SnoozeBlock to whatever sleep function you want to use from the Snooze class. We still have the same three sleep modes<br>
```
Snooze.sleep( config );
Snooze.deepSleep( config );
Snooze.hibernate( config );
```
<br>
---
Now that was easy enough lets take deeper look at the library structure.<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide3.png "Snooze Library Layout")
<br>
As you can see Snooze library has only one Snooze Class but can have mulitple SnoozeBlocks which have mulitiple Drivers. Refferring to the first graphic Snooze is 3 Tier Class structure:
1. Snooze Class
2. SnoozeBlock Class
3. Drivers

<h4> Snooze Class:</h4>
...
<h4> SnoozeBlock Class:</h4>
...
<h4> Driver Class:</h4>
...

---
<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide4.png "Snooze Library Layout")
<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide5.png "Snooze Library Layout")
<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide6.png "Snooze Library Layout")
<br>
![alt text](https://github.com/duff2013/Snooze_V6_Beta/blob/master/images/Snooze_Class_Layout/Slide7.png "Snooze Library Layout")
<br>





