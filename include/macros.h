
#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef TEST_PC_CPP
#define SHOW_MESSAGE(a) Serial.println(a)
#else
#define SHOW_MESSAGE(a) printf(a)
#endif

#ifndef TEST_PC_CPP
#define END1 digitalRead(X_end1)
#define END2 digitalRead(X_end2)
#else
#define END1 0//digitalRead(X_end1)
#define END2 0//digitalRead(X_end2)
#endif

#endif // _MACROS_H_
