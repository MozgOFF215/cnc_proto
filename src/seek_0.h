#ifndef _SEEK_0_H_
#define _SEEK_0_H_

#include "state.h"

void startZeroSeek(State *st, callback endMovingFunction);
void foundEndStop_0(State *st);
void leaveEndStop_0(State *st);
void nullIsOk_0(State *st);

#endif // _SEEK_0_H_