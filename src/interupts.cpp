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
    state.X_currentPos++;
  else
    state.X_currentPos--;
  if (!digitalRead(X_enc1) && digitalRead(X_enc2))
    state.X_currentPos--;
  else
    state.X_currentPos++;

  if (state.X_isStoped)
    return;

  if (!state.X_isStoped)
  {
    if (state.X_currentDirect == BACKWARD)
    {
      if (state.X_currentPos <= state.X_destinationPos)
      {
        stop("c<=r");

        // if (state.X_endMovingFunction != nullptr)
        //   state.X_endMovingFunction();

        return;
      }

      if ((state.X_currentPos - state.X_destinationPos) < config.X_slowDistance)
        setSpeed(config.X_minSpeed);
    }

    if (state.X_currentDirect == FORWARD)
    {
      if (state.X_currentPos >= state.X_destinationPos)
      {
        stop("c>=r");

        // if (state.X_endMovingFunction != nullptr)
        //   state.X_endMovingFunction();

        return;
      }

      if ((state.X_destinationPos - state.X_currentPos) < config.X_slowDistance)
        setSpeed(config.X_minSpeed);
    }
  }
}

void endStopInterrupt()
{
  if (END1 || END2)
  {
    stop((String) "endstop" + (END1 ? "1" : "2"));

    if (state.X_workspaceResearchMode != NO_PROCESS)
      foundEndStop();
    if (state.X_zeroSearchMode != NO_PROCESS)
      foundEndStop_0();
  }
  else
  {
    if (state.X_workspaceResearchMode == SEEK_0 || state.X_workspaceResearchMode == SEEK_MAX)
    {
      stop("leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop();
    }
    if (state.X_zeroSearchMode == SEEK_0)
    {
      stop("leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop_0();
    }
  }
}