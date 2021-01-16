 #ifndef event_h_included
 #define event_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 // terminated will Time = -1
typedef struct sEventList_t
 {
  unsigned int Time;
  void (*Init)(void);
  void (*Start)(void);
  void (*Run)(int timer);
  void (*Stop)(void);
  void (*Exit)(void);
  } sEventList_t;

// ---

extern int CurrentEventPos;
extern sEventList_t EventList[];

// ---

void InitEventList(void);
void ExitEventList(void);
void UpdateEventList(int pos);




 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 /*

 "BruksAnvisning"

 En eller annen plass i sourcen, sikkert i intro.c, inkulderer man "event.h",
 og definerer en "event"-liste, noe al  dette:

 sEventList_t Intro[] =
 {
  {0,Intro_Init,Intro_Start,Intro_Exit,Intro_Stop,Intro_Exit},
  {etc...

  hvor 0 er modul pos, eller hva som helst, ihvertfall et tall som ›ker
  gjennom introen....
  og de fem neste er pointerre til rutiner...
  *_Init  - kj›res f›r introen starter (memory allokering, etc, prekalk)
  *_Exit  - etter at alt er ferdig (frigj›r minne, etc)
  *_Start - n†r effekten starter
  *_Stop  - n†r effekten stopper (overgangen til ny effekt)
  *_Run   - hver frame

 */
