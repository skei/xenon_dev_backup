#ifndef spline_h_included
#define spline_h_included
// ��������������������������������������������������������������������������

#include "types.h"

// location = pos in spline (0..65535)
sCoord get_spline_point(int location, sSpline *spl);

// ��������������������������������������������������������������������������
#endif