#ifndef _STATE_H_
#define _STATE_H_

#include "header.h"

struct State
{
  long currentPos = 0;
  long requiredPos = 0;
  bool isStoped = true;

  long minPos = -9999;
  long maxPos = 9999;

  bool isZeroFound = false;

  moveDirection currentDirect = FORWARD;

  wsr_step workspaceResearchMode = NO_RESEARCH;

  void (*endMovingFunction)();
};

extern State state;

#endif // _STATE_H_