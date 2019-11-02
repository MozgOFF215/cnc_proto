#include "header.h"
#include "interrupts.h"

void initInterrupts()
{
  attachInterrupt(X_enc1, X_encoderInterrupt, CHANGE);
  attachInterrupt(X_end1, X_endStopInterrupt, CHANGE);
  attachInterrupt(X_end2, X_endStopInterrupt, CHANGE);
}

void X_encoderInterrupt()
{
  if (digitalRead(X_enc1) && digitalRead(X_enc2))
    X_state.currentPos++;
  else
    X_state.currentPos--;
  if (!digitalRead(X_enc1) && digitalRead(X_enc2))
    X_state.currentPos--;
  else
    X_state.currentPos++;

  if (X_state.isStoped)
    return;

  if (!X_state.isStoped)
  {
    if (X_state.currentDirect == BACKWARD)
    {
      if (X_state.currentPos <= X_state.destinationPos)
      {
        stop(&X_config, &X_state, "c<=r");

        // if (state.X_endMovingFunction != nullptr)
        //   state.X_endMovingFunction();

        return;
      }

      if ((X_state.currentPos - X_state.destinationPos) < X_config.X_slowDistance)
        setSpeed(&X_state, X_config.X_minSpeed);
    }

    if (X_state.currentDirect == FORWARD)
    {
      if (X_state.currentPos >= X_state.destinationPos)
      {
        stop(&X_config, &X_state, "c>=r");

        // if (state.X_endMovingFunction != nullptr)
        //   state.X_endMovingFunction();

        return;
      }

      if ((X_state.destinationPos - X_state.currentPos) < X_config.X_slowDistance)
        setSpeed(&X_state, X_config.X_minSpeed);
    }
  }
}

void X_endStopInterrupt()
{
  if (END1 || END2)
  {
    stop(&X_config, &X_state, (String) "endstop" + (END1 ? "1" : "2"));

    if (X_state.workspaceResearchMode != NO_PROCESS)
      foundEndStop(&X_config, &X_state);
    if (X_state.zeroSearchMode != NO_PROCESS)
      foundEndStop_0(&X_config, &X_state);
  }
  else
  {
    if (X_state.workspaceResearchMode == SEEK_0 || X_state.workspaceResearchMode == SEEK_MAX)
    {
      stop(&X_config, &X_state, "leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop(&X_config, &X_state);
    }
    if (X_state.zeroSearchMode == SEEK_0)
    {
      stop(&X_config, &X_state, "leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop_0(&X_config, &X_state);
    }
  }
}