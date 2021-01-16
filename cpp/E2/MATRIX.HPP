 #ifndef matrix_hpp_included
 #define matrix_hpp_included
 // -------------------------------------------------------------------------

 #include <math.h>

class c_MATRIX
 {

// ---
public:
// ---

c_MATRIX() {};
~c_MATRIX() {};

// operator overloading

inline c_MATRIX operator=(c_MATRIX m)
 {
  M[ 0]=m.M[ 0]; M[ 1]=m.M[ 1]; M[ 2]=m.M[ 2]; M[ 3]=m.M[ 3];
  M[ 4]=m.M[ 4]; M[ 5]=m.M[ 5]; M[ 6]=m.M[ 6]; M[ 7]=m.M[ 7];
  M[ 8]=m.M[ 8]; M[ 9]=m.M[ 9]; M[10]=m.M[10]; M[11]=m.M[11];
  M[12]=m.M[12]; M[13]=m.M[13]; M[14]=m.M[14]; M[15]=m.M[15];
  return *this;
 }

inline void operator*=(c_MATRIX m)
 {
  c_MATRIX ab;
  int i,j,k;

  for (i=0;i<4;i++)
  {
   for (j=0;j<4;j++)
   {
    M[j*4+i] = 0.0;
    for (k=0;k<4;k++)
    {
     M[j*4+i] += M[k*4+i] * m.M[j*4+k];
    }
   }
  }
 }

 // --- functions ---

inline void Identity(void)
 {
  M[ 0]=1; M[ 1]=0; M[ 2]=0; M[ 3]=0;
  M[ 4]=0; M[ 5]=1; M[ 6]=0; M[ 7]=0;
  M[ 8]=0; M[ 9]=0; M[10]=1; M[11]=0;
  M[12]=0; M[13]=0; M[14]=0; M[15]=1;
 }

 // ---

 //   1   0   0   tx
 //   0   1   0   ty
 //   0   0   1   tz
 //   0   0   0   1
inline void Translation(float tx, float ty, float tz)
 {
  M[ 0] = 1; M[ 1] = 0; M[ 2] = 0; M[ 3] = tx;
  M[ 4] = 0; M[ 5] = 1; M[ 6] = 0; M[ 7] = ty;
  M[ 8] = 0; M[ 9] = 0; M[10] = 1; M[11] = tz;
  M[12] = 0; M[13] = 0; M[14] = 0; M[15] = 1;
 }

 // -----

void RotationXYZ(float xang, float yang, float zang)
 {
  float s1 = sin(xang);
  float s2 = sin(yang);
  float s3 = sin(zang);
  float c1 = cos(xang);
  float c2 = cos(yang);
  float c3 = cos(zang);
  // Ú                                                 ¿
  // ³      c2*c3             c2*s3          s2      tx³
  // ³ -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    ty³
  // ³ -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    tz³
  // ³        0                 0            0        1³
  // À                                                 Ù
  M[ 0] = c2*c3;
  M[ 1] = c2*s3;
  M[ 2] = s2;
  M[ 3] = 0;
  M[ 4] = -s1*s2*c3 - c1*s3;
  M[ 5] = -s1*s2*s3 + c1*c3;
  M[ 6] = s1*c2;
  M[ 7] = 0;
  M[ 8] = -c1*s2*c3 + s1*s3;
  M[ 9] = -c1*s2*s3 - s1*c3;
  M[10] = c1*c2;
  M[11] = 0;
  M[12] = 0;
  M[13] = 0;
  M[14] = 0;
  M[15] = 1;
 }

  // ---

  // c[i][j] = ä 1ókóq ( a[i][k] * b[k][j] )
  // c[i][j] = sum of a[i][k] * b[k][j] for k varying from 1 to q

  // ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
  // DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
  // GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

// ---
private:
// ---
float M[16];

 };

 // -------------------------------------------------------------------------
 #endif
