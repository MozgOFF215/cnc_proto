#include "header.h"
#include "controller.h"

pidState X_pidState;

void initController(pidState *ps)
{
  ps->kP = 1;
}

void controller(Config *cfg, State *st, pidState *ps)
{
  // axis X

  if (!st->isStoped)
  {
    if (st->currentPos != st->destinationPos)
    {
      pidMV pidpv = getX_MV(cfg, st, ps);
    }
  }

  // axis Y
}

pidMV getX_MV(Config *cfg, State *st, pidState *ps)
{
  pidMV newMV;
  newMV.direction = FORWARD;
  newMV.pwm = 0;

  uint32_t now = micros();
  uint32_t deltaTime = now - ps->prevTime;

  if (now < ps->prevTime)
  {
    // overflow timer ~50 minuts -> take prev delta
    deltaTime = ps->prevDeltaTime;
  }

  if (ps->isFirstCycle)
  {
    // first cycle of regulation
    deltaTime = 0;
  }

  ps->prevTime = now;
  ps->prevDeltaTime = deltaTime;

  long e = st->destinationPos - st->currentPos;
  if (e == 0 && ps->prevE == 0)
  {
    return newMV;
  }

  float pMV = ps->kP * e;
  float iMV = ps->prevIntg + ps->kI * deltaTime * e / 1000000; // 1000000 - while µS
  float dMV = 0;
  if (deltaTime != 0)
    dMV = ps->kD * (e - ps->prevE) / deltaTime;

  ps->prevIntg = iMV;
  ps->prevE = e;

  float MV = pMV + iMV + dMV;

  newMV.direction = BACKWARD;
  if (MV >= 0)
    newMV.direction = FORWARD;
  else
    MV = -MV;

  if (MV > cfg->X_maxSpeed)
    newMV.pwm = cfg->X_maxSpeed;
  else
    newMV.pwm = MV;

  return newMV;
}