#include "header.h"

void startResearch()
{
  if (END1 || END2)
  {
    SHOW_MESSAGE("not possible to research: need to leave the endstops");
    return;
  }
  SHOW_MESSAGE("1. Start workspace research. Search left endstop.");
  state.workspaceResearchMode = LEFT_ENDSTOP_SEARCH;
  goBack(-9999, config.maxSpeed);
}

void foundEndStop()
{
  if (state.workspaceResearchMode == LEFT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    state.workspaceResearchMode = SEEK_0;
    goForward(100, config.minSpeed);
  }

  if (state.workspaceResearchMode == RIGHT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE((String) "4. right endstop found: " + state.currentPos);
    SHOW_MESSAGE("4. seek max");
    state.workspaceResearchMode = SEEK_MAX;
    goBack(state.currentPos - 100, config.minSpeed);
  }
}

void leaveEndStop()
{
  if (state.workspaceResearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    state.currentPos = -config.stopendProtectDistance;
    state.minPos = 0;
    state.isZeroFound = true;

    SHOW_MESSAGE("3. Search right endstop.");
    state.workspaceResearchMode = RIGHT_ENDSTOP_SEARCH;
    goForward(9999, config.maxSpeed);
  }

  if (state.workspaceResearchMode == SEEK_MAX)
  {
    state.maxPos = state.currentPos - config.stopendProtectDistance;
    SHOW_MESSAGE((String) "5. Max found: " + state.maxPos);

    SHOW_MESSAGE("5. end workspace research");
    state.workspaceResearchMode = NO_RESEARCH;

    SHOW_MESSAGE("5. go to 0");
    goBack(0, config.maxSpeed);
  }
}