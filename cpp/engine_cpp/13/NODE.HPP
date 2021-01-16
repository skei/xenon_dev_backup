 #ifndef node_hpp_included
 #define node_hpp_included
 // -------------------------------------------------------------------------

 #include "vector.hpp"
 #include "matrix.hpp"

class c_NODE
 {
 public:

  c_NODE();
  ~c_NODE();

 private:

  unsigned int Flags;

  c_VECTOR Position;		// Position in 3d space
  c_VECTOR Rotation;		// Rotation around object's local axes
  c_VECTOR Scale;       // Scale
  // c_QUAT;            // Rotation quaternion
  c_MATRIX Matrix;			// Final rotation matrix

  c_NODE &Parent;

 };

 // -------------------------------------------------------------------------
 #endif
