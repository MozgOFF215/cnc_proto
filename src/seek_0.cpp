#include "header.h"

void startZeroSeek(Config *cfg, State *st)
{
  if (END1 /*|| END2 */)
  {
    SHOW_MESSAGE("not possible zero seek: need to leave the left endstop.");
    return;
  }
  SHOW_MESSAGE("1. Start zero seek. Search left endstop.");
  st->zeroSearchMode = LEFT_ENDSTOP_SEARCH;
  goBack(st, -9999, cfg->maxSpeed);
}

void foundEndStop_0(Config *cfg, State *st)
{
  if (st->zeroSearchMode == LEFT_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    st->zeroSearchMode = SEEK_0;
    goForward(st, 100, cfg->minSpeed);
  }
}

void leaveEndStop_0(Config *cfg, State *st)
{
  if (st->zeroSearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    st->currentPos = -cfg->stopendProtectDistance;
    st->minPos = 0;
    st->isZeroFound = true;

    st->zeroSearchMode = NO_PROCESS;

    SHOW_MESSAGE("4. go to 0");
    moveTo(cfg, st, 0);
  }
}