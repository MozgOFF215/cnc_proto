#ifndef _STATE_H_1
#define _STATE_H_1

class State
{
public:
  char axis_name[8];
  State(const char name[8]);

  long currentPos = 0;
  long destinationPos = 0;
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

extern State X_state;

#endif // _STATE_H_1