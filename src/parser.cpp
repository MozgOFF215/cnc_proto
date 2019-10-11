#include "header.h"
#include "main.h"
#include "parser.h"

void parse(const char *p)
{
  // Skip spaces
  while (*p == ' ')
    ++p;

  const char letter = *p++;

  int codenum = 0;
  do
  {
    codenum *= 10, codenum += *p++ - '0';
  } while (NUMERIC(*p));

  Serial.println(codenum, DEC);
  Serial.println(letter);

  //if (letter == 'P')
  {
    Serial.println("moveTo...");
    moveTo(codenum);
  }
}