#include "header.h"
#include "interrupts.h"

void initInterrupts()
{
  attachInterrupt(p_enc1, encoderInterrupt, CHANGE);
  attachInterrupt(p_end1, endStopInterrupt, CHANGE);
  attachInterrupt(p_end2, endStopInterrupt, CHANGE);
}

void encoderInterrupt()
{
  if (digitalRead(p_enc1) && digitalRead(p_enc2))
    state.currentPos++;
  else
    state.currentPos--;
  if (!digitalRead(p_enc1) && digitalRead(p_enc2))
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

      if ((state.currentPos - state.requiredPos) < config.slowDistance)
        setSpeed(config.minSpeed);
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

      if ((state.requiredPos - state.currentPos) < config.slowDistance)
        setSpeed(config.minSpeed);
    }
  }
}

void endStopInterrupt()
{
  if (END1 || END2)
  {
    stop((String) "endstop" + (END1 ? "1" : "2"));

    if (state.workspaceResearchMode != NO_RESEARCH)
      foundEndStop();
  }
  else
  {
    if (state.workspaceResearchMode == SEEK_0 || state.workspaceResearchMode == SEEK_MAX)
    {
      stop("leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop();
    }
  }
}