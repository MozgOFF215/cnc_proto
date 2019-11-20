#pragma once

enum moveDirection
{
  FORWARD,
  BACKWARD
};

enum search_steps
{
  NO_PROCESS,
  MINUS_ENDSTOP_SEARCH,
  SEEK_0,
  PLUS_ENDSTOP_SEARCH,
  PLUS_ENDSTOP_LEAVE,
  SEEK_MAX
};