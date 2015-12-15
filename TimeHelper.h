/*
 * TimeHelper.h
 */

#ifndef TIMEHELPER_H_
#define TIMEHELPER_H_

#include <WString.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class TimeHelper {
 public:
  void
  static
  getDateAndTime(char* date, char* time);
  int
  static
  getYear();

 private:
  static
  char *
  leadZero(int i, char* buf);
};

#endif /* TIMEHELPER_H_ */
