#ifndef _PARSER_MY_H_
#define _PARSER_MY_H_

#define WITHIN(V, L, H) ((V) >= (L) && (V) <= (H))
#define NUMERIC(a) WITHIN(a, '0', '9')

struct parserCode
{
  char letter;
  int codenum;
  const char * pNextSymbol;
};

void parse_my(const char *p);
parserCode getCode(const char *p);

#endif // _PARSER_MY_H_