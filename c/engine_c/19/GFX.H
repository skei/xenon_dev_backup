 #ifndef gfx_h_included
 #define gfx_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

int Load_GIF(const char* file, unsigned char *outpix, unsigned char * pal,
                               int* width, int* height);
int Load_PCX(const char* file, unsigned char *outpix, unsigned char * pal,
                               int* width, int* height);
int Load_LBM(const char* file, unsigned char *outpix, unsigned char * pal,
                               int* width, int* height);
int Load_TGA(const char* file, unsigned char *outpix, unsigned char * pal,
                               int* width, int* height);
int Load_GFX(const char* file, unsigned char *outpix, unsigned char * pal,
                               int* width, int* height);
// int w = -1, h = -1;
// char pal[768];
// byte *im;
// if (!Load_GIF(gifname, NULL, NULL, &w, &h)) printf("File not found\n");
// printf("Width = %d, Height = %d\n", w, h);
// im = malloc(w*h);
// if (!Load_GIF(gifname, im, pal, NULL, NULL)) printf("Reloading pic.\n");


 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
