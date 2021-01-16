 #ifndef material_hpp_included
 #define material_hpp_included
 // -------------------------------------------------------------------------

class c_MATERIAL
{
// ---
public:
// ---

// ---
private:
// ---
unsigned int TEXTURE : 1;
unsigned int BUMP    : 1;
unsigned int ENVMAP  : 1;
int Flags;
int Id;
char *Name;
int Shading;          // shading, flags (flat,gouraud,phong,metal)
int MappingType;      // checked in Engine_Draw to call correct mapper... ??
c_RGB Ambient;        // ambient color
c_RGB Diffuse;        // diffuse color
c_RGB Specular;       // specular color
c_RGB Shininess;      // shininess
c_RGB ShinStrength;   // snininess strength
c_RGB Transparency;   // transparency
c_RGB TransFalloff;   // transparency falloff
c_RGB RefBlur;        // reflection blur
c_RGB SelfIllum;      // self illuminance
c_BITMAP Texture;     // texture map
c_BITMAP Bump;        // bump map
c_BITMAP Reflection;  // reflection map
};

 // -------------------------------------------------------------------------
 #endif
