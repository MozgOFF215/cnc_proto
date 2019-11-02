#include "header.h"
#include "state.h"

State::State(const char name[8])
{
  axis_name[0] = name[0];
  axis_name[1] = name[1];
  axis_name[2] = name[2];
  axis_name[3] = name[3];
  axis_name[4] = name[4];
  axis_name[5] = name[5];
  axis_name[6] = name[6];
  axis_name[7] = name[7];
}

// Create a global instance of the State singleton
State X_state("Axis X"); // Axis X