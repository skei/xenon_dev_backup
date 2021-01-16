#include <stdlib.h>   // exit
#include "engine.h"

sCamera *C;
sLight *L;
sMaterial *M;
sObject *O;
sVertex *V;
sPolygon *P;
sScene *Scene;

void main(int argc, char *argv[])
{
 int num,i;

 if (argc!=2)
 {
  printf("\nINFO <filename.3ds>\n");
  exit(1);
 }
 printf("\nLoading %s\n",argv[1]);
 Scene = Scene_New(10000);
 Load3DS(argv[1],Scene);

 printf("\nAnimation\n");
 printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
 if (Scene->SCENE_anim == 1)
   printf("\nAnimation found. Start: %i  End: %i\n",Scene->StartFrame,Scene->EndFrame);
   else printf("No animation found in 3DS\n");

 printf("\nCamera\n");
 printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
 C = Scene->Cameras;
 printf("Number of cameras: %i\n",Scene->NumCameras);
 while (C!=NULL)
 {
  printf("\n");
  printf("Name   : %s\n",C->Name);
  printf("Pos    : %f, %f, %f\n",C->Pos.x,C->Pos.y,C->Pos.z);
  printf("Target : %f, %f, %f\n",C->Target.x,C->Target.y,C->Target.z);
  printf("Roll   : %f\n",C->Roll);
  printf("FOV    : %f\n",C->Fov);
  printf("Hierarchy : %i\n",C->Id);
  printf("Parent    : %i (HierarchyNumber of Parent)\n",C->ParentId);
  printf("Pivot     : %f, %f, %f\n",C->Pivot.x,C->Pivot.y,C->Pivot.z);
  if (C->KF_Pos.NumKeys != 0)
  {
   printf("Number of Pos Keyframes : %i\n",C->KF_Pos.NumKeys);
   for (i=0;i<C->KF_Pos.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f, %f, %f\n",C->KF_Pos.FrameNumber[i]
                                               ,C->KF_Pos.Key[i].x
                                               ,C->KF_Pos.Key[i].y
                                               ,C->KF_Pos.Key[i].z);
   }
  }
  if (C->KF_Target.NumKeys != 0)
  {
   printf("Number of Target Keyframes : %i\n",C->KF_Target.NumKeys);
   for (i=0;i<C->KF_Target.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f, %f, %f\n",C->KF_Target.FrameNumber[i]
                                               ,C->KF_Target.Key[i].x
                                               ,C->KF_Target.Key[i].y
                                               ,C->KF_Target.Key[i].z);
   }
  }
  if (C->KF_Fov.NumKeys != 0)
  {
   printf("Number of FOV Keyframes : %i\n",C->KF_Fov.NumKeys);
   for (i=0;i<C->KF_Fov.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f\n",C->KF_Fov.FrameNumber[i]
                                       ,C->KF_Fov.Key[i].w);
   }
  }
  if (C->KF_Bank.NumKeys != 0)
  {
   printf("Number of Bank Keyframes : %i\n",C->KF_Bank.NumKeys);
   for (i=0;i<C->KF_Bank.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f\n",C->KF_Bank.FrameNumber[i]
                                       ,C->KF_Bank.Key[i].w);
   }
  }
  C = C-> Next;
 }

 printf("\nLights\n");
 printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
 L = Scene->Lights;
 printf("Number of lights: %i\n",Scene->NumLights);
 while (L!=NULL)
 {
  printf("\n");
  printf("Name  : %s\n",L->Name);
  printf("Pos   : %f, %f, %f\n",L->Pos.x,L->Pos.y,L->Pos.z);
  L = L-> Next;
 }

 printf("\nMaterials\n");
 printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
 M = Scene->Materials;
 printf("Number of materials: %i\n",Scene->NumMaterials);
 while (M!=NULL)
 {
  printf("\n");
  printf("Name    : %s\n",M->Name);

  if (M->MATERIAL_texture==1) printf("Texture : %s\n",M->FileName);
   else printf("No texture\n");
  M = M->Next;
 }

 printf("\nObjects\n");
 printf("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
 O = Scene->Objects;
 printf("Number of objects: %i\n",Scene->NumObjects);
 while (O!=NULL)
 {
  printf("\n");
  printf("Name      : %s\n",O->Name);
  printf("Vertices  : %i\n",O->NumVertices);
/*
  printf("----------------------------------------\n");
  for (i=0;i<O->NumVertices;i++)
  {
   V = &O->Vertices[i];
   printf("%i  -  X:%5.2f  Y:%5.2f  Z:%5.2f",i,V->Pos.x,V->Pos.y,V->Pos.z);
   if (O->OBJECT_mapped==1) printf(" U: %5.2f  V: %5.2f\n",V->u,V->v);
   else printf("\n");
  }
  printf("----------------------------------------\n");
*/
  printf("Polygons  : %i\n",O->NumPolygons);
//  printf("----------------------------------------\n");
//  for (i=0;i<O->NumPolygons;i++)
//  {
//   P = &O->Polygons[i];
//   printf("Flags:%i  Material:%s\n",
//    i,P->Flags,P->Material->Name);
//  }
//  printf("----------------------------------------\n");
  printf("Hierarchy : %i\n",O->Id);
  printf("Parent    : %i (HierarchyNumber of Parent)\n",O->ParentId);
  if (O->ParentId != -1) printf("Parent    : %s\n",O->Parent->Name);
  printf("Pivot     : %f, %f, %f\n",O->Pivot.x,O->Pivot.y,O->Pivot.z);
  if (O->KF_Pos.NumKeys != 0)
  {
   printf("Number of Pos Keyframes : %i\n",O->KF_Pos.NumKeys);
   for (i=0;i<O->KF_Pos.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f, %f, %f\n",O->KF_Pos.FrameNumber[i]
                                               ,O->KF_Pos.Key[i].x
                                               ,O->KF_Pos.Key[i].y
                                               ,O->KF_Pos.Key[i].z);
   }
  }
  if (O->KF_Rot.NumKeys != 0)
  {
   printf("Number of Rot Keyframes : %i\n",O->KF_Rot.NumKeys);
   for (i=0;i<O->KF_Rot.NumKeys;i++)
   {
    printf("FrameNumber   : %i  : %f, %f, %f : %f\n",O->KF_Rot.FrameNumber[i]
                                                    ,O->KF_Rot.Key[i].x
                                                    ,O->KF_Rot.Key[i].y
                                                    ,O->KF_Rot.Key[i].z
                                                    ,O->KF_Rot.Key[i].w);
   }
  }
  O = O->Next;
 }
}
