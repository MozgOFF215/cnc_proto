#ifndef _CONFIG_H_
#define _CONFIG_H_

struct Config
{
  const int X_timeWait = 200;
  const int X_timeDrive = 5000;
  const int X_minSpeed = 200;
  const int X_maxSpeed = 255;
  const long X_slowDistance = 200;
  const int X_stopendProtectDistance = 50;
};

extern Config config;

#endif // _CONFIG_H_