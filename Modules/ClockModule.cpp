/*
 * ClockModule.cpp
 */

#include "ClockModule.h"

ClockModule::ClockModule(Controller* ctrl, Display* display) {
  _display = display;

  // Init Loops
  ctrl->addLoop(dtm_show, ctrl->CreateIndirectCall(this, &ClockModule::dtm_show_cl));
  Controller::CallBase* setXxxCallback = ctrl->CreateIndirectCall(this, &ClockModule::dtm_set_xxx_cl);
  for (uint8_t i = 2; i < 10; i++) {
    ctrl->addLoop(i, setXxxCallback);
  }
  ctrl->addLoop(dtm_off, ctrl->CreateIndirectCall(this, &ClockModule::dtm_off_cl));

  // Init Transitions
  Controller::CallBaseTr* onOffTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_off_on_transition);
  Controller::CallBaseTr* commonClearTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_common_transition_clear);
  Controller::CallBaseTr* commonLowerTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_common_lower);
  Controller::CallBaseTr* commonHigherTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_common_higher);
  Controller::CallBaseTr* wdayLowerTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_wday_lower);
  Controller::CallBaseTr* wdayHigherTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_wday_higher);
  Controller::CallBaseTr* setYearTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_year_transition);
  Controller::CallBaseTr* setMonthTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_month_transition);
  Controller::CallBaseTr* setDateTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_date_transition);
  Controller::CallBaseTr* setWDayTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_wday_transition);
  Controller::CallBaseTr* setHoursTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_hours_transition);
  Controller::CallBaseTr* setMinutesTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_minutes_transition);
  Controller::CallBaseTr* setSecondsTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_seconds_transition);
  Controller::CallBaseTr* setTimeTrCallback = ctrl->CreateIndirectCallTr(this, &ClockModule::dtm_set_time);

  ctrl->addStateTr(dtm_show, Controller::btn_A_cancel, { ClockModule::dtm_off, onOffTrCallback });
  ctrl->addStateTr(dtm_show, Controller::btn_D_enter, { ClockModule::dtm_set_year, setYearTrCallback });

  ctrl->addStateTr(dtm_set_year, Controller::btn_A_cancel, { ClockModule::dtm_show, commonClearTrCallback });
  ctrl->addStateTr(dtm_set_year, Controller::btn_B_left, { ClockModule::dtm_set_year, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_year, Controller::btn_C_right, { ClockModule::dtm_set_year, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_year, Controller::btn_D_enter, { ClockModule::dtm_set_month, setMonthTrCallback });

  ctrl->addStateTr(dtm_set_month, Controller::btn_A_cancel, { ClockModule::dtm_set_year, setYearTrCallback });
  ctrl->addStateTr(dtm_set_month, Controller::btn_B_left, { ClockModule::dtm_set_month, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_month, Controller::btn_C_right, { ClockModule::dtm_set_month, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_month, Controller::btn_D_enter, { ClockModule::dtm_set_date, setDateTrCallback });

  ctrl->addStateTr(dtm_set_date, Controller::btn_A_cancel, { ClockModule::dtm_set_month, setMonthTrCallback });
  ctrl->addStateTr(dtm_set_date, Controller::btn_B_left, { ClockModule::dtm_set_date, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_date, Controller::btn_C_right, { ClockModule::dtm_set_date, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_date, Controller::btn_D_enter, { ClockModule::dtm_set_wday, setWDayTrCallback });

  ctrl->addStateTr(dtm_set_wday, Controller::btn_A_cancel, { ClockModule::dtm_set_date, setDateTrCallback });
  ctrl->addStateTr(dtm_set_wday, Controller::btn_B_left, { ClockModule::dtm_set_wday, wdayLowerTrCallback });
  ctrl->addStateTr(dtm_set_wday, Controller::btn_C_right, { ClockModule::dtm_set_wday, wdayHigherTrCallback });
  ctrl->addStateTr(dtm_set_wday, Controller::btn_D_enter, { ClockModule::dtm_set_hours, setHoursTrCallback });

  ctrl->addStateTr(dtm_set_hours, Controller::btn_A_cancel, { ClockModule::dtm_set_wday, setWDayTrCallback });
  ctrl->addStateTr(dtm_set_hours, Controller::btn_B_left, { ClockModule::dtm_set_hours, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_hours, Controller::btn_C_right, { ClockModule::dtm_set_hours, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_hours, Controller::btn_D_enter, { ClockModule::dtm_set_minutes, setMinutesTrCallback });

  ctrl->addStateTr(dtm_set_minutes, Controller::btn_A_cancel, { ClockModule::dtm_set_hours, setHoursTrCallback });
  ctrl->addStateTr(dtm_set_minutes, Controller::btn_B_left, { ClockModule::dtm_set_minutes, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_minutes, Controller::btn_C_right, { ClockModule::dtm_set_minutes, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_minutes, Controller::btn_D_enter, { ClockModule::dtm_set_seconds, setSecondsTrCallback });

  ctrl->addStateTr(dtm_set_seconds, Controller::btn_A_cancel, { ClockModule::dtm_set_minutes, setMinutesTrCallback });
  ctrl->addStateTr(dtm_set_seconds, Controller::btn_B_left, { ClockModule::dtm_set_seconds, commonLowerTrCallback });
  ctrl->addStateTr(dtm_set_seconds, Controller::btn_C_right, { ClockModule::dtm_set_seconds, commonHigherTrCallback });
  ctrl->addStateTr(dtm_set_seconds, Controller::btn_D_enter, { ClockModule::dtm_show, setTimeTrCallback });

  ctrl->addStateTr(dtm_off, Controller::btn_D_enter, { ClockModule::dtm_show, onOffTrCallback });
}

// Loops

void ClockModule::dtm_show_cl(uint8_t state, enum Controller::signals signal) {
  char date[16], time[9];
  TimeHelper::getDateAndTime(date, time);

  _display->println(date);
  _display->println(time);
}

void ClockModule::dtm_set_xxx_cl(uint8_t state, enum Controller::signals signal) {
  static unsigned long lastBlink = millis();
  static bool show = false;

  unsigned long cmillis = millis();

  if (cmillis < lastBlink || cmillis - lastBlink >= 500 + (unsigned int)show * 500) {
    show = !show;
    if (show) {
      switch (_currentSetMode) {
        case sm_year: _display->print(_newDate / (unsigned long)10000); break;
        case sm_month: _display->print(_newDate / (unsigned long)100 % (unsigned long)100); break;
        case sm_date: _display->print(_newDate % (unsigned long)100); break;
        case sm_hours: _display->print(_newTime / (unsigned long)10000); break;
        case sm_minutes: _display->print(_newTime / (unsigned long)100 % (unsigned long)100); break;
        case sm_seconds: _display->print(_newTime % (unsigned long)100); break;

        case sm_wday:
          char cvalue[4];
          TimeHelper::getWeekdayName(_newWDay, cvalue);
          _display->print(cvalue);
          break;
      }
    } else {
      _display->clearLine();
    }
    lastBlink = cmillis;
  }
}

void ClockModule::dtm_off_cl(uint8_t state, enum Controller::signals signal) {
  // nothing to do
}

// Transitions

void ClockModule::dtm_set_year_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set year:");

  _currentSetMode = sm_year;

  _newDate = TimeHelper::getYMD_long();
  _newTime = TimeHelper::getHMS_long();
  _newWDay = TimeHelper::getWeekday();
}

void ClockModule::dtm_set_month_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set month:");

  _currentSetMode = sm_month;
}

void ClockModule::dtm_set_date_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set date:");

  _currentSetMode = sm_date;
}

void ClockModule::dtm_set_wday_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set week day:");

  _currentSetMode = sm_wday;
}

void ClockModule::dtm_set_hours_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set hours:");

  _currentSetMode = sm_hours;
}

void ClockModule::dtm_set_minutes_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set minutes:");

  _currentSetMode = sm_minutes;
}

void ClockModule::dtm_set_seconds_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
  _display->println("Set seconds:");

  _currentSetMode = sm_seconds;
}

void ClockModule::dtm_off_on_transition(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  if (stateNew == dtm_off) {
    _display->off();
  } else {
    _display->on();
  }
}

void ClockModule::dtm_common_transition_clear(uint8_t stateOld, uint8_t stateNew,
                  enum Controller::signals signal) {
  _display->clear();
}


void ClockModule::dtm_common_offset(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal, int offset) {
  unsigned long cur;
  switch (_currentSetMode) {
    case sm_year:
      cur = _newDate / (unsigned long)10000;
      if (offset > 0 && cur < 2099) {
        _newDate += offset * (unsigned long)10000;
      }
      if (offset < 0 && cur > 2000) {
        _newDate += offset * (long)10000;
      }
      break;
    case sm_month:
      cur = _newDate / (unsigned long)100 % (unsigned long)100;
      if (offset > 0 && cur < 12) {
        _newDate += offset * 100;
      }
      if (offset < 0 && cur > 1) {
        _newDate += offset * 100;
      }
      break;
    case sm_date:
      cur = _newDate % (unsigned long)100;
      if (offset > 0 && cur < 31) { // TODO: relate to current month
        _newDate += offset;
      }
      if (offset < 0 && cur > 1) {
        _newDate += offset;
      }
      break;
    case sm_hours:
      cur = _newTime / (unsigned long)10000;
      if (offset > 0 && cur < 24) {
        _newTime += offset * (unsigned long)10000;
      }
      if (offset < 0 && cur > 1) {
        _newTime += offset * (long)10000;
      }
      break;
    case sm_minutes:
      cur = _newTime / (unsigned long)100 % (unsigned long)100;
      if (offset > 0 && cur < 60) {
        _newTime += offset * (unsigned long)100;
      }
      if (offset < 0 && cur > 1) {
        _newTime += offset * 100;
      }
      break;
    case sm_seconds:
      cur = _newTime % (unsigned long)100;
      if (offset > 0 && cur < 60) {
        _newTime += offset;
      }
      if (offset < 0 && cur > 1) {
        _newTime += offset;
      }
      break;
    default: break;
  }
}

void ClockModule::dtm_common_lower(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal) {
  dtm_common_offset(stateOld, stateNew, signal, -1);
}
void ClockModule::dtm_common_higher(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal) {
  dtm_common_offset(stateOld, stateNew, signal, 1);
}

void ClockModule::dtm_wday_lower(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal) {
  _newWDay <= 0 ? _newWDay = 6 : _newWDay--;
}
void ClockModule::dtm_wday_higher(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal) {
  _newWDay >= 6 ? _newWDay = 0 : _newWDay++;
}

void ClockModule::dtm_set_time(uint8_t stateOld, uint8_t stateNew,
                      enum Controller::signals signal) {
  _display->clear();
  TimeHelper::setYMD_HMS_wday(_newDate, _newTime, _newWDay);
}
