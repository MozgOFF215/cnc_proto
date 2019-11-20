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

class State;

typedef void (*callback)(State *);

class State
{
private:
#ifndef TEST_PC_CPP
  uint8_t pin_enc1, pin_enc2, pin_turnFwd, pin_turnBwd, pin_pwm, pin_end1, pin_end2;
  uint8_t pin_dir, pin_step;
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
  double kAxis;
  long fromPos = 0; // backup current pos before new motion
  long destinationPos = 0;

  bool wait = false;
  callback endMovingFunction = NULL;
  callback temp_callback = NULL;

  bool _isStepper = false;
  uint32_t periodStepper = 2000; // µs
  uint32_t timeToNext = 0;
  bool limitIsKnown = false;
  uint8_t lastStep = 0;
  uint8_t lastDir = 0;

  void waitNextStep()
  {
    timeToNext = micros() + periodStepper;
  }

  bool timeForNextStep()
  {
    uint32_t currentTime = micros();
    if (timeToNext <= periodStepper)
    {
      //with overflow
      return timeToNext <= currentTime && currentTime <= periodStepper;
    }
    else
    {
      return timeToNext <= currentTime || currentTime <= periodStepper;
    }
  }

public:
  const int stopendProtectDistance = 50;

  int getMinSpeed() { return minSpeed; }
  int getMaxSpeed() { return maxSpeed; }

  char axis_name;
#ifndef TEST_PC_CPP
  State(char name, uint8_t pin_enc1, uint8_t pin_enc2, uint8_t pin_turnFwd, uint8_t pin_turnBwd,
        uint8_t pin_pwm, uint8_t pin_end1, uint8_t pin_end2, int minSpeed, int maxSpeed, double kAxis)
  {
    axis_name = name;

    State::pin_enc1 = pin_enc1;
    State::pin_enc2 = pin_enc2;
    State::pin_turnFwd = pin_turnFwd;
    State::pin_turnBwd = pin_turnBwd;
    State::pin_pwm = pin_pwm;
    State::pin_end1 = pin_end1;
    State::pin_end2 = pin_end2;

    State::minSpeed = minSpeed;
    State::maxSpeed = maxSpeed;

    State::kAxis = kAxis;

    currentPos = 0;
    destinationPos = 0;
    endMovingFunction = NULL;
  }

  State(char name, uint8_t pin_dir, uint8_t pin_step, uint8_t pin_end2, double kAxis)
  {
    // stepper motor axis

    axis_name = name;
    State::pin_end2 = pin_end2;
    State::pin_dir = pin_dir;
    State::pin_step = pin_step;

    State::kAxis = kAxis;

    currentPos = 0;
    destinationPos = 0;
    endMovingFunction = NULL;
    _isStepper = true;

    timeToNext = micros() + periodStepper + periodStepper;
  }
#else
  State(char name, const char pin_enc1[8], const char pin_enc2[8], const char pin_turnFwd[8], const char pin_turnBwd[8],
        const char pin_pwm[8], const char pin_end1[8], const char pin_end2[8], int minSpeed, int maxSpeed, double kAxis)
  {
    axis_name = name;

    copy(pin_enc1, State::pin_enc1, 8);
    copy(pin_enc2, State::pin_enc2, 8);
    copy(pin_turnFwd, State::pin_turnFwd, 8);
    copy(pin_turnBwd, State::pin_turnBwd, 8);
    copy(pin_pwm, State::pin_pwm, 8);
    copy(pin_end1, State::pin_end1, 8);
    copy(pin_end2, State::pin_end2, 8);

    State::minSpeed = minSpeed;
    State::maxSpeed = maxSpeed;

    State::kAxis = kAxis;

    destinationPos = 0;
    endMovingFunction = NULL;
  }
#endif

  long currentPos = 0;
  bool isStoped = true; // starting value = true

  long minPos = -9999;
  long maxPos = 9999;

  bool isWorkspaceKnown = false;
  bool isZeroFound = false;

  moveDirection currentDirect = FORWARD;

  search_steps workspaceResearchMode = NO_PROCESS;
  search_steps zeroSearchMode = NO_PROCESS;

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

