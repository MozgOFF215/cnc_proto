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

    if ((pos - state.currentPos) < config.X_slowDistance)
      goForward(pos, config.X_minSpeed);
    else
      goForward(pos, config.X_maxSpeed);
    return;
  }

  if (END1)
  {
    SHOW_MESSAGE("enstop1 active!!!");
    return;
  }

  if ((state.currentPos - pos) < config.X_slowDistance)
    goBack(pos, config.X_minSpeed);
  else
    goBack(pos, config.X_maxSpeed);
}

void goForward(long pos, int speed)
{
  SHOW_MESSAGE((String) "Forward to:" + pos + " current:" + state.currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(X_turnFwd, HIGH);
  digitalWrite(X_turnBwd, LOW);
  setSpeed(speed);
  state.requiredPos = pos;
  state.currentDirect = FORWARD;
  state.isStoped = false;
}

void goBack(long pos, int speed)
{
  SHOW_MESSAGE((String) "Backward to:" + pos + " current:" + state.currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(X_turnFwd, LOW);
  digitalWrite(X_turnBwd, HIGH);
  setSpeed(speed);
  state.requiredPos = pos;
  state.currentDirect = BACKWARD;
  state.isStoped = false;
}

void stop(String reason)
{
  digitalWrite(p_led, LOW);
  digitalWrite(X_turnFwd, LOW);
  digitalWrite(X_turnBwd, LOW);
  setSpeed(config.X_maxSpeed);
  state.isStoped = true;

  SHOW_MESSAGE((String) "Stop" + (reason.length() > 0 ? "(" + reason + ")" : "") + ". Required:" + state.requiredPos + " current:" + state.currentPos);
}

int g_lastSpeed = 0;

void setSpeed(int speed)
{
  if (speed != g_lastSpeed)
  {
    SHOW_MESSAGE((String) "--- set speed:" + speed + " current:" + state.currentPos);
    analogWrite(X_enA, speed);
    g_lastSpeed = speed;
  }
}

void endMoving()
{
  //
}