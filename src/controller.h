#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

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
  float prevIntg;

  unsigned long prevTime; // µS
  long prevDeltaTime;

  bool isFirstCycle = true;

  float kP;
  float kI;
  float kD;

  pidMV MV;
};

void initController(pidState *ps);
void controller(Config *cfg, State *st, pidState *ps);
void update_MV(Config *cfg, State *st, pidState *ps);
void apply_MV(Config *cfg, State *st, pidState *ps);

extern pidState X_pidState;

#endif // _CONTROLLER_H_