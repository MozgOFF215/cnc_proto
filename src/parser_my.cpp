#include "parser_my.h"
#include "state.h"
#include "workspace_research.h"
#include "seek_0.h"

Parser parser;

void (*send_temp)(const char *);

void moveIsSuccess(State *st)
{
  if (!X_state.isWait() && !Y_state.isWait())
  {
    (*send_temp)("ok\n");
  }
}

void parse_my(const char *p, void (*send)(const char *))
{
  send_temp = send;

  parser.parse(p);

  if (parser.hasCommand('M'))
  {
    int c = parser.getCommand('M');
    switch (c)
    {
    case 105:
      send("ok B:25 T:35\n");
      break;

    case 104: // temp. extruder Sxxx, not wait
    case 110: // next line
    case 109: // temp. extruder Sxxx, and wait
    case 82:  // set axis of extruder in absolut
    case 107: // fan is off
      send("ok\n");
      break;

    default:
      break;
    }
  }

  if (parser.hasCommand('G'))
  {
    int c = parser.getCommand('G');
    switch (c)
    {
    case 0: //G0
    case 1: //G1
    {
      if (parser.hasCommand('X'))
        X_state.goTo(parser.getCommand('X'), &moveIsSuccess);
      if (parser.hasCommand('Y'))
        Y_state.goTo(parser.getCommand('Y'), &moveIsSuccess);
#ifdef TEST_PC_CPP
      send("ok\n");
#endif
    }
    break;

    case 90: // reset absolut origin
    {
#ifdef TEST_PC_CPP
      send("ok\n");
#endif
    }

    break;
    case 92: // set absolut origin
    {
#ifdef TEST_PC_CPP
      send("ok\n");
#endif
    }
    break;

    case 28:
    {
#ifndef TEST_PC_CPP
      if (X_state.isZeroFound)
      {
        X_state.goTo_Strokes(0L, &moveIsSuccess);
      }
      else
        startZeroSeek(&X_state, &moveIsSuccess);

      if (Y_state.isZeroFound)
      {
        Y_state.goTo_Strokes(0L, &moveIsSuccess);
      }
      else

        startZeroSeek(&Y_state, &moveIsSuccess);
#else
      send("ok\n");
#endif
    }
    break;

    case 280:
    {
#ifndef TEST_PC_CPP
      if (!X_state.isWorkspaceKnown)
        startResearch(&X_state, &moveIsSuccess);
      else
        SHOW_MESSAGE((String) "Workspace X alredy known. max=" + X_state.maxPos);

      if (!Y_state.isWorkspaceKnown)
        startResearch(&Y_state, &moveIsSuccess);
      else
        SHOW_MESSAGE((String) "Workspace Y alredy known. max=" + Y_state.maxPos);
#else
      send("ok\n");
#endif
    }
    break;

    case 281:
    {
#ifndef TEST_PC_CPP
      startResearch(&X_state, &moveIsSuccess);
      startResearch(&Y_state, & moveIsSuccess);
#else
      send("ok\n");
#endif
    }
    break;

    case 555:
    {
#ifndef TEST_PC_CPP
      String mon = "endstops: X-=";
      mon += (X_state.IsEndMinus() ? "1" : "0");
      mon += " X+=";
      mon += (X_state.IsEndPlus() ? "1" : "0");
      SHOW_MESSAGE(mon);
      SHOW_MESSAGE((String) "position X: " + X_state.currentPos);

      mon = "endstops: Y-=";
      mon += (Y_state.IsEndMinus() ? "1" : "0");
      mon += " X+=";
      mon += (Y_state.IsEndPlus() ? "1" : "0");
      SHOW_MESSAGE(mon);
      SHOW_MESSAGE((String) "position Y: " + Y_state.currentPos);
#else
      printf("endstops: X-=%s X+=%s", X_state.IsEndMinus() ? "ON" : "OFF", X_state.IsEndPlus() ? "ON" : "OFF");
      printf("position %ld", X_state.currentPos);
      printf("endstops: Y-=%s Y+=%s", Y_state.IsEndMinus() ? "ON" : "OFF", Y_state.IsEndPlus() ? "ON" : "OFF");
      printf("position %ld", Y_state.currentPos);
      send("ok\n");
#endif
      break;
    }
    default:
      break;
    }
  }
}