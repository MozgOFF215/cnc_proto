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
      if (state.X_isZeroFound)
        moveTo(0);
      else
        startZeroSeek();
    }

    if (codenum == 280)
    {
      if (!state.X_isWorkspaceKnown)
        startResearch();
      else
        SHOW_MESSAGE((String) "Workspace alredy known. max=" + state.X_maxPos);
    }

    if (codenum == 281)
      startResearch();

    if (codenum == 555)
    {
      String mon = "endstops: X-=";
      mon += (END1 ? "1" : "0");
      mon += " X+=";
      mon += (END2 ? "1" : "0");
      SHOW_MESSAGE(mon);
      SHOW_MESSAGE((String) "position" + state.X_currentPos);
    }
  }
}