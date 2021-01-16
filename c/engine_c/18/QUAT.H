// quat.h

#ifndef quat_h_included
#define quat_h_included

void Quat_to_matrix(int *q, int *m);
void Quat_multiply(int *q, int *q1, int *q2);
void Quat_add(int *q, int *q1, int *q2);
void Quat_normalize(int *qq, int *q);


#endif
