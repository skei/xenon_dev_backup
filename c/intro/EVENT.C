 // Event manager

 #include "event.h"
 #include "std.h"

 // ---

int CurrentEventPos;
//sEventList_t EventList[];

 // ---

void InitEventList(void)
 {
  int i;

  i = 0;
  while (EventList[i].Time != -1)
  {
   if (EventList[i].Init != NULL) EventList[i].Init();
   i++;
  }
  CurrentEventPos = 0;
 }

 // ---

void ExitEventList(void)
 {
  int i;

  i = 0;
  while (EventList[i].Time != -1)
  {
   if (EventList[i].Exit != NULL) EventList[i].Exit();
   i++;
  }
 }

 // ---

void UpdateEventList(int pos)
 {
  //pos = GetModulePos();
  if (pos >= EventList[CurrentEventPos+1].Time)
  {
   if (EventList[CurrentEventPos].Stop != NULL) EventList[CurrentEventPos].Stop();
   CurrentEventPos ++;
   if (EventList[CurrentEventPos].Start != NULL) EventList[CurrentEventPos].Start();
  }
  else /*if (EventList[CurrentEventPos].Run != NULL)*/ EventList[CurrentEventPos].Run(pos);
 }

