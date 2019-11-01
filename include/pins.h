#ifndef _PINS_H_
#define _PINS_H_

#define p_led LED_BUILTIN // PIN_D6 led
#define p_set0 PIN_F7     // research work area

// X axis

#define X_turnFwd PIN_C6
#define X_turnBwd PIN_C7

#define X_enc1 PIN_D2 // encoder 1 (INT)
#define X_enc2 PIN_D4 // encoder 2 (no INT) (previously had D3!!!)

#define X_end1 PIN_D0 // endstop - (INT)
#define X_end2 PIN_D1 // endstop + (INT)

#define X_enA PIN_B6 // ENA (PWM for motor A)

// Y axis

#define Y_turnFwd PIN_C4
#define Y_turnBwd PIN_C5

#define Y_enc1 PIN_D3 // encoder 1 (INT)
#define Y_enc2 PIN_D5 // encoder 2 (no INT)

#define Y_end1 PIN_E6 // endstop - (INT)
#define Y_end2 PIN_E7 // endstop + (INT)

#define Y_enB PIN_B5 // ENA (PWM for motor A)

#endif // _PINS_H_