
#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef TEST_PC_CPP
#define SHOW_MESSAGE(a) Serial.println(a)
#else
#define SHOW_MESSAGE(a) printf(a)
#endif

#endif // _MACROS_H_
