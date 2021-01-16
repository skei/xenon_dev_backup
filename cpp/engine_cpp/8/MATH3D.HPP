 #ifndef math3d_hpp_included
 #define math3d_hpp_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <math.h>

 #define PI 3.1415926535

 //
 // --- proto ---
 //

 class vector;
 class matrix;

//
// ÄÄÄ vector ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

class vector
 {
  public:

// ---

float X,Y,Z;

// --- constructors ---

inline vector() {};

inline vector(const float x, const float y, const float z)
 { X=x; Y=y; Z=z; }

inline vector(const vector& v)
 { X=v.X; Y=v.Y; Z=v.Z; }

inline ~vector() {};

// ---

inline vector& operator  = (const vector& v)   // assignment of a vector
 { X=v.X; Y=v.Y; Z=v.Z; return *this; }

inline vector& operator += (const vector& v)   // incrementation by a vector
 { X+=v.X; Y+=v.Y; Z+=v.Z; return *this; }

inline vector& operator -= (const vector& v)   // decrementation by a vector
 { X-=v.X; Y-=v.Y; Z-=v.Z; return *this; }

inline vector& operator *= (const float f)     // multiplication by a constant
 { X*=f; Y*=f; Z*=f; return *this; }

inline vector& operator /= (const float f)     // division by a constant
 { float i=1/f; X*=f; Y*=f; Z*=f; return *this; }

// ---

inline friend vector operator -  (const vector& v)                    // -v1
 { return vector(-v.X, -v.Y, -v.Z); }

inline friend vector operator +  (const vector& a, const vector& b)   // v1 + v2
 { return vector(a.X+b.X, a.Y+b.Y, a.Z+b.Z); }

inline friend vector operator -  (const vector& a, const vector& b)   // v1 - v2
 { return vector(a.X-b.X, a.Y-b.Y, a.Z-b.Z); }

inline friend vector operator *  (const vector& a, const float   f)   // v1 * 3.0
 { return vector(a.X*f, a.Y*f, a.Z*f); }

inline friend vector operator *  (const float   f, const vector& a)   // 3.0 * v1
 { return a*f; }

 // inline friend vector operator *  (const matrix& a, const vector& v)   // M . v
 //  {
 //  }

 // inline friend vector operator *  (const vector& v, const matrix& a)   // v . M
 //  {}

inline friend float  operator *  (const vector& a, const vector& b)   // dot product
 { return (a.X*b.X + a.Y*b.Y + a.Z*b.Z); }

inline friend vector operator /  (const vector& a, const float   f)   // v1 / 3.0
 { float i=1/f; return vector(a.X*i, a.Y*i, a.Z*i); }

inline friend vector operator ^  (const vector& a, const vector& b)   // cross product
 { return vector(a.Y*b.Z-a.Z*b.Y, a.Z*b.X-a.X*b.Z, a.X*b.Y-a.Y*b.X); }

inline friend int    operator == (const vector& a, const vector& b)   // v1 == v2 ?
 { return (a.X==b.X) && (a.Y == b.Y) && (a.Z == b.Z); }

inline friend int    operator != (const vector& a, const vector& b)   // v1 != v2 ?
 { return !(a==b); }

// ---

inline float Length()
 {
  return (sqrt(X*X + Y*Y + Z*Z ));
 }


inline void Normalize()
 {
  static float len,invlen;

  len = Length();
  if (len != 0)
  {
   invlen = 1/len;
   X *= len;
   Y *= len;
   Z *= len;
  }
 // else
 // {
 //  X = 0;
 //  Y = 0;
 //  Z = 0;
 // }
 }


 };

//
// ÄÄÄ matrix ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

