/*
 * Controller.h
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "TimeHelper.h"
#include "ButtonBlock.h"
#include "Display.h"
#include "Controller.h"

class Controller {
 public:
  enum states {
    dtm_no_state = 0,
    dtm_show,
    dtm_set_year,
    dtm_set_month,
    dtm_set_date,
    dtm_set_day,
    dtm_set_hours,
    dtm_set_minutes,
    dtm_set_seconds,
    dtm_off,
    cnt_st
  };

  enum signals {
    btn_no_signal = 0,
    btn_A_cancel,
    btn_B_left,
    btn_C_right,
    btn_D_enter,
    cnt_sg
  };

  typedef void (Controller::*modeLoop)(enum states state, enum signals signal);
  typedef void (Controller::*modeTransition)(enum states stateOld, enum states stateNew,
      enum signals signal);

  struct transitionInfo {
    enum states newState;
    modeTransition transition;
  };

  Controller(ButtonBlock *bbb, Display *display);
  void loop(void);

 private:
  ButtonBlock* _bb;
  Display* _display;
  enum states _mode;

  transitionInfo _stateTransitionMatrix[cnt_st][cnt_sg];
  void initFSM(void);

  enum signals _buttonToSignalMap[5] = {
      btn_no_signal,
      btn_D_enter,
      btn_C_right,
      btn_B_left,
      btn_A_cancel
  };

  modeLoop loops[10] = {
      NULL,
      &Controller::dtm_show_cl,
      &Controller::dtm_set_year_cl,
      NULL,  //    dtm_set_month_cl,
      NULL,  //    dtm_set_date_cl,
      NULL,  //    dtm_set_day_cl,
      NULL,  //    dtm_set_hours_cl,
      NULL,  //    dtm_set_minutes_cl,
      NULL,  //    dtm_set_seconds_cl,
      &Controller::dtm_off_cl
  };
  void dtm_show_cl(enum states state, enum signals signal);
  void dtm_set_year_cl(enum states state, enum signals signal);
  void dtm_off_cl(enum states state, enum signals signal);

  void dtm_set_year_transition(enum states stateOld, enum states stateNew,
                    enum signals signal);
  void dtm_off_on_transition(enum states stateOld, enum states stateNew,
                  enum signals signal);
  void dtm_common_transition_clear(enum states stateOld, enum states stateNew,
                    enum signals signal);
};

//void dtm_set_month_cl(enum states state, enum signals signal);
//void dtm_set_date_cl(enum states state, enum signals signal);
//void dtm_set_day_cl(enum states state, enum signals signal);
//void dtm_set_hours_cl(enum states state, enum signals signal);
//void dtm_set_minutes_cl(enum states state, enum signals signal);
//void dtm_set_seconds_cl(enum states state, enum signals signal);

#endif /* CONTROLLER_H_ */
