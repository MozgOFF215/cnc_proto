#include "header.h"
#include "moving.h"

void moveTo(long pos)
{
  if (state.currentPos == pos)
    return;

  if (state.currentPos < pos)
  {
    if (END2)
    {
      SHOW_MESSAGE("enstop2 active!!!");
      return;
    }

    if ((pos - state.currentPos) < config.slowDistance)
      goForward(pos, config.minSpeed);
    else
      goForward(pos, config.maxSpeed);
    return;
  }

  if (END1)
  {
    SHOW_MESSAGE("enstop1 active!!!");
    return;
  }

  if ((state.currentPos - pos) < config.slowDistance)
    goBack(pos, config.minSpeed);
  else
    goBack(pos, config.maxSpeed);
}

void goForward(long pos, int speed)
{
  SHOW_MESSAGE((String) "Forward to:" + pos + " current:" + state.currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(p_turnFwd, HIGH);
  digitalWrite(p_turnBwd, LOW);
  setSpeed(speed);
  state.requiredPos = pos;
  state.currentDirect = FORWARD;
  state.isStoped = false;
}

void goBack(long pos, int speed)
{
  SHOW_MESSAGE((String) "Backward to:" + pos + " current:" + state.currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(p_turnFwd, LOW);
  digitalWrite(p_turnBwd, HIGH);
  setSpeed(speed);
  state.requiredPos = pos;
  state.currentDirect = BACKWARD;
  state.isStoped = false;
}

void stop(String reason)
{
  digitalWrite(p_led, LOW);
  digitalWrite(p_turnFwd, LOW);
  digitalWrite(p_turnBwd, LOW);
  setSpeed(config.maxSpeed);
  state.isStoped = true;

  SHOW_MESSAGE((String) "Stop" + (reason.length() > 0 ? "(" + reason + ")" : "") + ". Required:" + state.requiredPos + " current:" + state.currentPos);
}

int g_lastSpeed = 0;

void setSpeed(int speed)
{
  if (speed != g_lastSpeed)
  {
    SHOW_MESSAGE((String) "--- set speed:" + speed + " current:" + state.currentPos);
    analogWrite(p_enA, speed);
    g_lastSpeed = speed;
  }
}

void endMoving()
{
  //
}