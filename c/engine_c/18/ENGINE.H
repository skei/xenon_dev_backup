// engine.h

#ifndef engine_h_included
#define engine_h_included

#include "types.h"				 // mainly structures

struct objstruc* Object_init(void);
struct objstruc* Object_copy(struct objstruc *o);


void Object_set_rot(struct objstruc *o, int xa, int ya, int za);
void Object_set_pos(struct objstruc *o, int x, int y, int z);
void Object_add_rot(struct objstruc *o, int xa, int ya, int za);
void Object_add_pos(struct objstruc *o, int x, int y, int z);
void Object_set_mapping(struct objstruc *o, int t);

void Camera_set_rot(struct camstruc *c, int xa, int ya, int za);
void Camera_set_pos(struct camstruc *c, int x, int y, int z);
void Camera_add_rot(struct camstruc *c, int xa, int ya, int za);
void Camera_add_pos(struct camstruc *c, int x, int y, int z);
void Camera_set_target(struct camstruc *c, int x, int y, int z);
void camera_create_lookat(struct camstruc *c, float *m);
void create_camera_matrix(struct camstruc *c, float *m);

#endif