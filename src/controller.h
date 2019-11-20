#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "state.h"
#include "enums.h"

void initControllerX();
void initControllerY();
void initControllerZ();
void controller(State *st);
void update_MV(State *st);
void apply_MV(State *st);

#endif // _CONTROLLER_H_