#ifndef _WORKSPACE_RESEARCH_H_
#define _WORKSPACE_RESEARCH_H_

enum wsr_step
{
  NO_RESEARCH,
  LEFT_ENDSTOP_SEARCH,
  SEEK_0,
  RIGHT_ENDSTOP_SEARCH,
  SEEK_MAX
};

enum moveDirection{
  FORWARD,
  BACKWARD
};

void startResearch();
void foundEndStop();
void leaveEndStop();

#endif // _WORKSPACE_RESEARCH_H_