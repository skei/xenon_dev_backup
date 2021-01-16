 #ifndef spline2_h_included
 #define spline2_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #include "engine.h"

int Spline_Init ( sKeyframe *keys);
int Spline_GetFloat ( float   *out, sKeyframe *keys, float frame);
int Spline_GetVector( sVector *out, sKeyframe *keys, float frame);
int Spline_GetQuat  ( float   *out, sKeyframe *keys, float frame);

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
