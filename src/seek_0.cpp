#include "header.h"

void startZeroSeek()
{
  if (END1 /*|| END2 */)
  {
    SHOW_MESSAGE("not possible zero seek: need to leave the left endstop.");
    return;
  }
  SHOW_MESSAGE("1. Start zero seek. Search left endstop.");
  state.zeroSearchMode = LEFT_ENDSTOP_SEARCH;
  goBack(-9999, config.X_maxSpeed);
}

void foundEndStop_0()
{
  if (state.zeroSearchMode == LEFT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    state.zeroSearchMode = SEEK_0;
    goForward(100, config.X_minSpeed);
  }
}

void leaveEndStop_0()
{
  if (state.workspaceResearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    state.currentPos = -config.X_stopendProtectDistance;
    state.minPos = 0;
    state.isZeroFound = true;

    state.workspaceResearchMode = NO_PROCESS;

    SHOW_MESSAGE("4. go to 0");
    moveTo(0);
  }
}