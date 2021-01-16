#include "node.hpp"

void c_NODE::CreateMatrix(void)
 {
  m_o2w.RotationXYZ( m_Rot );
  m_o2w.TranslationOnly( m_Pos );
  m_o2c = m_o2w /* * camera matrix */;
 }
