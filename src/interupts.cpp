#include "interrupts.h"
#include "pins.h"
#include "state.h"
#include "seek_0.h"
#include "workspace_research.h"

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
}

void X_endStopInterrupt()
{
  if (X_config.IsEndMinus() || X_config.IsEndPlus())
  {
    X_config.Stop(&X_state, X_config.IsEndMinus() ? "endstop '-' activate!" : "endstop '+' activate!");

    if (X_state.workspaceResearchMode != NO_PROCESS)
      foundEndStop(&X_config, &X_state);
    if (X_state.zeroSearchMode != NO_PROCESS)
      foundEndStop_0(&X_config, &X_state);
  }
  else
  {
    if (X_state.workspaceResearchMode == SEEK_0 || X_state.workspaceResearchMode == SEEK_MAX)
    {
      X_config.Stop(&X_state, "leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop(&X_config, &X_state);
    }
    if (X_state.zeroSearchMode == SEEK_0)
    {
      X_config.Stop(&X_state, "leave endstop");
      SHOW_MESSAGE("leave endstop");
      leaveEndStop_0(&X_config, &X_state);
    }
  }
}