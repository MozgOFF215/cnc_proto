#include "header.h"
#include "controller.h"

pidState::pidState(const char name[8])
{
  for (int i = 0; i < 8; i++)
    axis_name[i] = name[i];
}
pidState X_pidState("Axis X");

void initController(pidState *ps)
{
  ps->kP = 1;
  ps->kI = 0.1;
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
  if (e == 0 && ps->prevE == 0)
  {
    ps->MV = newMV;
    stop(cfg, st, "end of moving by PID");
    return;
  }

  float pMV = ps->kP * e;
  float iMV = ps->prevIntg + (deltaTime * e / 1000000.0) * ps->kI; // 1000000 - while µS
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

  if (ps->MV.pwm > 0)
  {
    if (ps->MV.pwm > cfg->maxSpeed)
      analogWrite(X_enA, cfg->maxSpeed);
    else
      analogWrite(X_enA, ps->MV.pwm);

    if (ps->MV.direction == FORWARD)
    {
      digitalWrite(X_turnFwd, HIGH);
      digitalWrite(X_turnBwd, LOW);
    }
    else
    {
      digitalWrite(X_turnFwd, LOW);
      digitalWrite(X_turnBwd, HIGH);
    }
  }
}