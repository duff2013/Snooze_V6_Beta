/********************************************************************
 - Snooze REDUCED_CPU_BLOCK Example -
 HardwareSerial1_LP Driver Class reconfigures Serial1 use at 2MHz for
 use in the REDUCED_CPU_BLOCK. You need a second Teensy to read from
 this sketches send through Serial1.
 ********************************************************************/
#include <Snooze.h>
#include "HardwareSerial1_LP.h"
#include "delay.h"

HardwareSerial1_LP serial1(115200);
SnoozeBlock config(serial1);

elapsedMillis time;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial1.begin(115200);
  time = 0;
}


void loop() {
  Serial1.printf("F_CPU:\t%i\n", F_CPU);
  Serial1.flush();
  
  digitalWriteFast(LED_BUILTIN, LOW);
  REDUCED_CPU_BLOCK( config ) {
    Serial1.print("2_MHZ:\t2000000\n");
    Serial1.flush();
    time = 0;
    delay_lp(1000);
    uint32_t t = time;
    Serial1.printf("DELAY:\t%i\n", t);
    Serial1.println("----------------------------------------");
    Serial1.flush();
  }
  digitalWriteFast(LED_BUILTIN, HIGH);
  delay(500);
}
