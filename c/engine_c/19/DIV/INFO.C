#include <stdio.h>
#include "engine.h"

kfScene  *scene;
sNode *node;
int err;

// --------------------------------------------------------------------------

void ShowCameraInfo(sNode *node)
{
 sCamera *c;

 printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
 c = (sCamera *)node->Object;
 printf("Camera: %s\n",c->Name);
 printf("Flags: %x\n",c->Flags);
 printf("Id: %i\n",c->Id);
 printf("Pos: %f,%f,%f\n",c->Pos.x,c->Pos.y,c->Pos.z);
 printf("Target: %f,%f,%f\n",c->Target.x,c->Target.y,c->Target.z);
 printf("Fov: %f\n",c->Fov);
 printf("Roll: %f\n",c->Roll);
 printf("Parent1: %i   Parent2: %i\n",c->Parent1,c->Parent2);
}

void ShowLightInfo(sNode *node)
{
 sLight *l;

 printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
 l = (sLight *)node->Object;
 printf("Light: %s\n",l->Name);
 printf("Flags: %x : ",l->Flags);
 if (l->Flags == LightFlagOmni) printf("Omni\n");
 else if (l->Flags == LightFlagSpot) printf("Spot\n");
 printf("Id: %i\n",l->Id);
 printf("Pos: %f,%f,%f\n",l->Pos.x,l->Pos.y,l->Pos.z);
 printf("Color (rgb): %f,%f,%f\n",l->Color.r,l->Color.g,l->Color.b);
 if (l->Flags == LightFlagSpot)
 {
  printf("Target: %f,%f,%f\n",l->Target.x,l->Target.y,l->Target.z);
  printf("Roll: %f\n",l->Roll);
  printf("Hotspot: %f\n",l->Hotspot);
  printf("Falloff: %f\n",l->Falloff);
 }
 printf("Parent1: %i   Parent2: %i\n",l->Parent1,l->Parent2);
}

void ShowObjectInfo(sNode *node)
{
 sObject *o;

 printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
 o = (sObject *)node->Object;
 printf("Object: %s\n",o->Name);
 printf("Flags: %x : ",o->Flags);
 printf("Id: %i\n",o->Id);
 printf("# Vertices: %i\n",o->NumVertices);
 printf("# Polygons: %i\n",o->NumPolygons);
 printf("Pivot: %f,%f,%f\n",o->Pivot.x,o->Pivot.y,o->Pivot.z);
 printf("Parent: %i\n",o->Parent);

}

void ShowMaterialInfo(sNode *node)
{
 sMaterial *m;

 printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
 m = (sMaterial *)node->Object;
 printf("Material: %s\n",m->Name);
 printf("Flags: %x : ",m->Flags);
 printf("Id: %i\n",m->Id);
 printf("Shading: %i\n",m->Shading);
 /*
 printf("Ambient (rgb): %f,%f,%f\n",m->Ambient.r,m->Ambient.g,m->Ambient.b);
 printf("Diffuse (rgb): %f,%f,%f\n",m->Diffuse.r,m->Diffuse.g,m->Diffuse.b);
 printf("Specular (rgb): %f,%f,%f\n",m->Specular.r,m->Specular.g,m->Specular.b);
 printf("Shininess (rgb): %f,%f,%f\n",m->Shininess.r,m->Shininess.g,m->Shininess.b);
 printf("ShinStrength (rgb): %f,%f,%f\n",m->ShinStrength.r,m->ShinStrength.g,m->ShinStrength.b);
 printf("Transparency (rgb): %f,%f,%f\n",m->Transparency.r,m->Transparency.g,m->Transparency.b);
 printf("TransFalloff (rgb): %f,%f,%f\n",m->TransFalloff.r,m->TransFalloff.g,m->TransFalloff.b);
 printf("RefBlur (rgb): %f,%f,%f\n",m->RefBlur.r,m->RefBlur.g,m->RefBlur.b);
 printf("SelfIllum (rgb): %f,%f,%f\n",m->SelfIllum.r,m->SelfIllum.g,m->SelfIllum.b);
 */
 printf("Texture file: %s\n",m->Texture.File);
 printf("Bump file: %s\n",m->Bump.File);
 printf("Reflection file: %s\n",m->Reflection.File);
}

void ShowAmbientInfo(sNode *node)
{
 sAmbient *a;

 printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴\n");
 a = (sAmbient *)node->Object;
 printf("Material: %s\n",a->Name);
 printf("Id: %i\n",a->Id);
 printf("Color (rgb): %f,%f,%f\n",a->Color.r,a->Color.g,a->Color.b);
}

// --------------------------------------------------------------------------

void main(int argc, char *argv[])
{
 Engine_Init (FlagCalcnorm | FlagTransform | FlagHierarchy);
 scene = Engine_AllocScene();
 Engine_setActive(scene);

 // --- loading scene ---

 err = Engine_LoadWorld(argv[1]/*"1.3ds"*/,scene);
 if (err != OK) printf("Error loading Meshes\n");
 err = Engine_LoadMotion(argv[1]/*"1.3ds"*/,scene);
 if (err != OK) printf("Error loading Keyframer\n");

 // ---

 node = Engine_Scene->World;
 while (node!=NULL)
 {
  switch(node->Type)
  {
   case NodeTypeCamera : ShowCameraInfo(node);
                         break;
   case NodeTypeLight : ShowLightInfo(node);
                        break;
   case NodeTypeObject : ShowObjectInfo(node);
                         break;
   case NodeTypeMaterial : ShowMaterialInfo(node);
                           break;
   case NodeTypeAmbient : ShowAmbientInfo(node);
                          break;
  }
  node = node->Next;
 }


 Engine_FreeWorld(scene);
 Engine_FreeMotion(scene);
}
