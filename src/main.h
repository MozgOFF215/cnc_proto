#ifndef _MAIN_H_
#define _MAIN_H_

void moveTo(long pos);
void stop();
void goBack(long pos, int speed);
void goForward(long pos, int speed);
void encoderInterrupt();
void endStopInterrupt();

#endif // _MAIN_H_