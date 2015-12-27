/*
 * ButtonBlock.h
 *
 *  Created on: 12 дек. 2015 г.
 *      Author: feedbee
 */

#ifndef BUTTONBLOCK_H_
#define BUTTONBLOCK_H_

#include <Arduino.h>

#define BB_BUTTON_EMPTY 0
#define BB_BUTTON_CANCEL 1
#define BB_BUTTON_LEFT 2
#define BB_BUTTON_RIGHT 3
#define BB_BUTTON_SELECT 4

class ButtonBlock {
 public:
  ButtonBlock();
  ButtonBlock(unsigned char pin);
  unsigned char getPressedButton(void);

 private:
  void setup(unsigned char pin);
  unsigned char _pin;
  unsigned char _wasPressed;
  unsigned char detectButton(int value);
};

#endif /* BUTTONBLOCK_H_ */
