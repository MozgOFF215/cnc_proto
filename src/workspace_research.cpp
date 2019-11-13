#include "workspace_research.h"

void startResearch(State *st, callback endMovingFunction)
{
  st->pushTempCallback(endMovingFunction);

  if (st->IsEndMinus() || st->IsEndPlus())
  {
    SHOW_MESSAGE("not possible to research: need to leave the endstops");
    return;
  }
  SHOW_MESSAGE("1. Start workspace research. Search left endstop.");
  st->workspaceResearchMode = LEFT_ENDSTOP_SEARCH;
  st->goTo_Strokes(-9999L);
}

void foundEndStop(State *st)
{
  if (st->workspaceResearchMode == LEFT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    st->workspaceResearchMode = SEEK_0;
    st->goTo_Strokes(100L);
  }

  if (st->workspaceResearchMode == RIGHT_ENDSTOP_SEARCH)
  {
#ifndef TEST_PC_CPP
    SHOW_MESSAGE((String) "4. right endstop found: " + st->currentPos);
#else
    printf("4. right endstop found: %ld\n", st->currentPos);
#endif
    SHOW_MESSAGE("4. seek max");
    st->workspaceResearchMode = SEEK_MAX;
    st->goTo_Strokes(st->currentPos - 100);
  }
}

void leaveEndStop(State *st)
{
  if (st->workspaceResearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    st->currentPos = -st->stopendProtectDistance;
    st->minPos = 0;
    st->isZeroFound = true;

    SHOW_MESSAGE("3. Search right endstop.");
    st->workspaceResearchMode = RIGHT_ENDSTOP_SEARCH;
    st->goTo_Strokes(9999L);
  }

  if (st->workspaceResearchMode == SEEK_MAX)
  {
    st->maxPos = st->currentPos - st->stopendProtectDistance;
#ifndef TEST_PC_CPP
    SHOW_MESSAGE((String) "5. Max found: " + st->maxPos);
#else
    printf("5. Max found: %ld", st->maxPos);
#endif

    SHOW_MESSAGE("5. end workspace research");
    st->workspaceResearchMode = NO_PROCESS;
    st->isWorkspaceKnown = true;

    SHOW_MESSAGE("5. go to 0");
    st->goTo_Strokes(0L, &nullIsOk);
  }
}

void nullIsOk(State *st)
{
  SHOW_MESSAGE("6. I am on the 0!");
  (*st->popTempCallback())(st);
}