/*
 * ButtonBlock.cpp
 *
 *  Created on: 12 дек. 2015 г.
 *      Author: feedbee
 */

#include "ButtonBlock.h"

#define BB_VALUE_OFFSET 50
#define BB_VALUE_UPPER_BOUND 1023 - BB_VALUE_OFFSET

ButtonBlock::ButtonBlock() {
  setup(A0);
}

ButtonBlock::ButtonBlock(unsigned char pin) {
  setup(pin);
}

void ButtonBlock::setup(unsigned char pin) {
  _pin = pin;
  pinMode(pin, INPUT);
  digitalWrite(pin, LOW);
}

unsigned char ButtonBlock::getPressedButton(void) {
  static unsigned char _wasPressed = BB_BUTTON_EMPTY;
  static unsigned int pressedMills = 0;
  static unsigned int haltTimes = 0;

  unsigned char button = readButton();

  if (button == BB_BUTTON_EMPTY && _wasPressed != BB_BUTTON_EMPTY) {  // button is realized now
    unsigned char tmpBuf = _wasPressed;
    _wasPressed = BB_BUTTON_EMPTY;
    pressedMills = 0;
    if (!haltTimes) {
      return tmpBuf;  // fix click
    } else {
      haltTimes = 0;
      return BB_BUTTON_EMPTY;  // prevent click if hold was detected
    }

  } else if (button != BB_BUTTON_EMPTY) {  // button is pressed now
    if (pressedMills && millis() - pressedMills > 1000
        && haltTimes++ % 4 == 0) {  // a button was halt for more than a second
      return button;  // fix hold every 4-th time (~(50+10) millis x 4 = ~240 millis)
    } else if (!pressedMills) {
      pressedMills = millis();
    }
  }

  _wasPressed = button;

  return BB_BUTTON_EMPTY;
}

unsigned char ButtonBlock::readButton(void) {
  unsigned char button = detectButton(analogRead(_pin));
  delay(10);  // bounce protection
  return button == detectButton(analogRead(_pin)) ? button : BB_BUTTON_EMPTY;
}

unsigned char ButtonBlock::detectButton(int value) {
  int levels[4] = { 0, 150, 351, 502 };
  for (unsigned char i = 0; i < 4; i++) {
    if ((levels[i] <= BB_VALUE_OFFSET || value > levels[i] - BB_VALUE_OFFSET)
        && (levels[i] >= BB_VALUE_UPPER_BOUND
            || value < levels[i] + BB_VALUE_OFFSET)) {
      return i + 1;
    }
  }

  return 0;
}
