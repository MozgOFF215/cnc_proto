#include "header.h"

void startResearch(Config *cfg, State *st)
{
  if (cfg->IsEndMinus() || cfg->IsEndPlus())
  {
    SHOW_MESSAGE("not possible to research: need to leave the endstops");
    return;
  }
  SHOW_MESSAGE("1. Start workspace research. Search left endstop.");
  st->workspaceResearchMode = LEFT_ENDSTOP_SEARCH;
  st->destinationPos = -9999;
}

void foundEndStop(Config *cfg, State *st)
{
  if (st->workspaceResearchMode == LEFT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    st->workspaceResearchMode = SEEK_0;
    st->destinationPos = 100;
  }

  if (st->workspaceResearchMode == RIGHT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE((String) "4. right endstop found: " + st->currentPos);
    SHOW_MESSAGE("4. seek max");
    st->workspaceResearchMode = SEEK_MAX;
    st->destinationPos = st->currentPos - 100;
  }
}

void leaveEndStop(Config *cfg, State *st)
{
  if (st->workspaceResearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    st->currentPos = -cfg->stopendProtectDistance;
    st->minPos = 0;
    st->isZeroFound = true;

    SHOW_MESSAGE("3. Search right endstop.");
    st->workspaceResearchMode = RIGHT_ENDSTOP_SEARCH;
    st->destinationPos = 9999;
  }

  if (st->workspaceResearchMode == SEEK_MAX)
  {
    st->maxPos = st->currentPos - cfg->stopendProtectDistance;
    SHOW_MESSAGE((String) "5. Max found: " + st->maxPos);

    SHOW_MESSAGE("5. end workspace research");
    st->workspaceResearchMode = NO_PROCESS;
    st->isWorkspaceKnown = true;

    SHOW_MESSAGE("5. go to 0");
    st->destinationPos = 0;
  }
}