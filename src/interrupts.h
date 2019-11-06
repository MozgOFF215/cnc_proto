#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#ifndef TEST_PC_CPP
#include <Arduino.h>
#else
#include "arduino.h"
#endif

void initInterrupts();
void X_encoderInterrupt();
void X_endStopInterrupt();

#endif // _INTERRUPTS_H_