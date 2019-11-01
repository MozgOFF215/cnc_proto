#ifndef _STATE_H_1
#define _STATE_H_1

struct State
{
  // X axis

  long X_currentPos = 0;
  long X_destinationPos = 0;
  bool X_isStoped = true;

  long X_minPos = -9999;
  long X_maxPos = 9999;

  bool X_isWorkspaceKnown = false;
  bool X_isZeroFound = false;

  moveDirection X_currentDirect = FORWARD;

  search_steps X_workspaceResearchMode = NO_PROCESS;
  search_steps X_zeroSearchMode = NO_PROCESS;

  void (*X_endMovingFunction)();

  // Y axis

  long Y_currentPos = 0;
  long Y_destinationPos = 0;
  bool Y_isStoped = true;

  long Y_minPos = -9999;
  long Y_maxPos = 9999;

  bool Y_isWorkspaceKnown = false;
  bool Y_isZeroFound = false;

  moveDirection Y_currentDirect = FORWARD;

  search_steps Y_workspaceResearchMode = NO_PROCESS;
  search_steps Y_zeroSearchMode = NO_PROCESS;

  void (*Y_endMovingFunction)();
};

extern State state;

#endif // _STATE_H_1