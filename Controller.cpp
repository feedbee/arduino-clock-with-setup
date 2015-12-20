/*
 * Controller.cpp
 */

#include <Arduino.h>

#include "Controller.h"

Controller::Controller(ButtonBlock *buttonBlock, Display *display) {
  _bb = buttonBlock;
  _display = display;
  _currentState = dtm_show;  // Default mode
  initFSM();
}

//enum states
//{
//    dtm_show = 0,
//    dtm_set_year,
//    dtm_set_month,
//    dtm_set_date,
//    dtm_set_day,
//    dtm_set_hours,
//    dtm_set_minutes,
//    dtm_set_seconds,
//    dtm_off
//};
//
//enum signals
//{
//    btn_A_cancel,
//    btn_B_left,
//    btn_C_right,
//    btn_D_enter
//};
void Controller::initFSM(void) {
  _stateMatrix[dtm_show][btn_A_cancel] = { dtm_off, &Controller::dtm_off_on_transition };
  _stateMatrix[dtm_show][btn_D_enter] = { dtm_set_year, &Controller::dtm_set_year_transition };

  _stateMatrix[dtm_set_year][btn_A_cancel] = { dtm_show, &Controller::dtm_common_transition_clear };
  _stateMatrix[dtm_set_year][btn_B_left] = { dtm_set_year, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_year][btn_C_right] = { dtm_set_year, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_year][btn_D_enter] = { dtm_set_month, &Controller::dtm_set_month_transition };

  _stateMatrix[dtm_set_month][btn_A_cancel] = { dtm_set_year, &Controller::dtm_set_year_transition };
  _stateMatrix[dtm_set_month][btn_B_left] = { dtm_set_month, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_month][btn_C_right] = { dtm_set_month, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_month][btn_D_enter] = { dtm_set_date, &Controller::dtm_set_date_transition };

  _stateMatrix[dtm_set_date][btn_A_cancel] = { dtm_set_month, &Controller::dtm_set_month_transition };
  _stateMatrix[dtm_set_date][btn_B_left] = { dtm_set_date, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_date][btn_C_right] = { dtm_set_date, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_date][btn_D_enter] = { dtm_set_hours, &Controller::dtm_set_hours_transition };

  _stateMatrix[dtm_set_hours][btn_A_cancel] = { dtm_set_date, &Controller::dtm_set_date_transition };
  _stateMatrix[dtm_set_hours][btn_B_left] = { dtm_set_hours, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_hours][btn_C_right] = { dtm_set_hours, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_hours][btn_D_enter] = { dtm_set_minutes, &Controller::dtm_set_minutes_transition };

  _stateMatrix[dtm_set_minutes][btn_A_cancel] = { dtm_set_hours, &Controller::dtm_set_hours_transition };
  _stateMatrix[dtm_set_minutes][btn_B_left] = { dtm_set_minutes, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_minutes][btn_C_right] = { dtm_set_minutes, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_minutes][btn_D_enter] = { dtm_set_seconds, &Controller::dtm_set_seconds_transition };

  _stateMatrix[dtm_set_seconds][btn_A_cancel] = { dtm_set_minutes, &Controller::dtm_set_minutes_transition };
  _stateMatrix[dtm_set_seconds][btn_B_left] = { dtm_set_seconds, &Controller::dtm_common_lower };
  _stateMatrix[dtm_set_seconds][btn_C_right] = { dtm_set_seconds, &Controller::dtm_common_higher };
  _stateMatrix[dtm_set_seconds][btn_D_enter] = { dtm_show, &Controller::dtm_set_time };

  _stateMatrix[dtm_off][btn_D_enter] = { dtm_show, &Controller::dtm_off_on_transition };
}

void Controller::loop(void) {
  unsigned char b = _bb->getPressedButton();
  enum signals signal = _buttonToSignalMap[b];
  if (signal && _stateMatrix[_currentState][signal].newState) {
    transitionInfo tri = _stateMatrix[_currentState][signal];
    enum states oldMode = _currentState;
    _currentState = tri.newState;
    if (tri.transition) {
      (this->*(tri.transition))(oldMode, _currentState, signal);
    }
  }

  modeLoop callback = loops[_currentState];
  if (!callback) {
    _display->clear();
    _display->println("Invalid mode");
    return;
  }
  (this->*callback)(_currentState, signal);
}

void Controller::dtm_show_cl(enum states state, enum signals signal) {
  char date[16], time[9];
  TimeHelper::getDateAndTime(date, time);

  _display->println(date);
  _display->print(time);

  unsigned char b = _bb->getPressedButton();
  _display->print("      ");
  _display->print(b);
  _display->println();
}
void Controller::dtm_set_xxx_cl(enum states state, enum signals signal) {
  static unsigned long lastBlink = millis();
  static bool show = false;

  unsigned long cmillis = millis();

  if (cmillis < lastBlink || cmillis - lastBlink >= 500 + (unsigned int)show * 500) {
    show = !show;
    if (show) {
      int value = 0;
      switch (_currentSetMode) {
        case sm_year: value = _newDate / (unsigned long)10000; break;
        case sm_month: value = _newDate / (unsigned long)100 % (unsigned long)100; break;
        case sm_date: value = _newDate % (unsigned long)100; break;
        case sm_hours: value = _newTime / (unsigned long)10000; break;
        case sm_minutes: value = _newTime / (unsigned long)100 % (unsigned long)100; break;
        case sm_seconds: value = _newTime % (unsigned long)100; break;
      }
      _display->print(value);
    } else {
      _display->clearLine();
    }
    lastBlink = cmillis;
  }
}
void Controller::dtm_off_cl(enum states state, enum signals signal) {
  // nothing to do
}


void Controller::dtm_set_year_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set year:");

  _currentSetMode = sm_year;

  _newDate = TimeHelper::getYMD_long();
  _newTime = TimeHelper::getHMS_long();
}

void Controller::dtm_set_month_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set month:");

  _currentSetMode = sm_month;
}

void Controller::dtm_set_date_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set date:");

  _currentSetMode = sm_date;
}

void Controller::dtm_set_hours_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set hours:");

  _currentSetMode = sm_hours;
}

void Controller::dtm_set_minutes_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set minutes:");

  _currentSetMode = sm_minutes;
}

void Controller::dtm_set_seconds_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set seconds:");

  _currentSetMode = sm_seconds;
}

void Controller::dtm_off_on_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  if (stateNew == dtm_off) {
    _display->off();
  } else {
    _display->on();
  }
}

void Controller::dtm_common_transition_clear(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
}


void Controller::dtm_common_offset(enum states stateOld, enum states stateNew,
                      enum signals signal, int offset) {
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
  }
}

void Controller::dtm_common_lower(enum states stateOld, enum states stateNew,
                      enum signals signal) {
  dtm_common_offset(stateOld, stateNew, signal, -1);
}
void Controller::dtm_common_higher(enum states stateOld, enum states stateNew,
                      enum signals signal) {
  dtm_common_offset(stateOld, stateNew, signal, 1);
}

void Controller::dtm_set_time(enum states stateOld, enum states stateNew,
                      enum signals signal) {
  _display->clear();
  TimeHelper::setYMD_HMS_long(_newDate, _newTime);
}
