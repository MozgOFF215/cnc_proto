#include "header.h"
#include "interrupts.h"

void initInterrupts()
{
  attachInterrupt(X_enc1, encoderInterrupt, CHANGE);
  attachInterrupt(X_end1, endStopInterrupt, CHANGE);
  attachInterrupt(X_end2, endStopInterrupt, CHANGE);
}

void encoderInterrupt()
{
  if (digitalRead(X_enc1) && digitalRead(X_enc2))
    state.currentPos++;
  else
    state.currentPos--;
  if (!digitalRead(X_enc1) && digitalRead(X_enc2))
    state.currentPos--;
  else
    state.currentPos++;

  if (state.isStoped)
    return;

  if (!state.isStoped)
  {
    if (state.currentDirect == BACKWARD)
    {
      if (state.currentPos <= state.requiredPos)
      {
        stop("c<=r");

        // if (state.endMovingFunction != nullptr)
        //   state.endMovingFunction();

        return;
      }

      if ((state.currentPos - state.requiredPos) < config.X_slowDistance)
        setSpeed(config.X_minSpeed);
    }

    if (state.currentDirect == FORWARD)
    {
      if (state.currentPos >= state.requiredPos)
      {
        stop("c>=r");

        // if (state.endMovingFunction != nullptr)
        //   state.endMovingFunction();

        return;
      }

      if ((state.requiredPos - state.currentPos) < config.X_slowDistance)
        setSpeed(config.X_minSpeed);
    }
  }
}

void endStopInterrupt()
{
  if (END1 || END2)
  {
    stop((String) "endstop" + (END1 ? "1" : "2"));

    if (state.workspaceResearchMode != NO_PROCESS)
      foundEndStop();
    if (state.zeroSearchMode != NO_PROCESS)
      foundEndStop_0();
  }
  else
  {
    if (state.workspaceResearchMode == SEEK_0 || state.workspaceResearchMode == SEEK_MAX)
    {
      stop("leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop();
    }
    if (state.zeroSearchMode == SEEK_0)
    {
      stop("leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop_0();
    }
  }
}