  bool isStepper() { return _isStepper; }

  void NextStep()
  {
    if (timeForNextStep())
    {
      waitNextStep();

      if (destinationPos > currentPos)
      {
        // step to plus
        if (lastDir != 1)
        {
          digitalWrite(pin_dir, 1);
          lastDir = 1;
          return; // need time for new direction init
        }

        if (IsEndPlus())
        {
          if (zeroSearchMode == PLUS_ENDSTOP_SEARCH)
          {
            // plus limit is found
            zeroSearchMode = PLUS_ENDSTOP_LEAVE;
            destinationPos = currentPos - 100;
            return;
          }

          if (!isStoped)
          {
            SHOW_MESSAGE("echo: plus endstop is active!!!");
            isStoped = true;
          }
          return;
        }

        currentPos++;
        lastStep = (lastStep + 1) & 1;
        digitalWrite(pin_step, lastStep);
      }

      if (destinationPos < currentPos)
      {
        // step to minus
        if (lastDir != 0)
        {
          digitalWrite(pin_dir, 0);
          lastDir = 0;
          return; // need time for new direction init
        }

        if (zeroSearchMode == PLUS_ENDSTOP_LEAVE && !IsEndPlus())
        {
          // leave plus endstop
          zeroSearchMode = NO_PROCESS;
          long posMax = FromSI(25);
          currentPos = posMax + 50;
          goTo_Strokes(posMax, popTempCallback());
        }

        currentPos--;
        lastStep = (lastStep + 1) & 1;
        digitalWrite(pin_step, lastStep);
      }

      checkSuccessfulMove();
    }
  }

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
    if (isStepper())
      return false;

    return digitalRead(pin_end1);
  }

  bool IsEndPlus()
  {
    if (isStepper())
      return !digitalRead(pin_end2);

    return digitalRead(pin_end2);
  }

  long FromSI(double mm)
  {
    return mm * kAxis;
  }

  double ToSI(long pos)
  {
    return pos / kAxis;
  }

  long getDestination() { return destinationPos; }

  void goTo(double posMM)
  {
    long newPos = FromSI(posMM);
    destinationPos = newPos;
    fromPos = currentPos;
  }

  void goTo(double posMM, callback endMovingFunction)
  {
    State::endMovingFunction = endMovingFunction;
    wait = true;
    goTo(posMM);
  }

  void goTo_Strokes(long pos)
  {
    SHOW_MESSAGE((String) "to pos:" + pos + " current:" + currentPos);
    destinationPos = pos;
    fromPos = currentPos;
  }

  void goTo_Strokes(long pos, callback endMovingFunction)
  {
    State::endMovingFunction = endMovingFunction;
    wait = true;
    goTo_Strokes(pos);
  }

  bool isWait() { return wait; }

  unsigned long lastCheck;
  unsigned long waitSomeTime = 50; // ms

  bool checkSuccessfulMove()
  {
    bool isSuccess;

    if (isStepper())
    {
      isSuccess = destinationPos == currentPos;
    }
    else
    {
      if (MV.pwm != 0)
        lastCheck = millis();

      bool noMoveSomeTime = millis() - lastCheck > waitSomeTime && MV.pwm == 0;

      isSuccess = fromPos < destinationPos ? currentPos >= destinationPos : currentPos <= destinationPos;
      if (fromPos < destinationPos)
      {
        if (currentPos >= destinationPos || noMoveSomeTime)
        {
          isSuccess = true;
        }
      }
      else
      {
        if (currentPos <= destinationPos || noMoveSomeTime)
        {
          isSuccess = true;
        }
      }
    }

    if (isSuccess)
    {
      wait = false;
      if (endMovingFunction != NULL)
      {
        (*endMovingFunction)(this);
        endMovingFunction = NULL;
      }
    }
    return isSuccess;
  }

  void pushTempCallback(callback temp_callback)
  {
    State::temp_callback = temp_callback;
  }

  callback popTempCallback()
  {
    return temp_callback;
  }
};

extern State X_state;
extern State Y_state;
extern State Z_state;

#endif // _STATE_H_1