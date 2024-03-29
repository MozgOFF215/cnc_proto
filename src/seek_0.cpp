#include "macros.h"
#include "seek_0.h"

void startZeroSeek(State *st, callback endMovingFunction)
{
  st->pushTempCallback(endMovingFunction);

  if (st->isStepper())
  {
    SHOW_MESSAGE("start seek Z '+' endstop.");
    if (st->IsEndPlus())
    {
      SHOW_MESSAGE("not possible zero seek: need to leave the '+' endstop.");
      return;
    }
    st->goTo_Strokes(9999L);
    st->zeroSearchMode = PLUS_ENDSTOP_SEARCH;
    return;
  }

  if (st->IsEndMinus() /*|| END2 */)
  {
    SHOW_MESSAGE("not possible zero seek: need to leave the '-' endstop.");
    return;
  }
  SHOW_MESSAGE("1. Start zero seek. Search '-' endstop.");
  st->zeroSearchMode = MINUS_ENDSTOP_SEARCH;
  st->goTo_Strokes(-9999L);
}

void foundEndStop_0(State *st)
{
  if (st->zeroSearchMode == MINUS_ENDSTOP_SEARCH)
  {
    SHOW_MESSAGE("2. Seek 0");
    st->zeroSearchMode = SEEK_0;
    st->goTo_Strokes(100L);
  }
}

void leaveEndStop_0(State *st)
{
  if (st->zeroSearchMode == SEEK_0)
  {
    SHOW_MESSAGE("3. 0 found");
    st->currentPos = -st->stopendProtectDistance;
    st->minPos = 0;
    st->isZeroFound = true;

    st->zeroSearchMode = NO_PROCESS;

    SHOW_MESSAGE("4. go to 0");
    st->goTo_Strokes(0L, &nullIsOk_0);
  }
}

void nullIsOk_0(State *st)
{
  SHOW_MESSAGE("5. I am on the 0!");
  (*st->popTempCallback())(st);
}