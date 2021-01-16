 #include "engine.h"

// --------------------------------------------------------------------------

void print_camera (sCamera *C)
 {
  printf("  Cam_Flags   %i\n",C->Flags);
  printf("  Cam_Id      %i\n",C->Id);
  printf("  Cam_Name    %s\n",C->Name);
  printf("  Cam_Pos     %f,%f,%f\n",C->Pos.x,C->Pos.y,C->Pos.z);
  printf("  Cam_Target  %f,%f,%f\n",C->Target.x,C->Target.y,C->Target.z);
  printf("  Cam_Fov     %f\n",C->Fov);
  printf("  Cam_Roll    %f\n",C->Roll);
  printf("  Cam_Parent1 %i\n",C->Parent1);
  printf("  Cam_Parent2 %i\n",C->Parent2);
 }

void print_light (sLight *L)
 {
  printf("  Light_Flags   %i\n",L->Flags);
  printf("  Light_Id      %i\n",L->Id);
  printf("  Light_Name    %s\n",L->Name);
  printf("  Light_Pos     %f,%f,%f\n",L->Pos.x,L->Pos.y,L->Pos.z);
  printf("  Light_Color   %f,%f,%f\n",L->Color.r,L->Color.g,L->Color.b);
  printf("  Light_Roll    %f\n",L->Roll);
  printf("  Light_Target  %f,%f,%f\n",L->Target.x,L->Target.y,L->Target.z);
  printf("  Light_Hotspot %f\n",L->Hotspot);
  printf("  Light_Falloff %f\n",L->Falloff);
  printf("  Light_Parent1 %i\n",L->Parent1);
  printf("  Light_Parent2 %i\n",L->Parent2);
 }

void print_object (sObject *O)
 {
  int i;

  printf("  Obj_Flags  %i\n",O->Flags);
  printf("  Obj_Id     %i\n",O->Id);
  printf("  Obj_Name   %s\n",O->Name);
  printf("  Obj_NumVertices %i\n",O->NumVertices);
  printf("  # ÄÄÄ\n");
  for (i=0;i<O->NumVertices;i++) printf("    %f,%f,%f\n",O->Vertices[i].Pos.x,O->Vertices[i].Pos.y,O->Vertices[i].Pos.z);
  printf("  # ÄÄÄ\n");
  printf("  Obj_NumPolygons %i\n",O->NumPolygons);
  printf("  Obj_Pivot %f,%f,%f\n",O->Pivot.x,O->Pivot.y,O->Pivot.z);
  printf("  Obj_Parent %i\n",O->Parent);
 }

void print_material (sMaterial *C)
 {
 }

void print_ambient (sAmbient *C)
 {
 }


// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void main(int argc, char *argv[])
 {
  kfScene  *scene;
  int width,height;
  unsigned char Pal[768];
  int err;
  sNode   *node;
  kfNode  *kfnode;
  sCamera *cam;

  // ---

  if (argc != 2)
  {
   printf("INFO <3ds-file>\n");
   exit(1);
  }

  // ---

  printf("þ Initializing engine\n");
  Engine_Init (FlagCalcnorm /*| FlagTransform*/ | FlagHierarchy);
  scene = Engine_AllocScene(320,200,8,20000);

  // ---

  printf("þ Loading default texture TEXTURE.PIX\n");
  Load_GIF("texture.gif",NULL,NULL,&width,&height);
  printf("Width: %i   Height: %i\n",width,height);
  scene->DefaultTexture = (char *)malloc(width*height);
  Load_GIF("texture.gif",(unsigned char *)scene->DefaultTexture,Pal,NULL,NULL);

  // ---

  printf("þ Loading 3DS file\n");
  err = Engine_LoadWorld(argv[1]/*"1.3ds"*/,scene);
  err = Engine_LoadMotion(argv[1]/*"1.3ds"*/,scene);
  Engine_SetActive(scene);

  // ---

  printf("\nWORLD\n\n");

  node = scene->World;
  while (node!=NULL)
  {
   switch(node->Type)
   {
    case NodeTypeCamera:

       print_camera( (sCamera *)node->Object );
       break;

    case NodeTypeLight:

       print_light( (sLight *)node->Object );
       break;

    case NodeTypeObject:

      print_object( (sObject *)node->Object );
      break;

    case NodeTypeMaterial:

      break;

    case NodeTypeAmbient:

      break;
   }
   node=node->Next;
  }

  printf("\nKEYFRAMER\n\n");

  kfnode = scene->Keyframer;
  while (kfnode!=NULL)
  {
   switch(kfnode->Type)
   {
    case KeyTypePos:

      printf("POS keyframer: Object: %i (%s)\n",kfnode->Id);


    //KeyTypeRotate
    //KeyTypeScale
    //KeyTypeFov
    //KeyTypeRoll
    //KeyTypeColor
    //KeyTypeMorph
    //KeyTypeHide

   }
  }
 }
