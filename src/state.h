#ifndef _STATE_H_
#define _STATE_H_

#include "header.h"

enum moveDirection{
  FORWARD,
  BACKWARD
};

struct State
{
  long currentPos = 0;
  long requiredPos = 0;
  bool isStoped = true;

  long minPos = -9999;
  long maxPos = 9999;

  bool isWorkspaceKnown = false;
  bool isZeroFound = false;

  moveDirection currentDirect = FORWARD;

  search_steps workspaceResearchMode = NO_PROCESS;
  search_steps zeroSearchMode = NO_PROCESS;

  void (*endMovingFunction)();
};

extern State state;

#endif // _STATE_H_