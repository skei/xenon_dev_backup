#ifndef voxel_h_included
#define voxel_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extern int lastzobs;
extern unsigned char VoxelPic[65536];
extern unsigned char VoxelPal[768];

void MakeMulTable(void);
void createfractalmap(char *heightmap, int x1, int y1, int x2, int y2);
void smoothmap( char *heightmap);
void DrawVoxel (int xp, int yp, float dir, char *heightmap, char *colormap, char *buffer);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
