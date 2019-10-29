#ifndef _PINS_H_
#define _PINS_H_

#include <Arduino.h>

#define p_led LED_BUILTIN // PIN_D6 led

#define X_turnFwd PIN_C6
#define X_turnBwd PIN_C7

#define X_enc1 PIN_D2 // encoder 1 (INT)
#define X_enc2 PIN_D3 // encoder 2 (no INT)

#define X_end1 PIN_D0 // endstop 1 (INT)
#define X_end2 PIN_D1 // endstop 2 (INT)

#define X_enA PIN_B6 // ENA (PWM for motor A)

#define p_set0 PIN_F7 // research work area

#endif // _PINS_H_