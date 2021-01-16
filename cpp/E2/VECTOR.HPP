 #ifndef vector_hpp_included
 #define vector_hpp_included
 // -------------------------------------------------------------------------

 #include <math.h>

class c_VECTOR
 {
 public:

  c_VECTOR() {};
  c_VECTOR(float xx,float yy,float zz) { x=xx; y=yy; z=zz; }
  ~c_VECTOR() {};

  // operator overloading

  inline c_VECTOR operator=(c_VECTOR v) { x=v.x; y=v.y; z=v.z; return *this; }

  inline c_VECTOR operator+(c_VECTOR v) { return c_VECTOR(x+v.x, y+v.y, z+v.z); }
  inline c_VECTOR operator-(c_VECTOR v) { return c_VECTOR(x-v.x, y-v.y, z-v.z); }
  inline c_VECTOR operator*(c_VECTOR v) { return c_VECTOR(x*v.x, y*v.y, z*v.z); }
  inline c_VECTOR operator/(c_VECTOR v) { return c_VECTOR(x/v.x, y/v.y, z/v.z); }

  inline c_VECTOR operator-() { return c_VECTOR(-x, -y, -z); }

  inline c_VECTOR operator+=(c_VECTOR v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
  inline c_VECTOR operator-=(c_VECTOR v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
  inline c_VECTOR operator*=(c_VECTOR v) { x*=v.x; y*=v.y; z*=v.z; return *this; }
  inline c_VECTOR operator/=(c_VECTOR v) { x/=v.x; y/=v.y; z/=v.z; return *this; }

  inline int operator==(c_VECTOR v) { if (x==v.x && y==v.y && z==v.z) return 1; return 0; }

  void Normalize()
  {
   float l = x*x + y*y + z*z;
   if (l!=0)
   {
	float ll = 1/sqrt(l);
	x*=ll;
	y*=ll;
	z*=ll;
   }
  }

  float Dot(c_VECTOR v) { return (x*v.x + y*v.y + z*v.z); }

  c_VECTOR Cross(c_VECTOR v) { return c_VECTOR(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }

 private:

  float x,y,z;
 };

 // -------------------------------------------------------------------------
 #endif
