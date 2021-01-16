 #include "map.hpp"
 #include "scene.hpp"

// --- defines ---

#define MAX_POLYS_ON_SCREEN 5000

// --- MAIN ---

void main(void)
 {
  c_MAP Buffer(320,200,8);
  c_MAP Screen(320,200,8,(void *)0xA0000);

  c_SCENE Scene(MAX_POLYS_ON_SCREEN);

  Scene.Load3DS("test.3ds",LOAD_MESHES|LOAD_ANIM);

  Screen.SetMode();

  while (!kbhit())
  {
   Buffer.Clear();
   Scene.Render(Buffer);
   Buffer.BlitTo(Screen);
  }
 }
