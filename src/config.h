#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>

class Config
{
private:
  uint8_t pin_enc1, pin_enc2, pin_turnFwd, pin_turnBwd, pin_pwm, pin_end1, pin_end2;

public:
  const int minSpeed = 129;
  const int maxSpeed = 180;
  const int stopendProtectDistance = 50;

  char axis_name[8];
  Config(const char name[8], uint8_t pin_enc1, uint8_t pin_enc2, uint8_t pin_turnFwd, uint8_t pin_turnBwd,
         uint8_t pin_pwm, uint8_t pin_end1, uint8_t pin_end2)
  {
    for (int i = 0; i < 8; i++)
      axis_name[i] = name[i];

    Config::pin_enc1 = pin_enc1;
    Config::pin_enc2 = pin_enc2;
    Config::pin_turnFwd = pin_turnFwd;
    Config::pin_turnBwd = pin_turnBwd;
    Config::pin_pwm = pin_pwm;
    Config::pin_end1 = pin_end1;
    Config::pin_end2 = pin_end2;
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
    return digitalRead(pin_end1);
  }

  bool IsEndPlus()
  {
    return digitalRead(pin_end2);
  }
};

extern Config X_config;
#endif // _CONFIG_H_