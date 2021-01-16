 #ifndef node_hpp_included
 #define node_hpp_included
 // -------------------------------------------------------------------------

#include "vector.hpp"
#include "matrix.hpp"

class c_NODE
{

c_NODE();
~c_NODE();

void CreateMatrix(void);

// ---
public:
// ---

// ---
private:
// ---

c_VECTOR m_Pos;   // object position in 3d space
c_VECTOR m_Rot;   // object rotation around local origo

c_MATRIX m_o2w;   // object -> world matrix
c_MATRIX m_o2c;   // object -> camera matrix

};

 // -------------------------------------------------------------------------
 #endif

