/*
 * Display.cpp
 *
 *  Created on: 12 дек. 2015 г.
 *      Author: feedbee
 */

#include "Display.h"

Display::Display() : LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE) {
  _line = 0;
}

void Display::nextLine(void) {
  if (_line >= _numlines - 1) {
    _line = 0;
  } else {
    _line++;
  }
  LiquidCrystal_I2C::setCursor(0, _line);
}

void Display::setCursor(uint8_t col, uint8_t row) {
  LiquidCrystal_I2C::setCursor(col, row);
  _line = row;
}

void Display::clear() {
  _line = 0;
  LiquidCrystal_I2C::clear();
}

void Display::clearLine() {
  LiquidCrystal_I2C::setCursor(0, _line);
  for (int i = 0; i < LiquidCrystal_I2C::_cols; i++) {
    LiquidCrystal_I2C::print(" ");
  }
  LiquidCrystal_I2C::setCursor(0, _line);
}

size_t Display::println(const __FlashStringHelper *ifsh)
{
  size_t n = print(ifsh);
  nextLine();
  return n;
}

size_t Display::println(void)
{
  nextLine();
  return 0;
}

size_t Display::println(const String &s)
{
  size_t n = print(s);
  nextLine();
  return n;
}

size_t Display::println(const char c[])
{
  size_t n = print(c);
  nextLine();
  return n;
}

size_t Display::println(char c)
{
  size_t n = print(c);
  nextLine();
  return n;
}

size_t Display::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  nextLine();
  return n;
}

size_t Display::println(int num, int base)
{
  size_t n = print(num, base);
  nextLine();
  return n;
}

size_t Display::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  nextLine();
  return n;
}

size_t Display::println(long num, int base)
{
  size_t n = print(num, base);
  nextLine();
  return n;
}

size_t Display::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  nextLine();
  return n;
}

size_t Display::println(double num, int digits)
{
  size_t n = print(num, digits);
  nextLine();
  return n;
}

size_t Display::println(const Printable& x)
{
  size_t n = print(x);
  nextLine();
  return n;
}
