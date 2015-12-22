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

  void static setYMD_HMS_wday(unsigned long newDate, unsigned long newTime, uint8_t wday);

  /**
   * 0 for Sunday, 6 for Saturday
   */
  uint8_t static getWeekday();

  void static getWeekdayName(uint8_t index, char* buffer);

  int static getYear();
  int static getMonth();

  int static getDate();

 private:
  static char * leadZero(int i, char* buf);
};

#endif /* TIMEHELPER_H_ */
