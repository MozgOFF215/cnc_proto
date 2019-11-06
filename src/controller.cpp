#include "header.h"
#include "controller.h"
#include "math.h"


pidState X_pidState("Axis X");

void initController(pidState *ps)
{
  ps->kP = 0.1;
  ps->kI = 0;
  ps->kD = 0;
}

void controller(Config *cfg, State *st, pidState *ps)
{
  update_MV(cfg, st, ps);
  apply_MV(cfg, st, ps);
}

void update_MV(Config *cfg, State *st, pidState *ps)
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

  float pMV = ps->kP * e;

  float iMV;
  float ii = (deltaTime * e / 1000000.0) * ps->kI;
  iMV = ps->prevIntg + ii; // 1000000 - while µS

  float dMV = 0;
  if (deltaTime != 0)
  {
    long de = e - ps->prevE;
    if (de != 0)
      dMV = ps->kD * (e - ps->prevE) / deltaTime;
  }

  ps->prevIntg = iMV;
  ps->prevE = e;

  float MV = pMV + iMV + dMV;

  newMV.direction = BACKWARD;
  if (MV >= 0)
    newMV.direction = FORWARD;
  else
    MV = -MV;

  newMV.pwm = MV;

  ps->MV = newMV;
}

void apply_MV(Config *cfg, State *st, pidState *ps)
{
  // TODO check endstops

  if (st->isStoped)
  {
    if (ps->MV.pwm > 0)
      st->isStoped = false;
  }

  if (ps->MV.pwm == 0)
  {
    cfg->SetPWM(0);
  }
  else
  {
    int mv = ps->MV.pwm;
    if (mv < cfg->minSpeed)
      mv = cfg->minSpeed;

    SHOW_MESSAGE((String) + "#pos " + st->currentPos + " mv " + mv + " MV.pwm  " + (ps->MV.direction ? -ps->MV.pwm : ps->MV.pwm) + " e " + ps->prevE);

    if (mv > cfg->maxSpeed)
      cfg->SetPWM(cfg->maxSpeed);
    else
      cfg->SetPWM(mv);
  }

  if (ps->MV.direction == FORWARD)
    cfg->TurnFWD();
  else
    cfg->TurnBWD();
}