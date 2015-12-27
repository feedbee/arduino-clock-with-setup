/*
 * ClockModule.h
 */

#include "../Controller.h"

#ifndef MODULES_CLOCKMODULE_H_
#define MODULES_CLOCKMODULE_H_

class ClockModule {
 public:

  static const uint8_t dtm_no_state = 0,
    dtm_show = 1,
    dtm_set_year = 2,
    dtm_set_month = 3,
    dtm_set_date = 4,
    dtm_set_day = 5,
    dtm_set_wday = 6,
    dtm_set_hours = 7,
    dtm_set_minutes = 8,
    dtm_set_seconds = 9,
    dtm_off = 10;

  ClockModule(Controller *ctrl, Display *display);

  // Loops
  void dtm_show_cl(uint8_t state,
                   enum Controller::signals signal);
  void dtm_set_xxx_cl(uint8_t state,
                      enum Controller::signals signal);
  void dtm_off_cl(uint8_t state,
                  enum Controller::signals signal);

  // Transitions
  void dtm_set_year_transition(uint8_t stateOld,
                               uint8_t stateNew,
                               enum Controller::signals signal);
  void dtm_set_month_transition(uint8_t stateOld,
                                uint8_t stateNew,
                                enum Controller::signals signal);
  void dtm_set_date_transition(uint8_t stateOld,
                               uint8_t stateNew,
                               enum Controller::signals signal);
  void dtm_set_wday_transition(uint8_t stateOld,
                               uint8_t stateNew,
                               enum Controller::signals signal);
  void dtm_set_hours_transition(uint8_t stateOld,
                                uint8_t stateNew,
                                enum Controller::signals signal);
  void dtm_set_minutes_transition(uint8_t stateOld,
                                  uint8_t stateNew,
                                  enum Controller::signals signal);
  void dtm_set_seconds_transition(uint8_t stateOld,
                                  uint8_t stateNew,
                                  enum Controller::signals signal);
  void dtm_off_on_transition(uint8_t stateOld,
                             uint8_t stateNew,
                             enum Controller::signals signal);
  void dtm_common_transition_clear(uint8_t stateOld,
                                   uint8_t stateNew,
                                   enum Controller::signals signal);
  void dtm_common_offset(uint8_t stateOld,
                         uint8_t stateNew,
                         enum Controller::signals signal, int offset);
  void dtm_common_lower(uint8_t stateOld,
                        uint8_t stateNew,
                        enum Controller::signals signal);
  void dtm_common_higher(uint8_t stateOld,
                         uint8_t stateNew,
                         enum Controller::signals signal);
  void dtm_wday_lower(uint8_t stateOld,
                      uint8_t stateNew,
                      enum Controller::signals signal);
  void dtm_wday_higher(uint8_t stateOld,
                       uint8_t stateNew,
                       enum Controller::signals signal);
  void dtm_set_time(uint8_t stateOld,
                    uint8_t stateNew,
                    enum Controller::signals signal);

 private:
  Display* _display;

  unsigned long _newDate;
  unsigned long _newTime;
  uint8_t _newWDay;
  enum setMode {
    sm_year,
    sm_month,
    sm_date,
    sm_wday,
    sm_hours,
    sm_minutes,
    sm_seconds
  };
  enum setMode _currentSetMode;
};

#endif /* MODULES_CLOCKMODULE_H_ */
