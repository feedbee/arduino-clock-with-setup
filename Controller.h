/*
 * Controller.h
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "TimeHelper.h"
#include "ButtonBlock.h"
#include "Display.h"

#define CTRL_STATES_COUNT 11

class Controller {
 public:

  enum signals {
    btn_no_signal = 0,
    btn_A_cancel,
    btn_B_left,
    btn_C_right,
    btn_D_enter,
    cnt_sg
  };

  // ---

  struct CallBase {
    void operator()(uint8_t state, enum signals signal) {
      Call(state, signal);
    }
   protected:
    virtual void Call(uint8_t state, enum signals signal) = 0;
  };

  template<class T>
  class IndirectCall : public CallBase {
   public:
    typedef void (T::*Method)(uint8_t, enum signals signal);
    IndirectCall(T* target, Method method)
        : target(target),
          method(method) {
    }
   private:
    T* target;
    Method method;
    virtual void Call(uint8_t state, enum signals signal) override
    {
      (target->*method)(state, signal);
    }
  };

  template<class T>
  IndirectCall<T>* CreateIndirectCall(T* ptr,
                                      typename IndirectCall<T>::Method method) {
    return new IndirectCall<T>(ptr, method);
  }

  // ---

  // ---

  struct CallBaseTr {
    void operator()(uint8_t stateOld, uint8_t stateNew, enum signals signal) {
      Call(stateOld, stateNew, signal);
    }
   protected:
    virtual void Call(uint8_t stateOld, uint8_t stateNew,
                      enum signals signal) = 0;
  };

  template<class T>
  class IndirectCallTr : public CallBaseTr {
   public:
    typedef void (T::*Method)(uint8_t, uint8_t, enum signals signal);
    IndirectCallTr(T* target, Method method)
        : target(target),
          method(method) {
    }
   private:
    T* target;
    Method method;
    virtual void Call(uint8_t stateOld, uint8_t stateNew, enum signals signal)
        override
        {
      (target->*method)(stateOld, stateNew, signal);
    }
  };

  template<class T>
  IndirectCallTr<T>* CreateIndirectCallTr(
      T* ptr, typename IndirectCallTr<T>::Method method) {
    return new IndirectCallTr<T>(ptr, method);
  }

  // ---

  struct transitionInfo {
    uint8_t newState;
    CallBaseTr* transition;
  };

  Controller(ButtonBlock *bbb, Display *display)
      : _bb(bbb),
        _display(display) {
  }

  void loop(void);

  void setState(const uint8_t state);

  void addLoop(const uint8_t state, CallBase *callback);
  void addStateTr(const uint8_t state, enum signals signal,
                  struct transitionInfo trInfo);

 private:
  ButtonBlock* _bb;
  Display* _display;
  uint8_t _currentState = 0;

  transitionInfo _stateMatrix[CTRL_STATES_COUNT][cnt_sg];

  CallBase* loops[CTRL_STATES_COUNT];

  enum signals _buttonToSignalMap[5] = { btn_no_signal, btn_D_enter,
      btn_C_right, btn_B_left, btn_A_cancel };
};

#endif /* CONTROLLER_H_ */
