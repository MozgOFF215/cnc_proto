#ifndef _CONFIG_H_
#define _CONFIG_H_

class Config
{
public:
  Config(const char name[8]);

  char axis_name[8];
  const int timeWait = 200;
  const int timeDrive = 5000;
  const int minSpeed = 200;
  const int maxSpeed = 255;
  const long slowDistance = 200;
  const int stopendProtectDistance = 50;
};

extern Config X_config;
#endif // _CONFIG_H_