#include <stdlib.h>
#include "engine.h"

#define MaxPolys 10000

sScene    *S;
sObject   *O;
sCamera   *C;
sLight    *L;
sMaterial *M;
sKey      *K;

void main(int argc, char **argv)
 {
  int i;

  if (argc != 2)
  {
   printf("%s <3DS-file>\n",argv[0]);
   exit(1);
  }
  S = Scene_New(MaxPolys);
  printf("# %s\n\n",argv[1]);
  Load3DS(argv[1],S);

  printf("#\n# ÄÄÄ SCENE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n#\n");
  printf("Scene_NumObjects     %i\n",S->NumObjects);
  printf("Scene_NumLights      %i\n",S->NumLights);
  printf("Scene_NumMaterials   %i\n",S->NumMaterials);
  printf("Scene_NumCameras     %i\n",S->NumCameras);
  printf("Scene_AmbientLight   %f\n",S->AmbientLight);
  printf("Scene_AnimStartFrame %i\n",S->StartFrame);
  printf("Scene_AnimEndFrame   %i\n",S->EndFrame);

  printf("#\n# ÄÄÄ MATERIALS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n#\n");
  M = S->Materials;
  while (M != NULL)
  {
   if (M != S->Materials) printf("# ÄÄÄ\n");
   printf("Mat_Name               %s\n",M->Name);
   printf("Mat_Flags              %i\n",M->Flags);
   printf("Mat_Type               %i\n",M->Type);
   printf("Mat_FilenameTexture    %s\n",M->FileNameTexture);
   printf("Mat_FilenameOpacity    %s\n",M->FileNameOpacity);
   printf("Mat_FilenameReflection %s\n",M->FileNameReflection);
   printf("Mat_FilenameBump       %s\n",M->FileNameBump);
   printf("Mat_Color              %i\n",M->Color);
   M = M->Next;
  }

  printf("#\n# ÄÄÄ OBJECTS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n#\n");
  O = S->Objects;
  while (O != NULL)
  {
   if (O != S->Objects) printf("# ÄÄÄ\n");
   printf("Obj_Name           %s\n",O->Name);
   printf("Obj_Id             %i\n",O->Id);
   printf("Obj_Flags          %i\n",O->Flags);
   printf("Obj_Type           %i\n",O->Type);
   printf("Obj_Pos            %f %f %f\n",O->Pos.x,O->Pos.y,O->Pos.z);
   printf("Obj_Rot            %f %f %f\n",O->Rot.x,O->Rot.y,O->Rot.z);
   printf("Obj_Scale          %f %f %f\n",O->Scale.x,O->Scale.y,O->Scale.z);
   printf("Obj_Matrix3DS      %f %f %f %f\n",O->Matrix3DS[ 0],O->Matrix3DS[ 1],O->Matrix3DS[ 2],O->Matrix3DS[ 3]);
   printf("                   %f %f %f %f\n",O->Matrix3DS[ 4],O->Matrix3DS[ 5],O->Matrix3DS[ 6],O->Matrix3DS[ 7]);
   printf("                   %f %f %f %f\n",O->Matrix3DS[ 8],O->Matrix3DS[ 9],O->Matrix3DS[10],O->Matrix3DS[11]);
   printf("                   %f %f %f %f\n",O->Matrix3DS[12],O->Matrix3DS[13],O->Matrix3DS[14],O->Matrix3DS[15]);
//  printf("Obj_Sphere         %f\n",O->SphereRadius);
   printf("Obj_NumVertices    %i\n",O->NumVertices);
   printf("Obj_NumPolygons    %i\n",O->NumPolygons);
   printf("Obj_NumNormals     %i\n",O->NumNormals);
   printf("Obj_ParentId       %i\n",O->ParentId);
   if (O->ParentId != -1) printf("Obj_Parent         %s\n",O->Parent->Name);
   else                   printf("Obj_Parent         NONE\n");
   printf("Obj_Pivot          %f %f %f\n",O->Pivot.x,O->Pivot.y,O->Pivot.z);
   printf("Obj_kfPosNumKeys   %i\n",O->kfPos.NumKeys);
   // printf("Obj_kfPosFrames    %i\n",O->kfPos.Frames);
   printf("Obj_kfRotNumKeys   %i\n",O->kfRot.NumKeys);
   // printf("Obj_kfRotFrames    %i\n",O->kfRot.Frames);
   printf("Obj_kfScaleNumKeys %i\n",O->kfScale.NumKeys);
   // printf("Obj_kfScaleFrames    %i\n",O->kfScale.Frames);

   printf("# ÄÄÄ VERTICES ÄÄÄ\n");
   for (i=0;i<O->NumVertices;i++)
   {
    printf("Obj_Vertex %i   %f %f %f\n",i,O->Vertices[i].Pos.x,O->Vertices[i].Pos.y,O->Vertices[i].Pos.z);
   }

   printf("# ÄÄÄ POLYGONS ÄÄÄ\n");
   for (i=0;i<O->NumPolygons;i++)
   {
    printf("Obj_Polygon %i   %i %i %i %i %s\n",i,O->Polygons[i].Flags,O->Polygons[i].A,O->Polygons[i].B,O->Polygons[i].C,O->Polygons[i].Material->Name);
   }

/*

   printf("# ÄÄÄ NORMALS ÄÄÄ\n");
   for (i=0;i<O->NumNormals;i++)
   {
    printf("Obj_NormalUV %i   %f %f\n",i,O->Normals[i].u,O->Normals[i].v);
   }

*/

   printf("# ÄÄÄ POS KEYFRAMES ÄÄÄ\n");
   for (i=0;i<O->kfPos.NumKeys;i++)
   {
    K = &(O->kfPos.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Obj_kfPosKey    %i\n",i);
    printf("Obj_kfPosFrame  %f\n",K->Frame);
    printf("Obj_kfPosSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Obj_kfPosData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
   }

   printf("# ÄÄÄ ROT KEYFRAMES ÄÄÄ\n");
   for (i=0;i<O->kfRot.NumKeys;i++)
   {
    K = &(O->kfRot.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Obj_kfRotKey    %i\n",i);
    printf("Obj_kfRotFrame  %f\n",K->Frame);
    printf("Obj_kfRotSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Obj_kfRotData   %f %f %f %f\n",K->Val._quat[qX],K->Val._quat[qY],K->Val._quat[qZ],K->Val._quat[qW]);
   }

   printf("# ÄÄÄ SCALE KEYFRAMES ÄÄÄ\n");
   for (i=0;i<O->kfScale.NumKeys;i++)
   {
    K = &(O->kfScale.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Obj_kfScaleKey    %i\n",i);
    printf("Obj_kfScaleFrame  %f\n",K->Frame);
    printf("Obj_kfScaleSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Obj_kfScaleData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
   }
   O=O->Next;
  }

  printf("#\n# ÄÄÄ CAMERAS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n#\n");
  C = S->Cameras;
  while (C != NULL)
  {
   if (C!=S->Cameras) printf("# ÄÄÄ\n");
   printf("Cam_Name            %s\n",C->Name);
   printf("Cam_Id              %i\n",C->Id);
   printf("Cam_Flags           %i\n",C->Flags);
   printf("Cam_Type            %i\n",C->Type);
   printf("Cam_Pos             %f %f %f\n",C->Pos.x,C->Pos.y,C->Pos.z);
   printf("Cam_Rot             %f %f %f\n",C->Rot.x,C->Rot.y,C->Rot.z);
   printf("Cam_Target          %f %f %f\n",C->Target.x,C->Target.y,C->Target.z);
   printf("Cam_UpVector        %f %f %f\n",C->UpVector.x,C->UpVector.y,C->UpVector.z);
   printf("Cam_Roll            %f\n",C->Roll);
   printf("Cam_Fov             %f\n",C->Fov);
   printf("Cam_ParentId        %i\n",C->ParentId);
   if (C->ParentId != -1) printf("Cam_Parent          %s\n",C->Parent->Name);
   else                   printf("Cam_Parent          NONE\n");
   printf("Cam_Pivot           %f %f %f\n",C->Pivot.x,C->Pivot.y,C->Pivot.z);
   printf("Cam_kfPosNumKeys    %i\n",C->kfPos.NumKeys);
   // printf("Cam_kfPosFrames    %i\n",C->kfPos.Frames);
   printf("Cam_kfTargetNumKeys %i\n",C->kfTarget.NumKeys);
   // printf("Cam_kfTargetFrames    %i\n",C->kfTarget.Frames);
   printf("Cam_kfRollNumKeys   %i\n",C->kfRoll.NumKeys);
   // printf("Cam_kfRollFrames    %i\n",O->kfRoll.Frames);
   printf("Cam_kfFovNumKeys    %i\n",C->kfFov.NumKeys);
   // printf("Cam_kfFovFrames    %i\n",O->kfFov.Frames);

   printf("# ÄÄÄ POS KEYFRAMES ÄÄÄ\n");
   for (i=0;i<C->kfPos.NumKeys;i++)
   {
    K = &(C->kfPos.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Cam_kfPosKey    %i\n",i);
    printf("Cam_kfPosFrame  %f\n",K->Frame);
    printf("Cam_kfPosSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Cam_kfPosData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
   }

   printf("# ÄÄÄ TARGET KEYFRAMES ÄÄÄ\n");
   for (i=0;i<C->kfTarget.NumKeys;i++)
   {
    K = &(C->kfTarget.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Cam_kfTargetKey    %i\n",i);
    printf("Cam_kfTargetFrame  %f\n",K->Frame);
    printf("Cam_kfTargetSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Cam_kfTargetData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
   }

   printf("# ÄÄÄ ROLL KEYFRAMES ÄÄÄ\n");
   for (i=0;i<C->kfRoll.NumKeys;i++)
   {
    K = &(C->kfRoll.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Cam_kfRollKey    %i\n",i);
    printf("Cam_kfRollFrame  %f\n",K->Frame);
    printf("Cam_kfRollSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Cam_kfRollData   %f\n",K->Val._float);
   }

   printf("# ÄÄÄ FOV KEYFRAMES ÄÄÄ\n");
   for (i=0;i<C->kfFov.NumKeys;i++)
   {
    K = &(C->kfFov.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Cam_kfFovKey    %i\n",i);
    printf("Cam_kfFovFrame  %f\n",K->Frame);
    printf("Cam_kfFovSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Cam_kfFovData   %f\n",K->Val._float);
   }
   C = C->Next;

  printf("#\n# ÄÄÄ LIGHTS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n#\n");
  L = S->Lights;
  while (L != NULL)
  {
   if (L!=S->Lights) printf("# ÄÄÄ\n");
   printf("Lig_Name            %s\n",L->Name);
   printf("Lig_Id              %i\n",L->Id);
   printf("Lig_Flags           %i\n",L->Flags);
   printf("Lig_Type            %i\n",L->Type);
   printf("Lig_Pos             %f %f %f\n",L->Pos.x,L->Pos.y,L->Pos.z);
   printf("Lig_Rot             %f %f %f\n",L->Rot.x,L->Rot.y,L->Rot.z);
   printf("Lig_Target          %f %f %f\n",L->Target.x,L->Target.y,L->Target.z);
   printf("Lig_Strength        %f\n",L->Strength);
   printf("Lig_MaxRange        %f\n",L->MaxRange);
   printf("Lig_HotSpoth        %f\n",L->HotSpot);
   printf("Lig_FallOff         %f\n",L->FallOff);
   printf("Lig_SpotRoll        %f\n",L->SpotRoll);
   printf("Lig_ParentId        %i\n",L->ParentId);
   if (L->ParentId != -1) printf("Lig_Parent          %s\n",L->Parent->Name);
   else                   printf("Lig_Parent          NONE\n");
   printf("Lig_kfPosNumKeys    %i\n",L->kfPos.NumKeys);
   // printf("Lig_kfPosFrames    %i\n",L->kfPos.Frames);
   printf("Lig_kfTargetNumKeys %i\n",L->kfTarget.NumKeys);
   // printf("Lig_kfTargetFrames    %i\n",L->kfTarget.Frames);

   printf("# ÄÄÄ POS KEYFRAMES ÄÄÄ\n");
   for (i=0;i<L->kfPos.NumKeys;i++)
   {
    K = &(L->kfPos.Keys[i]);
    if (i!=0) printf("# ÄÄÄ\n");
    printf("Lig_kfPosKey    %i\n",i);
    printf("Lig_kfPosFrame  %f\n",K->Frame);
    printf("Lig_kfPosSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
    printf("Lig_kfPosData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
   }

   if (L->kfTarget.NumKeys != 0)
   {
    printf("# ÄÄÄ TARGET KEYFRAMES ÄÄÄ\n");
    for (i=0;i<L->kfTarget.NumKeys;i++)
    {
     K = &(L->kfTarget.Keys[i]);
     if (i!=0) printf("# ÄÄÄ\n");
     printf("Lig_kfTargetKey    %i\n",i);
     printf("Lig_kfTargetFrame  %f\n",K->Frame);
     printf("Lig_kfTargetSpline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("Lig_kfTargetData   %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
    }
   }

   L = L->Next;
  }

  }
 }