class matrix
 {
  public:

// ---

static float M[16];

// --- Constructors ---

inline matrix() {};

inline matrix(const matrix& m)
 {
  M[ 0]=m.M[ 0];  M[ 1]=m.M[ 1];  M[ 2]=m.M[ 2];  M[ 3]=m.M[ 3];
  M[ 4]=m.M[ 4];  M[ 5]=m.M[ 5];  M[ 6]=m.M[ 6];  M[ 7]=m.M[ 7];
  M[ 8]=m.M[ 8];  M[ 9]=m.M[ 9];  M[10]=m.M[10];  M[11]=m.M[11];
  M[12]=m.M[12];  M[13]=m.M[13];  M[14]=m.M[14];  M[15]=m.M[15];
 }

inline ~matrix() {};

// ---

inline matrix& operator  = (const matrix& m)  // assignment of a matrix
 {
  M[ 0]=m.M[ 0];  M[ 1]=m.M[ 1];  M[ 2]=m.M[ 2];  M[ 3]=m.M[ 3];
  M[ 4]=m.M[ 4];  M[ 5]=m.M[ 5];  M[ 6]=m.M[ 6];  M[ 7]=m.M[ 7];
  M[ 8]=m.M[ 8];  M[ 9]=m.M[ 9];  M[10]=m.M[10];  M[11]=m.M[11];
  M[12]=m.M[12];  M[13]=m.M[13];  M[14]=m.M[14];  M[15]=m.M[15];
  return *this;
 }

// ---

inline friend matrix operator *= (const matrix& a, const matrix& b) // m1 *= m2
 {
  matrix m;
  int i,j,k;

  for (i=0;i<4;i++)
  {
   for (j=0;j<4;j++)
   {
    m.M[j*4+i] = 0;
    for (k=0;k<4;k++)
    {
     m.M[j*4+i] += a.M[k*4+i] * b.M[j*4+k];
    }
   }
  }
  return m;
 }

 // Creates inverse of matrix... stable???
inline friend matrix operator -  (const matrix& m) // -m
 {
  matrix t1,t2;

  t1.Translation(-m.M[3],-m.M[7],-m.M[11]);
  t2.Identity();
  t2.M[ 0] = m.M[ 0];
  t2.M[ 1] = m.M[ 4];
  t2.M[ 2] = m.M[ 8];
  t2.M[ 4] = m.M[ 1];
  t2.M[ 5] = m.M[ 5];
  t2.M[ 6] = m.M[ 9];
  t2.M[ 8] = m.M[ 2];
  t2.M[ 9] = m.M[ 6];
  t2.M[10] = m.M[10];
  t1 *= t2;
  return t1;
 }

// ---

 //   1   0   0   0
 //   0   1   0   0
 //   0   0   1   0
 //   0   0   0   1
inline void Identity        (void)
 {
  M[ 0] = 1;  M[ 1] = 0;  M[ 2] = 0;  M[ 3] = 0;
  M[ 4] = 0;  M[ 5] = 1;  M[ 6] = 0;  M[ 7] = 0;
  M[ 8] = 0;  M[ 9] = 0;  M[10] = 1;  M[11] = 0;
  M[12] = 0;  M[13] = 0;  M[14] = 0;  M[15] = 1;
 }

 // ÄÄÄ

 //   1   0   0   tx
 //   0   1   0   ty
 //   0   0   1   tz
 //   0   0   0   1

inline void Translation     (const float tx, const float ty, const float tz)
 {
  M[ 0] = 1;  M[ 1] = 0;  M[ 2] = 0;  M[ 3] = tx;
  M[ 4] = 0;  M[ 5] = 1;  M[ 6] = 0;  M[ 7] = ty;
  M[ 8] = 0;  M[ 9] = 0;  M[10] = 1;  M[11] = tz;
  M[12] = 0;  M[13] = 0;  M[14] = 0;  M[15] = 1;
 }

inline void Translation     (const vector& v)
 {
  M[ 0] = 1;  M[ 1] = 0;  M[ 2] = 0;  M[ 3] = v.X;
  M[ 4] = 0;  M[ 5] = 1;  M[ 6] = 0;  M[ 7] = v.Y;
  M[ 8] = 0;  M[ 9] = 0;  M[10] = 1;  M[11] = v.Z;
  M[12] = 0;  M[13] = 0;  M[14] = 0;  M[15] = 1;
 }

inline void TranslationOnly (const float tx, const float ty, const float tz)
 {
  M[ 3] = tx;
  M[ 7] = ty;
  M[11] = tz;
 }

inline void TranslationOnly (const vector& v)
 {
  M[ 3] = v.X;
  M[ 7] = v.Y;
  M[11] = v.Z;
 }

 // ---

inline void RotationXYZ     (const float x, const float y, const float z)
 {
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(x);
  s2 = sin(y);
  s3 = sin(z);
  c1 = cos(x);
  c2 = cos(y);
  c3 = cos(z);
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

 // ÄÄÄ

inline void RotationXYZonly (const float x, const float y, const float z)
 {
  float s1,s2,s3,c1,c2,c3;
  s1 = sin(x);
  s2 = sin(y);
  s3 = sin(z);
  c1 = cos(x);
  c2 = cos(y);
  c3 = cos(z);
  M[ 0] = c2*c3;
  M[ 1] = c2*s3;
  M[ 2] = s2;
  M[ 4] = -s1*s2*c3 - c1*s3;
  M[ 5] = -s1*s2*s3 + c1*c3;
  M[ 6] = s1*c2;
  M[ 8] = -c1*s2*c3 + s1*s3;
  M[ 9] = -c1*s2*s3 - s1*c3;
  M[10] = c1*c2;
 }

 // ---

inline void RotationXYZ     (const vector& v)
 {
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(v.X);
  s2 = sin(v.Y);
  s3 = sin(v.Z);
  c1 = cos(v.X);
  c2 = cos(v.Y);
  c3 = cos(v.Z);
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

inline void RotationXYZonly (const vector& v)
 {
  float s1,s2,s3,c1,c2,c3;
  s1 = sin(v.X);
  s2 = sin(v.Y);
  s3 = sin(v.Z);
  c1 = cos(v.X);
  c2 = cos(v.Y);
  c3 = cos(v.Z);
  M[ 0] = c2*c3;
  M[ 1] = c2*s3;
  M[ 2] = s2;
  M[ 4] = -s1*s2*c3 - c1*s3;
  M[ 5] = -s1*s2*s3 + c1*c3;
  M[ 6] = s1*c2;
  M[ 8] = -c1*s2*c3 + s1*s3;
  M[ 9] = -c1*s2*s3 - s1*c3;
  M[10] = c1*c2;
 }

 // ---

/*

 // Creates inverse of matrix... returns 1 if success/posible, 0 otherwise
int  Matrix_Inverse2( float *matrix )
 {
  int i,j,k;

  float pvt_i[4];
  float pvt_j[4];

  float pvt_val;
  float hold;
  float determ;

  determ = 1.0;
  for(k=0; k<4; k++)
  {
   pvt_val = matrix[k+4*k];                       // ???
   pvt_i[k] = k;
   pvt_j[k] = k;
   for(i=k; i<4; i++)
   {
    for(j=0; j<4; j++)
    {
     if(fabs(matrix[i+4*j]) > fabs(pvt_val))      // ???
     {
      pvt_i[k] = i;
      pvt_j[k] = j;
      pvt_val = matrix[i+4*j];                    // ???
     }
    }
   }
   // Check for singularity
   determ *= pvt_val;
   if(determ == 0.0)
   {
    // Matrix is singular
    return (0);
   }
   // "Interchange" rows
   i = pvt_i[k];
   if(i!=k)
   {
    for(j=0; j<4; j++)
    {
     hold = -matrix[k+4*j];                                   // ???
     matrix[k+4*j] = matrix[i+4*j];                           // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   // "Interchange" columns
   j = pvt_j[k];
   if(j!=k)
   {
    for(i=0; i<4; i++)
    {
     hold = -matrix[i+4*k];                                   // ???
     matrix[i+4*k] = matrix[i+4*j];                           // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   // Divide column by minus pivot value
   for(i=0; i<4; i++)
   {
    if(i!=k) matrix[i+4*k] /= -pvt_val;                       // ???
   }
   // Reduce the matrix
   for(i=0; i<4; i++)
   {
    hold = matrix[i+4*k];                                     // ???
    for(j=0; j<4; j++)
    {
     if(i!=k && j!=k) matrix[i+4*j] += hold * matrix[k+4*j];  // ???
    }
   }
   // Divide row by pivot
   for(j=0; j<4; j++)
   {
    if(j!=k) matrix[k+4*j] /= pvt_val;                        // ???
   }
   // Replace pivot by reciprocal
   matrix[k+4*k] = 1.0/pvt_val;                               // ???
  }
  // Final pass of row/column interchange
  for(k=2; k>=0; k--)
  {
   i = pvt_j[k];
   if(i!=k)
   {
    for(j=0; j<4; j++)
    {
     hold = matrix[k*4+j];                                    // ???
     matrix[k+4*j] = -matrix[i+4*j];                          // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   j = pvt_i[k];
   if(j!=k)
   {
    for(i=0; i<4; i++)
    {
     hold = matrix[i+4*k];                                    // ???
     matrix[i+4*k] = -matrix[i+4*j];                          // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
  }
  return(1);
 }

*/


 };

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
