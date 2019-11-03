#include "header.h"
#include "controller.h"

pidState::pidState(const char name[8])
{
  axis_name[0] = name[0];
  axis_name[1] = name[1];
  axis_name[2] = name[2];
  axis_name[3] = name[3];
  axis_name[4] = name[4];
  axis_name[5] = name[5];
  axis_name[6] = name[6];
  axis_name[7] = name[7];
}
pidState X_pidState("Axis X");

void initController(pidState *ps)
{
  ps->kP = 1;
}

void controller(Config *cfg, State *st, pidState *ps)
{
  // axis X

  //if (!st->isStoped)
  {
    //if (st->currentPos != st->destinationPos)
    {
      getX_MV(cfg, st, ps);
    }
  }

  // axis Y
}

void getX_MV(Config *cfg, State *st, pidState *ps)
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
    ps->isFirstCycle = false;
  }

  ps->prevTime = now;
  ps->prevDeltaTime = deltaTime;

  long e = st->destinationPos - st->currentPos;
  if (e == 0 && ps->prevE == 0)
  {
    ps->MV = newMV;
    stop(cfg, st, "end of moving by PID");
    return;
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

  if (MV > cfg->maxSpeed)
    newMV.pwm = cfg->maxSpeed;
  else
    newMV.pwm = MV;

  ps->MV = newMV;
}