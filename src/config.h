#ifndef _CONFIG_H_
#define _CONFIG_H_

struct Config
{
  const int timeWait = 200;
  const int timeDrive = 5000;
  const int minSpeed = 200;
  const int maxSpeed = 255;
  const long slowDistance = 200;
  const int stopendProtectDistance = 50;
};

extern Config config;

#endif // _CONFIG_H_