#ifndef rotozoom_h_included
#define rotozoom_h_included
// --------------------------------------------------------------------------

extern int *yTab;

void InitRotoZoomTables(void);
void DrawRotoZoom(char *screen,char *image, int angle, int scale);
void preparezoom(int x1, int y1, int x2, int y2, unsigned short int *map);
void drawzoom(unsigned short int *zoommap, char *bitmap,char *buffer);
void Blur(char *dest, char *source);
void Mix(char *dest, char *source);

// --------------------------------------------------------------------------
#endif
