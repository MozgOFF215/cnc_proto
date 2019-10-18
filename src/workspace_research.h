#ifndef _WORKSPACE_RESEARCH_H_
#define _WORKSPACE_RESEARCH_H_

enum search_steps
{
  NO_PROCESS,
  LEFT_ENDSTOP_SEARCH,
  SEEK_0,
  RIGHT_ENDSTOP_SEARCH,
  SEEK_MAX
};

void startResearch();
void foundEndStop();
void leaveEndStop();

#endif // _WORKSPACE_RESEARCH_H_