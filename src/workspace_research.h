#ifndef _WORKSPACE_RESEARCH_H_
#define _WORKSPACE_RESEARCH_H_

#include "state.h"

void startResearch(State *st, callback endMovingFunction);
void foundEndStop(State *st);
void leaveEndStop(State *st);
void nullIsOk(State *st);

#endif // _WORKSPACE_RESEARCH_H_