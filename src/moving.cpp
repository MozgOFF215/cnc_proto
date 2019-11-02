#include "header.h"
#include "moving.h"

void moveTo(Config *cfg, State *st, long pos)
{
  if (st->currentPos == pos)
    return;

  if (st->currentPos < pos)
  {
    if (END2)
    {
      SHOW_MESSAGE("enstop2 active!!!");
      return;
    }

    if ((pos - st->currentPos) < cfg->X_slowDistance)
      goForward(st, pos, cfg->X_minSpeed);
    else
      goForward(st, pos, cfg->X_maxSpeed);
    return;
  }

  if (END1)
  {
    SHOW_MESSAGE("enstop1 active!!!");
    return;
  }

  if ((st->currentPos - pos) < cfg->X_slowDistance)
    goBack(st, pos, cfg->X_minSpeed);
  else
    goBack(st, pos, cfg->X_maxSpeed);
}

void goForward(State *st, long pos, int speed)
{
  SHOW_MESSAGE((String) "Forward to:" + pos + " current:" + st->currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(X_turnFwd, HIGH);
  digitalWrite(X_turnBwd, LOW);
  setSpeed(st, speed);
  st->destinationPos = pos;
  st->currentDirect = FORWARD;
  st->isStoped = false;
}

void goBack(State *st, long pos, int speed)
{
  SHOW_MESSAGE((String) "Backward to:" + pos + " current:" + st->currentPos + " speed: " + speed);

  digitalWrite(p_led, HIGH);
  digitalWrite(X_turnFwd, LOW);
  digitalWrite(X_turnBwd, HIGH);
  setSpeed(st, speed);
  st->destinationPos = pos;
  st->currentDirect = BACKWARD;
  st->isStoped = false;
}

void stop(Config *cfg, State *st, String reason)
{
  digitalWrite(p_led, LOW);
  digitalWrite(X_turnFwd, LOW);
  digitalWrite(X_turnBwd, LOW);
  setSpeed(st, cfg->X_maxSpeed);
  st->isStoped = true;

  SHOW_MESSAGE((String) "Stop" + (reason.length() > 0 ? "(" + reason + ")" : "") + ". Required:" + st->destinationPos + " current:" + st->currentPos);
}

int g_lastSpeed = 0;

void setSpeed(State *st, int speed)
{
  if (speed != g_lastSpeed)
  {
    SHOW_MESSAGE((String) "--- set speed:" + speed + " current:" + st->currentPos);
    analogWrite(X_enA, speed);
    g_lastSpeed = speed;
  }
}

void endMoving()
{
  //
}