/*
 * Controller.cpp
 */

#include <Arduino.h>

#include "Controller.h"
#include "Modules/ClockModule.h"

void Controller::setState(const uint8_t state) {
  _currentState = state;
}

void Controller::addLoop(const uint8_t state, CallBase *callback) {
  loops[state] = callback;
}

void Controller::addStateTr(const uint8_t state, enum signals signal,
                            struct transitionInfo trInfo) {
  _stateMatrix[state][signal] = trInfo;
}

void Controller::loop(void) {
  unsigned char b = _bb->getPressedButton();
  enum signals signal = _buttonToSignalMap[b];
  if (signal && _stateMatrix[_currentState][signal].newState) {
    transitionInfo tri = _stateMatrix[_currentState][signal];
    uint8_t oldMode = _currentState;
    _currentState = tri.newState;
    if (tri.transition) {
      (*tri.transition)(oldMode, _currentState, signal);
    }
  }

  auto callback = loops[_currentState];
  if (!callback) {
    _display->clear();
    _display->println("Invalid mode");
    return;
  }
  (*callback)(_currentState, signal);
}
