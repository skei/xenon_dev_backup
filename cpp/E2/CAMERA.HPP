 #ifndef camera_hpp_included
 #define camera_hpp_included
 // -------------------------------------------------------------------------

 #include "node.hpp"

// camera class.. inherits frm Node...
class c_CAMERA : public c_NODE
 {

 public:

  c_CAMERA();
  ~c_CAMERA();

 private:

 unsigned int Flags;

 };

 // -------------------------------------------------------------------------
 #endif


