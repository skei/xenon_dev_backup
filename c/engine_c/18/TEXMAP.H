// texmap.h

#ifndef texmap_h_included
#define texmap_h_included

#include "types.h"

void TexMap(struct screen_S *vp,int c1, int c2, int c3, int *tric, int *texc, unsigned char *Texture);
void GTexMap(struct screen_S *vp,int cc1, int cc2, int cc3, int *tric, int *texc, char *colorc, unsigned char *Texture);


#endif