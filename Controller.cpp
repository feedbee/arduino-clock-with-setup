/*
 * Controller.cpp
 */

#include <Arduino.h>

#include "Controller.h"

Controller::Controller(ButtonBlock *buttonBlock, Display *display) {
  _bb = buttonBlock;
  _display = display;
  _mode = dtm_show;  // Default mode
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
  _stateTransitionMatrix[dtm_show][btn_A_cancel] = { dtm_off, &Controller::dtm_off_on_transition };
  _stateTransitionMatrix[dtm_show][btn_D_enter] = { dtm_set_year, &Controller::dtm_set_year_transition };

  _stateTransitionMatrix[dtm_set_year][btn_A_cancel] = { dtm_show, &Controller::dtm_common_transition_clear };
  _stateTransitionMatrix[dtm_set_year][btn_B_left] = { dtm_set_year, NULL };
  _stateTransitionMatrix[dtm_set_year][btn_C_right] = { dtm_set_year, NULL };
  _stateTransitionMatrix[dtm_set_year][btn_D_enter] = { dtm_set_month, &Controller::dtm_common_transition_clear };

  _stateTransitionMatrix[dtm_off][btn_D_enter] = { dtm_show, &Controller::dtm_off_on_transition };
}

void Controller::loop(void) {
  unsigned char b = _bb->getPressedButton();
  enum signals signal = _buttonToSignalMap[b];
  if (signal && _stateTransitionMatrix[_mode][signal].newState) {
    transitionInfo tri = _stateTransitionMatrix[_mode][signal];
    enum states oldMode = _mode;
    _mode = tri.newState;
    if (tri.transition) {
      (this->*(tri.transition))(oldMode, _mode, signal);
    }
  }

  modeLoop callback = loops[_mode];
  if (!callback) {
    _display->println("Invalid mode");
    return;
  }
  (this->*callback)(_mode, signal);
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
void Controller::dtm_set_year_cl(enum states state, enum signals signal) {
  static unsigned long lastBlink = millis();
  static bool show = false;

  unsigned long cmillis = millis();

  if (cmillis < lastBlink || cmillis - lastBlink >= 500) {
    show = !show;
    if (show) {
      _display->print(TimeHelper::getYear());
    } else {
      _display->clearLine();
    }
    _display->setCursor(0, 2);
    lastBlink = cmillis;
  }
}
//void dtm_set_month_cl(enum states state, enum signals signal) {
//
//}
//void dtm_set_date_cl(enum states state, enum signals signal) {
//
//}
//void dtm_set_day_cl(enum states state, enum signals signal) {
//
//}
//void dtm_set_hours_cl(enum states state, enum signals signal) {
//
//}
//void dtm_set_minutes_cl(enum states state, enum signals signal) {
//
//}
//void dtm_set_seconds_cl(enum states state, enum signals signal) {
//
//}
void Controller::dtm_off_cl(enum states state, enum signals signal) {
  // nothing to do
}


void Controller::dtm_set_year_transition(enum states stateOld, enum states stateNew,
                  enum signals signal) {
  _display->clear();
  _display->println("Set year:");
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