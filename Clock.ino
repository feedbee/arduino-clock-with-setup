#include <Arduino.h>

// LCD (1602)
#include "Display.h"
Display display;

// Clock DS3231
#include <Sodaq_DS3231.h>

#include "TimeHelper.h"
#include "ButtonBlock.h"
#include "Controller.h"

#define VERSION "2.4.0"

ButtonBlock bb(A0);
Controller ctrl(&bb, &display);

void setup() {
  display.begin(16, 2);

  display.println("Digital Clock");
  display.println("v." VERSION);
  delay(1000);
  display.clear();

  rtc.begin();
}

void loop() {
  ctrl.loop();
  delay(100);
}
