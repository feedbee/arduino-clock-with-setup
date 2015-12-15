/*
 * TimeHelper.cpp
 */

#include "TimeHelper.h"

// Clock DS3231
#include <Sodaq_DS3231.h>

void TimeHelper::getDateAndTime(char* date, char* time) {
  static char weekDay[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

  DateTime now = rtc.now();

  sprintf(date, "%04d-%02d-%02d, %s", now.year(), now.month(), now.date(), weekDay[now.dayOfWeek()]);
  sprintf(time, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
}

int TimeHelper::getYear() {
  DateTime now = rtc.now();
  return now.year();
}
