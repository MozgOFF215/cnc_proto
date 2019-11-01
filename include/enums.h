#pragma once

enum moveDirection
{
  FORWARD,
  BACKWARD
};

enum search_steps
{
  NO_PROCESS,
  LEFT_ENDSTOP_SEARCH,
  SEEK_0,
  RIGHT_ENDSTOP_SEARCH,
  SEEK_MAX
};