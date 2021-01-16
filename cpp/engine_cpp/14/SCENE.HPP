 #ifndef scene_hpp_included
 #define scene_hpp_included
 // -------------------------------------------------------------------------

#include "map.hpp"
#include "node.hpp"
#include "camera.hpp"

class c_SCENE
{

// ---
public:
// ---

c_SCENE();
~c_SCENE();

inline void SetActiveCamera( c_CAMERA &c ) { m_ActiveCamera = c; }
inline void SetRenderBuffer( c_MAP &m ) { m_RenderBuffer = m; }

// ---
private:
// ---

c_MAP m_RenderBuffer;
c_NODE &m_Nodes;
c_CAMERA &m_ActiveCamera;     // ptr to the node containing active camera

};

 // -------------------------------------------------------------------------
 #endif

