#ifndef music_h_included
#define music_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

int Music_Init(void);
void Music_DeInit(void);
void Music_Start(void);
void Music_Stop(void);
// v = volume (0..64)
void Music_SetVolume(unsigned char v);
// returns sync value (effect EFx or Sxx)
unsigned char Music_GetSync(void);
// returns current playing pos... (highbyte: order, lowbyte: row)
unsigned short int Music_GetPos(void);
// returns current playing order
unsigned char Music_GetOrder(void);
// returns current playing row
unsigned char Music_GetRow(void);
// returns number of ticks since module start. (1193046Hz)
int Music_GetTimer(void);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
