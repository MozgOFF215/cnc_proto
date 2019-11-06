#include "parser_my.h"
#include "state.h"
#include "config.h"
#include "workspace_research.h"
#include "seek_0.h"

parserCode getCode(const char *p)
{
  parserCode result;
  result.letter = '?';
  result.codenum = 0;
  result.pNextSymbol = nullptr;

  if (*p == 0)
    return result;

  // Skip spaces
  while (*p == ' ')
    ++p;

  if (*p == 0)
    return result;

  const char letter = *p >= 'a' && *p <= 'z' ? *p & 0xDF : *p; // to upper case
  p++;

  int codenum = 0;
  if (NUMERIC(*p))
    do
    {
      codenum *= 10;
      codenum += *p++ - '0';
    } while (NUMERIC(*p));

  result.letter = letter;
  result.codenum = codenum;

  if (*p != 0)
    result.pNextSymbol = p;

  if (*p == 0)
    return result;

  return result;
}

void parse_my(const char *p)
{
  parserCode c = getCode(p);

  if (c.letter == 'X') // moveTo(&X_config, &X_state, codenum); - this is old!
    X_state.destinationPos = c.codenum;

  if (c.letter == 'G')
  {
#ifndef TEST_PC_CPP
    if (c.codenum == 28)
    {
      if (X_state.isZeroFound)
        X_state.destinationPos = 0l;
      else
        startZeroSeek(&X_config, &X_state);
    }

    if (c.codenum == 280)
    {
      if (!X_state.isWorkspaceKnown)
        startResearch(&X_config, &X_state);
      else
        SHOW_MESSAGE((String) "Workspace alredy known. max=" + X_state.maxPos);
    }

    if (c.codenum == 281)
      startResearch(&X_config, &X_state);
#endif

    if (c.codenum == 555)
    {
#ifndef TEST_PC_CPP
      String mon = "endstops: X-=";
      mon += (X_config.IsEndMinus() ? "1" : "0");
      mon += " X+=";
      mon += (X_config.IsEndPlus() ? "1" : "0");
      SHOW_MESSAGE(mon);
      SHOW_MESSAGE((String) "position" + X_state.currentPos);
#else
      printf("endstops: X-=%s X+=%s", X_config.IsEndMinus() ? "ON" : "OFF", X_config.IsEndPlus() ? "ON" : "OFF");
      printf("position %ld", X_state.currentPos);
#endif
    }
  }
}