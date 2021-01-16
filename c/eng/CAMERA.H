#ifndef camera_h_included
#define camera_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

void Camera_Set_Rot(sCamera *c, int xa, int ya, int za);
void Camera_Set_Pos(sCamera *c, int x, int y, int z);
void Camera_Add_Rot(sCamera *c, int xa, int ya, int za);
void Camera_Add_Pos(sCamera *c, int x, int y, int z);
void Camera_Set_Target(sCamera *c, int x, int y, int z);
// Create camera look-at matrix in m...
void Camera_Create_Lookat(sCamera *c, int *m);







// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
