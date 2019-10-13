#include "header.h"
#include "main.h"

void parse_my(const char *p)
{
  // Skip spaces
  while (*p == ' ')
    ++p;

  const char letter = *p++;

  int codenum = 0;
  do
  {
    codenum *= 10;
    codenum += *p++ - '0';
  } while (NUMERIC(*p));

  Serial.println(codenum, DEC);
  Serial.println(letter);

  if (letter == 'P')
  {
    SHOW_MESSAGE("moveTo...");
    moveTo(codenum);
  }

  if (letter == 'G')
  {
    if (codenum == 28) startResearch();
  }
}