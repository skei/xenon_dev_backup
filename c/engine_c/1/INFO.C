 #include <stdlib.h>
 #include "engine.h"

 #define MaxPolys 1

 sScene    *S;
 sObject   *O;
 sCamera   *C;
 sLight    *L;
 sMaterial *M;
 sKeyframe *K;
 sNode     *N;

 float minx,miny,minz;
 float maxx,maxy,maxz;
 int i,j;

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void ShowScene(void)
 {
  printf("  # Testing scene\n");

  // Check objects
  N = S->Nodes;
  while (N!=NULL)
  {
   if (N->Type == NODEtype_object)
   {
    O = (sObject *)N->Data;
    if (!(O->Flags & OBJflag_mapped))
    {
     printf("  # Object %s has no mapping coordinates\n",N->Name);
     j = 0;
     for (i=0;i<O->NumPolygons;i++) if (O->Polygons[i].Material == NULL) j++;
     if (j!=0) printf("  # The object %s has %i polygons not assigned to any material\n",N->Name,j);
    }
   }
   N=N->Next;
  }

  printf("# ÄÄÄ SCENE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
  printf("  Scene_StartFrame            %f\n",S->StartFrame);
  printf("  Scene_EndFrame              %f\n",S->EndFrame);
  printf("  Scene_NumNodes              %i\n",S->NumNodes);
  printf("  Scene_NumMaterials          %i\n",S->NumMaterials);
  printf("  Scene_MasterScale           %f\n",S->MasterScale);
  printf("  Scene_AmbientLight          %f %f %f\n",S->AmbientLightRGB.r,S->AmbientLightRGB.g,S->AmbientLightRGB.b);
  printf("  Scene_UseFog                ");
  if (S->UseFog == NO) printf("No\n");
  else
  {
   printf("Yes\n");
   printf("  Scene_NearFogPlaneDistance %f\n",S->NearFogPlaneDistance);
   printf("  Scene_NearFogPlaneDensity  %f\n",S->NearFogPlaneDensity);
   printf("  Scene_FarFogPlaneDistance  %f\n",S->FarFogPlaneDistance);
   printf("  Scene_FarFogPlaneDensity   %f\n",S->FarFogPlaneDensity);
   printf("  Scene_FogColor             %f %f %f\n",S->FogColor.r,S->FogColor.g,S->FogColor.b);
  }
 }

 // ---

void ShowNodes(void)
 {
  printf("  Nodelist:\n");
  N = S->Nodes;
  while (N!=NULL)
  {
   printf("    %11s",N->Name);
   if (N->Type == NODEtype_object) printf("  (object)   ");
   else if (N->Type == NODEtype_light) printf("  (light)   ");
   else if (N->Type == NODEtype_camera) printf("  (camera)   ");
   if (N->Parent  != NULL) printf(" P: %11s  ",N->Parent->Name);
   if (N->Child   != NULL) printf(" C: %11s  ",N->Child->Name);
   if (N->Brother != NULL) printf(" B: %11s  ",N->Brother->Name);
   printf("\n");
   N=N->Next;
  }
 }

 // ---

void ShowMaterials(void)
 {
  j = 0;
  printf("# ÄÄÄ MATERIALS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
  M = S->Materials;
  while (M!=NULL)
  {
   if (j!=0) printf("  # ÄÄÄÄÄÄÄÄÄÄ\n");
   printf("  Mat_Name               %s\n",M->Name);
   printf("  Mat_Flags              %i",M->Flags);
   if (M->Flags != 0)
   {
    printf("  (");
    if (M->Flags & MATflag_texture1)   printf(" texture");
    if (M->Flags & MATflag_texture2)   printf(" texture2");
    if (M->Flags & MATflag_opacity)    printf(" opacity");
    if (M->Flags & MATflag_reflection) printf(" reflection");
    if (M->Flags & MATflag_bump)       printf(" bump");
    if (M->Flags & MATflag_twosided)   printf(" twosided");
    if (M->Flags & MATflag_addtrans)   printf(" addtrans");
    if (M->Flags & MATflag_wireframe)  printf(" wireframe");
    printf(" )");
   }
   printf("\n");
   if (M->Flags & MATflag_texture1)   printf("  Mat_FileNameTexture1   %s\n",M->FileNameTexture1);
   if (M->Flags & MATflag_texture2)   printf("  Mat_FileNameTexture2   %s\n",M->FileNameTexture2);
   if (M->Flags & MATflag_opacity)    printf("  Mat_FileNameOpacity    %s\n",M->FileNameOpacity);
   if (M->Flags & MATflag_reflection) printf("  Mat_FileNameReflection %s\n",M->FileNameReflection);
   if (M->Flags & MATflag_bump)       printf("  Mat_FileNameBump       %s\n",M->FileNameBump);
   printf("  Mat_AmbientColor       %f %f %f\n",M->AmbientColor.r,M->AmbientColor.g,M->AmbientColor.b);
   printf("  Mat_DiffuseColor       %f %f %f\n",M->DiffuseColor.r,M->DiffuseColor.g,M->DiffuseColor.b);
   printf("  Mat_SpecularColor      %f %f %f\n",M->SpecularColor.r,M->SpecularColor.g,M->SpecularColor.b);
   M=M->Next;
   j++;
  }
 }

 // ---

void ShowObjects(void)
 {
  j =0;
  printf("# ÄÄÄ OBJECTS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
  N = S->Nodes;
  while (N!=NULL)
  {
   if (N->Type == NODEtype_object)
   {
    if (j!=0) printf("  # ÄÄÄÄÄÄÄÄÄÄ\n");
    O = (sObject *)N->Data;
    printf("  Obj_Name          %s\n",N->Name);
    printf("  Obj_Id            %i\n",N->Id);
    printf("  Obj_ParentId      %i\n",N->ParentId);
    if (N->Parent!=NULL) printf("  Obj_ParentName    %s\n",N->Parent->Name);

    printf("  Obj_Flags         %i",O->Flags);
    if (O->Flags != 0)
    {
     printf("  (");
     if (O->Flags & OBJflag_mapped)  printf(" mapped");
     if (O->Flags & OBJflag_hidden)  printf(" hidden");
     if (O->Flags & OBJflag_dummy)   printf(" dummy");
     if (O->Flags & OBJflag_ambient) printf(" ambient");
     printf(" )");
    }
    printf("\n");

    printf("  Obj_Type          %i\n",O->Type);
    printf("  Obj_Pivot         %f %f %f\n",O->Pivot.x,O->Pivot.y,O->Pivot.z);
    printf("  Obj_Matrix3DS     %f %f %f\n",O->Matrix3DS[0],O->Matrix3DS[1],O->Matrix3DS[2]);
    printf("                    %f %f %f\n",O->Matrix3DS[3],O->Matrix3DS[4],O->Matrix3DS[5]);
    printf("                    %f %f %f\n",O->Matrix3DS[6],O->Matrix3DS[7],O->Matrix3DS[8]);
    printf("  Obj_Pivot3DS      %f %f %f\n",O->Pivot3DS.x,O->Pivot3DS.y,O->Pivot3DS.z);
    printf("  Obj_NumVertices   %i\n",O->NumVertices);
    //minx =  999999; miny =  999999; minz =  999999;
    //maxx = -999999; maxy = -999999; maxz = -999999;
    //for (i=0;i<O->NumVertices;i++)
    //{
    // // find smallest and largest values (for debugging)
    // if (O->Vertices[i].Pos.x > maxx) maxx = O->Vertices[i].Pos.x;
    // if (O->Vertices[i].Pos.y > maxy) maxy = O->Vertices[i].Pos.y;
    // if (O->Vertices[i].Pos.z > maxz) maxz = O->Vertices[i].Pos.z;
    // if (O->Vertices[i].Pos.x < minx) minx = O->Vertices[i].Pos.x;
    // if (O->Vertices[i].Pos.y < miny) miny = O->Vertices[i].Pos.y;
    // if (O->Vertices[i].Pos.z < minz) minz = O->Vertices[i].Pos.z;
    // printf("    %f %f %f\n",O->Vertices[i].Pos.x,O->Vertices[i].Pos.y,O->Vertices[i].Pos.z);
    //}
    //printf("    # -\n");
    //printf("    Minx %f Maxx %f\n",minx,maxx);
    //printf("    Miny %f Maxy %f\n",miny,maxy);
    //printf("    Minz %f Maxz %f\n",minz,maxz);
    //printf("    # -\n");
    printf("  Obj_NumPolygons   %i\n",O->NumPolygons);
    for (i=0;i<O->NumPolygons;i++)
    {
     printf("    %i %i %i",O->Polygons[i].A,O->Polygons[i].B,O->Polygons[i].C);
     if (O->Polygons[i].Material != NULL) printf(" : %s\n",O->Polygons[i].Material->Name);
     else printf("\n");
    }
    if (O->Flags & OBJflag_mapped)
    {
     printf("  Obj_NumNormals    %i\n",O->NumNormals);
     for (i=0;i<O->NumNormals;i++)
     {
      printf("    %f %f\n",O->Normals[i].u,O->Normals[i].v);
     }
    }
    printf("  # Ä\n");
    printf("  Obj_NumPosKeys    %i\n",O->kfPos.NumKeys);
    K = O->kfPos.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      pos %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Obj_NumRotKeys    %i\n",O->kfRot.NumKeys);
    K = O->kfRot.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      rot %f %f %f %f (x,y,z,w)\n",K->Val._quat[qX],K->Val._quat[qY],K->Val._quat[qZ],K->Val._quat[qW]);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Obj_NumScaleKeys  %i\n",O->kfScale.NumKeys);
    K = O->kfScale.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      scale %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Obj_NumMorphKeys  %i\n",O->kfMorph.NumKeys);
    K = O->kfMorph.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      morph_to %s\n",K->Val._string);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    //HiddenFlag = 0;
    printf("  Obj_NumHideKeys   %i\n",O->kfHide.NumKeys);
    K = O->kfHide.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    j++;
   }
   N=N->Next;
  }
 }

 // ---

void ShowLights(void)
 {
  j = 0;
  printf("# ÄÄÄ LIGHTS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
  N = S->Nodes;
  while (N!=NULL)
  {
   if (N->Type == NODEtype_light)
   {
    if (j!=0) printf("  # ÄÄÄÄÄÄÄÄÄÄ\n");
    L = (sLight *)N->Data;
    printf("  Light_Name          %s\n",N->Name);
    printf("  Light_Id            %i\n",N->Id);
    printf("  Light_ParentId      %i\n",N->ParentId);
    if (N->Parent!=NULL) printf("  Light_ParentName    %s\n",N->Parent->Name);

    printf("  Light_Flags         %i\n",L->Flags);
    printf("  Light_Type          %i",L->Type);
    if (L->Type == LIGHTtype_omni) printf(" (omni)\n");
    else if (L->Type == LIGHTtype_spot) printf(" (spot)\n");
    printf("  Light_Pos           %f %f %f\n",L->Pos.x,L->Pos.y,L->Pos.z);
    printf("  Light_Rot           %f %f %f\n",L->Rot.x,L->Rot.y,L->Rot.z);
    printf("  Light_Target        %f %f %f\n",L->Target.x,L->Target.y,L->Target.z);
    printf("  Light_Pivot         %f %f %f\n",L->Pivot.x,L->Pivot.y,L->Pivot.z);
    //printf("  Light_Strength      %f\n",L->Strength);
    //printf("  Light_MaxRange      %f\n",L->MaxRange);
    printf("  Light_HotSpot       %f\n",L->HotSpot);
    printf("  Light_FallOff       %f\n",L->FallOff);
    printf("  Light_SpotRoll      %f\n",L->SpotRoll);
    printf("  # Ä\n");
    printf("  Light_NumPosKeys    %i\n",L->kfPos.NumKeys);
    K = L->kfPos.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      pos %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Light_NumTargetKeys %i\n",L->kfTarget.NumKeys);
    K = L->kfTarget.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      target %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    j++;
   }
   N=N->Next;
  }
 }

 // ---

void ShowCameras(void)
 {
  j = 0;
  printf("# ÄÄÄ CAMERAS ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");
  N = S->Nodes;
  while (N!=NULL)
  {
   if (N->Type == NODEtype_camera)
   {
    if (j!=0) printf("  # ÄÄÄÄÄÄÄÄÄÄ\n");
    C = (sCamera *)N->Data;
    printf("  Camera_Name          %s\n",N->Name);
    printf("  Camera_Id            %i\n",N->Id);
    printf("  Camera_ParentId      %i\n",N->ParentId);
    if (N->Parent!=NULL) printf("  Camera_ParentName    %s\n",N->Parent->Name);

    printf("  Camera_Flags         %i\n",C->Flags);
    printf("  Camera_Type          %i\n",C->Type);
    printf("  Camera_Pos           %f %f %f\n",C->Pos.x,C->Pos.y,C->Pos.z);
    printf("  Camera_Rot           %f %f %f\n",C->Rot.x,C->Rot.y,C->Rot.z);
    printf("  Camera_Target        %f %f %f\n",C->Target.x,C->Target.y,C->Target.z);
    printf("  Camera_Pivot         %f %f %f\n",C->Pivot.x,C->Pivot.y,C->Pivot.z);
    printf("  Camera_Roll          %f\n",C->Roll);
    printf("  Camera_Fov           %f\n",C->Fov);
    printf("  # Ä\n");
    printf("  Camera_NumPosKeys    %i\n",C->kfPos.NumKeys);
    K = C->kfPos.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      pos %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Camera_NumTargetKeys %i\n",C->kfTarget.NumKeys);
    K = C->kfTarget.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      target %f %f %f\n",K->Val._vector.x,K->Val._vector.y,K->Val._vector.z);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Camera_NumFovKeys    %i\n",C->kfFov.NumKeys);
    K = C->kfFov.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      fov %f\n",K->Val._float);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    printf("  Camera_NumRollKeys   %i\n",C->kfRoll.NumKeys);
    K = C->kfRoll.Keys;
    while(K!=NULL)
    {
     printf("    Frame %f\n",K->Frame);
     printf("      spline %f %f %f %f %f\n",K->Tens,K->Cont,K->Bias,K->EaseTo,K->EaseFrom);
     printf("      roll %f\n",K->Val._float);
     //printf("      ds: %f,%f,%f,%f\n",K->dsa,K->dsb,K->dsc,K->dsd);
     //printf("      dd: %f,%f,%f,%f\n",K->dda,K->ddb,K->ddc,K->ddd);
     K=K->Next;
    }
    j++;
   }
   N=N->Next;
  }
 }


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void main(int argc, char **argv)
 {

  if (argc != 2)
  {
   printf("\n3DS file dumper - Axon/Inf^Xenon\n");
   printf("usage:   INFO <3DS-file>\n");
   exit(1);
  }
  S = Scene_New(MaxPolys);
  if (Scene_Load3DS(S,argv[1]) != Error3DS_NoError)
  {
   printf("Error loading %s\n",argv[1]);
   exit(1);
  }

  ShowScene();
  ShowNodes();
  ShowMaterials();
  ShowObjects();
  ShowLights();
  ShowCameras();
 }

