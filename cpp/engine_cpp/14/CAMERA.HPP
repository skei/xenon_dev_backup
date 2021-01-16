 #ifndef camera_hpp_included
 #define camera_hpp_included
 // -------------------------------------------------------------------------

#include "node.hpp"
#include "matrix.hpp"

class c_CAMERA : public c_NODE
{

// ---
public:
// ---

c_CAMERA();
~c_CAMERA();

// void CreateMatrix_Free(void);

// ---
private:
// ---

c_MATRIX matrix;

};

 // -------------------------------------------------------------------------
 #endif


