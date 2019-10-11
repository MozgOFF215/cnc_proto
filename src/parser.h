#ifndef _PARSER_H_
#define _PARSER_H_

#include "header.h"

#define WITHIN(V, L, H) ((V) >= (L) && (V) <= (H))
#define NUMERIC(a) WITHIN(a, '0', '9')

void parse(const char *p);

#endif // _PARSER_H_