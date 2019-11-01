#include "header.h"
#include "controller.h"

struct pidState
{
  long prevPos;
  long prevIntg;
  long prevE;

  unsigned long prevTime; // µS
  long prevDeltaTime;

  bool isFirstCycle = true;

  float kP;
  float kI;
  float kD;
};

pidState X_pidState;

void initController()
{
  X_pidState.kP = 1;
}

void controller()
{
  // axis X

  if (!state.X_isStoped)
  {
    if (state.X_currentPos != state.X_destinationPos)
    {
      pidMV pidpv = getX_MV();
    }
  }

  // axis Y
}

pidMV getX_MV()
{
  pidMV newMV;
  newMV.direction = FORWARD;
  newMV.pwm = 0;

  uint32_t now = micros();
  uint32_t deltaTime = now - X_pidState.prevTime;

  if (now < X_pidState.prevTime)
  {
    // overflow timer ~50 minuts -> take prev delta
    deltaTime = X_pidState.prevDeltaTime;
  }

  if (X_pidState.isFirstCycle)
  {
    // first cycle of regulation
    deltaTime = 0;
  }

  X_pidState.prevTime = now;
  X_pidState.prevDeltaTime = deltaTime;

  long e = state.X_destinationPos - state.X_currentPos;
  if (e == 0 && X_pidState.prevE == 0)
  {
    return newMV;
  }

  float pMV = X_pidState.kP * e;
  float iMV = X_pidState.prevIntg + X_pidState.kI * deltaTime * e / 1000000; // 1000000 - while µS
  float dMV = 0;
  if (deltaTime != 0)
    dMV = X_pidState.kD * (e - X_pidState.prevE) / deltaTime;

  X_pidState.prevIntg = iMV;
  X_pidState.prevE = e;

  float MV = pMV + iMV + dMV;

  newMV.direction = BACKWARD;
  if (MV >= 0)
    newMV.direction = FORWARD;
  else
    MV = -MV;

  if (MV > config.X_maxSpeed)
    newMV.pwm = config.X_maxSpeed;
  else
    newMV.pwm = MV;

  return newMV;
}