/*
 * Display.h
 *
 *  Created on: 12 дек. 2015 г.
 *      Author: feedbee
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Display : public LiquidCrystal_I2C {
 public:
  Display();

  void nextLine(void);
  void setCursor(uint8_t, uint8_t);
  void clear();
  void clearLine();

  size_t println(const __FlashStringHelper *);
  size_t println(const String &s);
  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(double, int = 2);
  size_t println(const Printable&);
  size_t println(void);

 private:
  uint8_t _line;
};

#endif /* DISPLAY_H_ */
