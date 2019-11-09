#include "state.h"

State::State(const char name[8])
{
  for (int i = 0; i < 8; i++)
    axis_name[i] = name[i];
}

// Create a global instance of the State singleton
State X_state("Axis X"); // Axis X
State Y_state("Axis Y");