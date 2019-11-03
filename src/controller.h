#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

struct pidMV
{
  byte pwm;
  moveDirection direction;
};

class pidState
{
public:
  pidState(const char name[8]);

  char axis_name[8];

  long prevPos;
  long prevIntg;
  long prevE;

  unsigned long prevTime; // µS
  long prevDeltaTime;

  bool isFirstCycle = true;

  float kP;
  float kI;
  float kD;

  pidMV MV;
};

void initController(pidState *ps);
void getX_MV(Config *cfg, State *st, pidState *ps);
void controller(Config *cfg, State *st, pidState *ps);

extern pidState X_pidState;

#endif // _CONTROLLER_H_