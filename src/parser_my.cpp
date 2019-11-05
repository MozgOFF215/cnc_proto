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

  if (letter == 'X') // moveTo(&X_config, &X_state, codenum); - this is old!
    X_state.currentPos = codenum;
    
  if (letter == 'G')
  {
    if (codenum == 28)
    {
      if (X_state.isZeroFound)
        moveTo(&X_config, &X_state, 0);
      else
        startZeroSeek(&X_config, &X_state);
    }

    if (codenum == 280)
    {
      if (!X_state.isWorkspaceKnown)
        startResearch(&X_config, &X_state);
      else
        SHOW_MESSAGE((String) "Workspace alredy known. max=" + X_state.maxPos);
    }

    if (codenum == 281)
      startResearch(&X_config, &X_state);

    if (codenum == 555)
    {
      String mon = "endstops: X-=";
      mon += (END1 ? "1" : "0");
      mon += " X+=";
      mon += (END2 ? "1" : "0");
      SHOW_MESSAGE(mon);
      SHOW_MESSAGE((String) "position" + X_state.currentPos);
    }
  }
}