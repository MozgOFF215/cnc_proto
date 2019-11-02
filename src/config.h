#ifndef _CONFIG_H_
#define _CONFIG_H_

struct Config
{
  // X axis

  const int X_timeWait = 200;
  const int X_timeDrive = 5000;
  const int X_minSpeed = 200;
  const int X_maxSpeed = 255;
  const long X_slowDistance = 200;
  const int X_stopendProtectDistance = 50;

  // Y axis

  const int Y_timeWait = 200;
  const int Y_timeDrive = 5000;
  const int Y_minSpeed = 200;
  const int Y_maxSpeed = 255;
  const long Y_slowDistance = 200;
  const int Y_stopendProtectDistance = 50;
};

extern Config X_config;
#endif // _CONFIG_H_