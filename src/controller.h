#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

struct pidMV
{
  byte pwm;
  moveDirection direction;
};

void initController();
pidMV getX_MV();
void controller();

#endif // _CONTROLLER_H_