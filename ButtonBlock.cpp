/*
 * ButtonBlock.cpp
 *
 *  Created on: 12 дек. 2015 г.
 *      Author: feedbee
 */

#include "ButtonBlock.h"

#define BB_VALUE_OFFSET 10
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
  unsigned char button = detectButton(analogRead(_pin));
  delay(10);
  if (button == detectButton(analogRead(_pin))) {
    return button;
  } else {
    return BB_BUTTON_EMPTY;
  }
}

unsigned char ButtonBlock::detectButton(int value) {
  int levels[4] = { 0, 150, 351, 502 };
  for (unsigned char i = 0; i < 4; i++) {
    if ((levels[i] <= BB_VALUE_OFFSET || value > levels[i] - BB_VALUE_OFFSET)
    &&
    (levels[i] >= BB_VALUE_UPPER_BOUND || value < levels[i] + BB_VALUE_OFFSET)) {
      return i + 1;
    }
  }

  return 0;
}
