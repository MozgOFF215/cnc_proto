#ifndef _SEEK_0_H_
#define _SEEK_0_H_

#include "state.h"
#include "config.h"

void startZeroSeek(Config *cfg, State *st);
void foundEndStop_0(Config *cfg, State *st);
void leaveEndStop_0(Config *cfg, State *st);

#endif // _SEEK_0_H_