#ifndef _MOVING_H_
#define _MOVING_H_

void moveTo(Config *cfg, State *st, long pos);
void goForward(State *st, long pos, int speed);
void goForward(State *st, long pos, int speed);
void goBack(State *st, long pos, int speed);
void stop(Config *cfg, State *st, String reason);
void setSpeed(State *st, int speed);
void endMoving();

#endif // _MOVING_H_