#include "controller.h"
#include "math.h"

void initControllerX()
{
  X_state.kP = 0.2;
  X_state.kI = 0;
  X_state.kD = 0;
}

void initControllerY()
{
  Y_state.kP = 0.3; //0.4;
  Y_state.kI = 0;   //0.1;
  Y_state.kD = 0;   //0.001;
}

void initControllerZ()
{
  Z_state.kP = 0.3; //0.4;
  Z_state.kI = 0;   //0.1;
  Z_state.kD = 0;   //0.001;
}

void controller(State *st)
{
  if (st->isStepper())
    st->NextStep();
  else
  {
    update_MV(st);
    apply_MV(st);
  }
}

void update_MV(State *st)
{
  pidMV newMV;
  newMV.direction = FORWARD;
  newMV.pwm = 0;

  uint32_t now = micros();
  double deltaTime = now - st->ts;

  if (now < st->ts)
  {
    // overflow timer ~50 minuts -> take prev delta
    deltaTime = st->dt;
  }

  if (st->isFirstCycle)
  {
    // first cycle of regulation
    deltaTime = 0;
  }

  st->ts = now;
  st->dt = deltaTime;

  long e = st->getDestination() - st->currentPos;

  double pMV = st->kP * e;

  double iMV;
  double ii = (deltaTime * e / 1000000.0) * st->kI; // 1000000 - while µS
  iMV = st->iMV + ii;

  double dMV = 0;
  if (deltaTime != 0)
  {
    long de = e - st->e;
    if (de != 0)
      dMV = st->kD * (e - st->e) / deltaTime * 1000000.0;
  }

  st->pMV = pMV;
  st->iMV = iMV;
  st->dMV = dMV;
  st->e = e;

  double MV = pMV + iMV + dMV;

  newMV.direction = BACKWARD;
  if (MV >= 0)
    newMV.direction = FORWARD;
  else
    MV = -MV;

  newMV.pwm = MV;

  st->MV = newMV;
}

void apply_MV(State *st)
{
  bool isSuccess = st->checkSuccessfulMove();

  int mv = 0;

  if (st->MV.pwm != 0)
  {
    if (st->isFirstCycle)
    {
      // first cycle of regulation
      st->isFirstCycle = false;
    }

    mv = st->MV.pwm;
    if (mv < st->getMinSpeed() || st->workspaceResearchMode != NO_PROCESS || st->zeroSearchMode != NO_PROCESS)
      mv = st->getMinSpeed();

#ifndef TEST_PC_CPP
      //SHOW_MESSAGE((String) + "#pos " + st->currentPos + " mv " + mv + " MV.pwm  " + (ps->MV.direction ? -ps->MV.pwm : ps->MV.pwm) + " e " + ps->prevE);
#else
    if (!no_prompt)
      printf("#pos %ld mv %d MV.pwm %ld e %ld\n", st->currentPos, mv, (st->MV.direction ? -st->MV.pwm : st->MV.pwm), st->e);
#endif

    if (mv > st->getMaxSpeed())
      mv = st->getMaxSpeed();
  }
  else
  {
    if (!st->isFirstCycle)
    {
      //SHOW_MESSAGE((String)st->axis_name + " stopped! pos " + st->ToSI(st->currentPos) + " e " + st->e + " isEnd " + isSuccess);
      st->isFirstCycle = true;
    }
  }

  if (st->MV.direction == FORWARD)
  {
    if (!st->isStoped)
    {
      st->SetPWM(mv);
      st->TurnFWD();
    }
    else
    {
      if (!st->IsEndPlus() && !st->IsEndMinus())
      {
        st->isStoped = 0;
      }
      else
      {
        if (!st->IsEndPlus())
        {
          st->SetPWM(mv);
          st->TurnFWD();
        }
      }
    }
  }
  else
  {
    if (!st->isStoped)
    {
      st->SetPWM(mv);
      st->TurnBWD();
    }
    else
    {
      if (!st->IsEndPlus() && !st->IsEndMinus())
      {
        st->isStoped = 0;
      }
      else
      {
        if (!st->IsEndMinus())
        {
          st->SetPWM(mv);
          st->TurnBWD();
        }
      }
    }
  }
}
