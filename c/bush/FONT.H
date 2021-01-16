#ifndef font_h_included
#define font_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void Font_Setup(void);
int  Font_CharWidth(unsigned char c);
int  Font_StringWidth( char *s);
void Font_DrawChar(char *buffer,int x1, int y1, unsigned char c);
void Font_DrawString(char *buffer,int x1, int y1, char *s);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
