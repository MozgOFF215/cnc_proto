#ifndef _STATE_H_
#define _STATE_H_

#include "header.h"

struct State
{
  long currentPos = 0;
  long requiredPos = 0;

  long minPos = -9999;
  long maxPos = 9999;

  moveDirection currentDirect = FORWARD;

  bool isStoped = true;

  wsr_step workspaceResearchMode = NO_RESEARCH;

  void (*endMovingFunction)();
};

extern State state;

#endif // _STATE_H_