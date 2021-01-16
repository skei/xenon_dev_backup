#ifndef axon_h_included
#define axon_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// square root approximation
// with Newton Raphson with iteration: a(n+1)=a(n)-(a(n)^2-b)/2a(n)
int Sqrt(unsigned int a);
unsigned int AsmAbs(int x);
void SetMode(unsigned short int vmode);
void draw_pixel(int x,int y,char color,char *sptr);
void draw_pixel_clip(int x,int y,char color,char *sptr);
void set_rgb(char col,char red,char green,char blue);
void set_palette(char *pal);
// Vaits for Vertical retrace to START
void waitvr (void);
void wait_key(void);
void load_pcx(char *filename, char *sptr, char *pal);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
