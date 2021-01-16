#ifndef matrix_h_included
#define matrix_h_included

void init_identity(float *m);
void init_translation(float *m, int tx, int ty, int tz);
void init_rotation(float *m, int xang, int yang, int zang);
void mult_matrix(float *a, float *b);
void process_matrix(float *m, int *src, int *dst, int num);
void copy_matrix(float *m1, float *m2);

#endif
