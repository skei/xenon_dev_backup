#ifndef matrix_h_included
#define matrix_h_included

void init_identity(int *m);
void init_translation(int *m, int tx, int ty, int tz);
void init_rotation(int *m,int xang, int yang, int zang);
void mult_matrix(int *a, int *b);
void process_matrix(int *m, int *src, int *dst, int num);
void copy_matrix(int *m1, int *m2);

int fixedmul2(int, int);
int fixedmul3(int, int, int);


#endif
