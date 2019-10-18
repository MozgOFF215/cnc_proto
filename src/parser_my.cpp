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

  if (letter == 'X')
    moveTo(codenum);

  if (letter == 'G')
  {
    if (codenum == 28)
    {
      if (state.isZeroFound)
        moveTo(0);
      else
        startResearch();
    }
  }
}