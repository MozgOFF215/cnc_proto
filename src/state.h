#ifndef _STATE_H_
#define _STATE_H_

struct State
{
  long currentPos = 0;
  long requiredPos = 0;

  bool currentDirect = true;
  bool isStoped = true;
};

extern State state;

#endif // _STATE_H_