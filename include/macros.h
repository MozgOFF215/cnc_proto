
#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef TEST_PC_CPP
#define SHOW_MESSAGE(a) Serial.println(a)
#else
#define SHOW_MESSAGE(a) println(a)
#endif

#define END1 digitalRead(X_end1)
#define END2 digitalRead(X_end2)

#endif // _MACROS_H_
