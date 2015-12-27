#include <Arduino.h>

// LCD (1602)
#include "Display.h"
Display display;

// Clock DS3231
#include <Sodaq_DS3231.h>

#include "TimeHelper.h"
#include "ButtonBlock.h"
#include "Controller.h"
#include "Modules/ClockModule.h"

#define VERSION "2.5.1"

ButtonBlock bb(A0);
Controller ctrl(&bb, &display);
ClockModule cm(&ctrl, &display);

void setup() {
  display.begin(16, 2);

  display.println("Digital Clock");
  display.println("v." VERSION);
  delay(1000);
  display.clear();

  ctrl.setState(ClockModule::dtm_show); // Default mode

  rtc.begin();
}

void loop() {
  ctrl.loop();
  delay(100);
}
