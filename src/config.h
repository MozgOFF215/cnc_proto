#ifndef _CONFIG_H_
#define _CONFIG_H_

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
#include "state.h"

class Config
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

public:
  const int minSpeed = 129;
  const int maxSpeed = 180;
  const int stopendProtectDistance = 50;

  char axis_name[8];
#ifndef TEST_PC_CPP
  Config(const char name[8], uint8_t pin_enc1, uint8_t pin_enc2, uint8_t pin_turnFwd, uint8_t pin_turnBwd,
         uint8_t pin_pwm, uint8_t pin_end1, uint8_t pin_end2)
  {
    copy(name, axis_name, 8);

    Config::pin_enc1 = pin_enc1;
    Config::pin_enc2 = pin_enc2;
    Config::pin_turnFwd = pin_turnFwd;
    Config::pin_turnBwd = pin_turnBwd;
    Config::pin_pwm = pin_pwm;
    Config::pin_end1 = pin_end1;
    Config::pin_end2 = pin_end2;
  }
#else
  Config(const char name[8], const char pin_enc1[8], const char pin_enc2[8], const char pin_turnFwd[8], const char pin_turnBwd[8],
         const char pin_pwm[8], const char pin_end1[8], const char pin_end2[8])
  {
    copy(name, axis_name, 8);

    copy(pin_enc1, Config::pin_enc1, 8);
    copy(pin_enc2, Config::pin_enc2, 8);
    copy(pin_turnFwd, Config::pin_turnFwd, 8);
    copy(pin_turnBwd, Config::pin_turnBwd, 8);
    copy(pin_pwm, Config::pin_pwm, 8);
    copy(pin_end1, Config::pin_end1, 8);
    copy(pin_end2, Config::pin_end2, 8);
  }
#endif

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

extern Config X_config;
#endif // _CONFIG_H_