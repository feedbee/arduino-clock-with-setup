/*
 * TimeHelper.cpp
 */

#include "TimeHelper.h"

// Clock DS3231
#include <Sodaq_DS3231.h>

void TimeHelper::getDateAndTime(char* date, char* time) {
  char weekDay[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

  DateTime now = rtc.now();

  sprintf(date, "%04d-%02d-%02d, %s", now.year(), now.month(), now.date(), weekDay[now.dayOfWeek() - 1]);
  sprintf(time, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
}

unsigned long TimeHelper::getYMD_long() {
  DateTime now = rtc.now();
  return now.year() * (unsigned long)10000 + now.month() * (unsigned long)100 + now.date();
}

unsigned long TimeHelper::getHMS_long() {
  DateTime now = rtc.now();
  return now.hour() * (unsigned long)10000 + now.minute() * (unsigned long)100 + now.second();
}

void TimeHelper::setYMD_HMS_wday(unsigned long newDate, unsigned long newTime, uint8_t wday) {
  unsigned long y, m, d, hr, min, sec;
  y = newDate / (unsigned long)10000;
  m = newDate / (unsigned long)100 % (unsigned long)100;
  d = newDate % (unsigned long)100;
  hr = newTime / (unsigned long)10000;
  min = newTime / (unsigned long)100 % (unsigned long)100;
  sec = newTime % (unsigned long)100;
  DateTime dt = DateTime(y, m, d, hr, min, sec, wday + 1);
  rtc.setDateTime(dt);
}

/**
 * 0 for Sunday, 6 for Saturday
 */
uint8_t TimeHelper::getWeekday() {
  return rtc.now().dayOfWeek() - 1;
}

/**
 * 0 for Sunday, 6 for Saturday
 */
void TimeHelper::getWeekdayName(uint8_t index, char* buffer) {
  char weekDay[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  strcpy(buffer, weekDay[index]);
}

int TimeHelper::getYear() {
  DateTime now = rtc.now();
  return now.year();
}

int TimeHelper::getMonth() {
  DateTime now = rtc.now();
  return now.month();
}

int TimeHelper::getDate() {
  DateTime now = rtc.now();
  return now.date();
}
