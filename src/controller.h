#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "state.h"
#include "config.h"
#include "enums.h"

struct pidMV
{
  long pwm;
  moveDirection direction;
};

class pidState
{
public:
  pidState(const char name[8])
  {
    for (int i = 0; i < 8; i++)
      axis_name[i] = name[i];
  }

  char axis_name[8];

  long prevPos;
  long prevE;
  double prevIntg;

  unsigned long prevTime; // µS
  long prevDeltaTime;

  bool isFirstCycle = true;

  double kP;
  double kI;
  double kD;

  pidMV MV;
};

void initController(pidState *ps);
void controller(Config *cfg, State *st, pidState *ps);
void update_MV(Config *cfg, State *st, pidState *ps);
void apply_MV(Config *cfg, State *st, pidState *ps);

extern pidState X_pidState;

#endif // _CONTROLLER_H_