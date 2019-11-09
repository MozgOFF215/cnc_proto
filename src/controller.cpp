#include "controller.h"
#include "math.h"

pidState X_pidState("Axis X");
pidState Y_pidState("Axis Y");

void initControllerX(pidState *ps)
{
  ps->kP = 0.2;
  ps->kI = 0;
  ps->kD = 0;
}

void initControllerY(pidState *ps)
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
  double deltaTime = now - ps->ts;

  if (now < ps->ts)
  {
    // overflow timer ~50 minuts -> take prev delta
    deltaTime = ps->dt;
  }

  if (ps->isFirstCycle)
  {
    // first cycle of regulation
    deltaTime = 0;
  }

  ps->ts = now;
  ps->dt = deltaTime;

  long e = st->destinationPos - st->currentPos;

  double pMV = ps->kP * e;

  double iMV;
  double ii = (deltaTime * e / 1000000.0) * ps->kI; // 1000000 - while µS
  iMV = ps->iMV + ii;

  double dMV = 0;
  if (deltaTime != 0)
  {
    long de = e - ps->e;
    if (de != 0)
      dMV = ps->kD * (e - ps->e) / deltaTime * 1000000.0;
  }

  ps->pMV = pMV;
  ps->iMV = iMV;
  ps->dMV = dMV;
  ps->e = e;

  double MV = pMV + iMV + dMV;

  newMV.direction = BACKWARD;
  if (MV >= 0)
    newMV.direction = FORWARD;
  else
    MV = -MV;

  newMV.pwm = MV;

  ps->MV = newMV;
}

long prevTime = 0;
void apply_MV(Config *cfg, State *st, pidState *ps)
{
  int mv = 0;

  if (ps->MV.pwm != 0)
  {
    if (ps->isFirstCycle)
    {
      // first cycle of regulation
      ps->isFirstCycle = false;
    }

    mv = ps->MV.pwm;
    if (mv < cfg->getMinSpeed() || st->workspaceResearchMode != NO_PROCESS || st->zeroSearchMode != NO_PROCESS)
      mv = cfg->getMinSpeed();

#ifndef TEST_PC_CPP
      //SHOW_MESSAGE((String) + "#pos " + st->currentPos + " mv " + mv + " MV.pwm  " + (ps->MV.direction ? -ps->MV.pwm : ps->MV.pwm) + " e " + ps->prevE);
#else
    if (!no_prompt)
      printf("#pos %ld mv %d MV.pwm %ld e %ld\n", st->currentPos, mv, (ps->MV.direction ? -ps->MV.pwm : ps->MV.pwm), ps->e);
#endif

    if (mv > cfg->getMaxSpeed())
      mv = cfg->getMaxSpeed();
  }
  else
  {
    if (!ps->isFirstCycle)
    {
      SHOW_MESSAGE((String)ps->axis_name + " stopped! in " + (micros() - prevTime));
      prevTime = micros();
      ps->isFirstCycle = true;
    }
  }

  if (ps->MV.direction == FORWARD)
  {
    if (!st->isStoped)
    {
      cfg->SetPWM(mv);
      cfg->TurnFWD();
    }
    else
    {
      if (!cfg->IsEndPlus() && !cfg->IsEndMinus())
      {
        st->isStoped = 0;
      }
      else
      {
        if (!cfg->IsEndPlus())
        {
          cfg->SetPWM(mv);
          cfg->TurnFWD();
        }
      }
    }
  }
  else
  {
    if (!st->isStoped)
    {
      cfg->SetPWM(mv);
      cfg->TurnBWD();
    }
    else
    {
      if (!cfg->IsEndPlus() && !cfg->IsEndMinus())
      {
        st->isStoped = 0;
      }
      else
      {
        if (!cfg->IsEndMinus())
        {
          cfg->SetPWM(mv);
          cfg->TurnBWD();
        }
      }
    }
  }
}