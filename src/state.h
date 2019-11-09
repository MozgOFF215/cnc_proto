#ifndef _STATE_H_1
#define _STATE_H_1

#ifndef TEST_PC_CPP
#include <Arduino.h>
#else
#include <stdio.h>
#include "arduino.h"
#endif

#ifdef TEST_PC_CPP
#include <stdint.h>
#endif

#include "macros.h"
#include "enums.h"

struct pidMV
{
  long pwm;
  moveDirection direction;
};

class State
{
private:
#ifndef TEST_PC_CPP
  uint8_t pin_enc1, pin_enc2, pin_turnFwd, pin_turnBwd, pin_pwm, pin_end1, pin_end2;
#else
  char pin_enc1[8], pin_enc2[8], pin_turnFwd[8], pin_turnBwd[8], pin_pwm[8], pin_end1[8], pin_end2[8];
#endif

  void copy(const char *s, char *d, int volume)
  {
    for (int i = 0; i < volume; i++)
      *d++ = *s++;
  }

  int minSpeed;
  int maxSpeed;

public:
  const int stopendProtectDistance = 50;

  int getMinSpeed() { return minSpeed; }
  int getMaxSpeed() { return maxSpeed; }

  char axis_name[8];
#ifndef TEST_PC_CPP
  State(const char name[8], uint8_t pin_enc1, uint8_t pin_enc2, uint8_t pin_turnFwd, uint8_t pin_turnBwd,
        uint8_t pin_pwm, uint8_t pin_end1, uint8_t pin_end2, int minSpeed, int maxSpeed)
  {
    copy(name, axis_name, 8);

    State::pin_enc1 = pin_enc1;
    State::pin_enc2 = pin_enc2;
    State::pin_turnFwd = pin_turnFwd;
    State::pin_turnBwd = pin_turnBwd;
    State::pin_pwm = pin_pwm;
    State::pin_end1 = pin_end1;
    State::pin_end2 = pin_end2;

    State::minSpeed = minSpeed;
    State::maxSpeed = maxSpeed;
  }
#else
  State(const char name[8], const char pin_enc1[8], const char pin_enc2[8], const char pin_turnFwd[8], const char pin_turnBwd[8],
        const char pin_pwm[8], const char pin_end1[8], const char pin_end2[8])
  {
    copy(name, axis_name, 8);

    copy(pin_enc1, State::pin_enc1, 8);
    copy(pin_enc2, State::pin_enc2, 8);
    copy(pin_turnFwd, State::pin_turnFwd, 8);
    copy(pin_turnBwd, State::pin_turnBwd, 8);
    copy(pin_pwm, State::pin_pwm, 8);
    copy(pin_end1, State::pin_end1, 8);
    copy(pin_end2, State::pin_end2, 8);
  }
#endif

  long currentPos = 0;
  long destinationPos = 0;
  bool isStoped = true; // starting value = true

  long minPos = -9999;
  long maxPos = 9999;

  bool isWorkspaceKnown = false;
  bool isZeroFound = false;

  moveDirection currentDirect = FORWARD;

  search_steps workspaceResearchMode = NO_PROCESS;
  search_steps zeroSearchMode = NO_PROCESS;

  void (*endMovingFunction)();

// PID
  long position;
  long e;
  double pMV;
  double iMV;
  double dMV;

  unsigned long ts; // time stamp µS
  long dt;          // delta time

  bool isFirstCycle = true;

  double kP;
  double kI;
  double kD;

  pidMV MV;
  // END PID

  void TurnFWD()
  {
    digitalWrite(pin_turnFwd, HIGH);
    digitalWrite(pin_turnBwd, LOW);
  }

  void TurnBWD()
  {
    digitalWrite(pin_turnFwd, LOW);
    digitalWrite(pin_turnBwd, HIGH);
  }

  void SetPWM(uint8_t value)
  {
    analogWrite(pin_pwm, value);
  }

  void Stop(State *st, String reason)
  {
#ifndef TEST_PC_CPP
    SHOW_MESSAGE((String) "Stop" + (reason.length() > 0 ? "(" + reason + ")" : "") + ". Required:" + st->destinationPos + " current:" + st->currentPos);
#else
    if (!no_prompt)
      printf("stop. reason: %s\n", reason);
#endif
    digitalWrite(pin_turnFwd, LOW);
    digitalWrite(pin_turnBwd, LOW);
    SetPWM(maxSpeed);
    st->isStoped = true;
  }

  bool IsEndMinus()
  {
    return digitalRead(pin_end1);
  }

  bool IsEndPlus()
  {
    return digitalRead(pin_end2);
  }
};

extern State X_state;
extern State Y_state;

#endif // _STATE_H_1