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

  attachInterrupt(Y_enc1, Y_encoderInterrupt, CHANGE);
  attachInterrupt(Y_end1, Y_endStopInterrupt, CHANGE);
  attachInterrupt(Y_end2, Y_endStopInterrupt, CHANGE);
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
  if (X_state.IsEndMinus() || X_state.IsEndPlus())
  {
    X_state.Stop(&X_state, X_state.IsEndMinus() ? "endstop -X is active!" : "endstop +X is active!");

    if (X_state.workspaceResearchMode != NO_PROCESS)
      foundEndStop(&X_state);
    if (X_state.zeroSearchMode != NO_PROCESS)
      foundEndStop_0(&X_state);
  }
  else
  {
    if (X_state.workspaceResearchMode == SEEK_0 || X_state.workspaceResearchMode == SEEK_MAX)
    {
      X_state.Stop(&X_state, "leave endstop X");
      SHOW_MESSAGE("leave endstop X");
      leaveEndStop(&X_state);
    }
    if (X_state.zeroSearchMode == SEEK_0)
    {
      X_state.Stop(&X_state, "leave endstop X");
      SHOW_MESSAGE("leave endstop X");
      leaveEndStop_0(&X_state);
    }
  }
}

void Y_encoderInterrupt()
{
  if (digitalRead(Y_enc1) && digitalRead(Y_enc2))
    Y_state.currentPos++;
  else
    Y_state.currentPos--;

  if (!digitalRead(Y_enc1) && digitalRead(Y_enc2))
    Y_state.currentPos--;
  else
    Y_state.currentPos++;
}

void Y_endStopInterrupt()
{
  if (Y_state.IsEndMinus() || Y_state.IsEndPlus())
  {
    Y_state.Stop(&Y_state, Y_state.IsEndMinus() ? "endstop -Y is active!" : "endstop +Y is active!");

    if (Y_state.workspaceResearchMode != NO_PROCESS)
      foundEndStop(&Y_state);
    if (Y_state.zeroSearchMode != NO_PROCESS)
      foundEndStop_0(&Y_state);
  }
  else
  {
    if (Y_state.workspaceResearchMode == SEEK_0 || Y_state.workspaceResearchMode == SEEK_MAX)
    {
      Y_state.Stop(&Y_state, "leave endstop Y");
      SHOW_MESSAGE("leave endstop Y");
      leaveEndStop(&Y_state);
    }
    if (Y_state.zeroSearchMode == SEEK_0)
    {
      Y_state.Stop(&Y_state, "leave endstop Y");
      SHOW_MESSAGE("leave endstop Y");
      leaveEndStop_0(&Y_state);
    }
  }
}