#include "header.h"
#include "config.h"

Config::Config(const char name[8])
{
  for (int i = 0; i < 8; i++)
    axis_name[i] = name[i];
}

// Create a global instance of the Config singleton
Config X_config("Axis X"); // Axis X
