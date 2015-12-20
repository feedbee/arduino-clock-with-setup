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
  void static getDateAndTime(char* date, char* time);

  unsigned long static getYMD_long();
  unsigned long static getHMS_long();

  void static setYMD_HMS_long(unsigned long newDate, unsigned long newTime);

  int static getYear();
  int static getMonth();

  int static getDate();

 private:
  static char * leadZero(int i, char* buf);
};

#endif /* TIMEHELPER_H_ */
