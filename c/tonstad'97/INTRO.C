 // ÄÄÄÄÄ INCLUDE ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <stdlib.h>  // malloc
 #include <stdio.h>   // printf, sprintf
 #include <conio.h>   // getch
 #include <string.h>  // memset, memcpy
 #include <math.h>    // sin, cos

 #include "engine.h"
 #include "music.h"
 #include "font.h"
 #include "routines.h"

 #include "voxel.h"

 #include "tex_pic.inc"
 #include "tex_pal.inc"
 #include "blob.inc"

 #include "logo.inc"

 // ÄÄÄÄÄ defines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #define UVmult 256
 #define vertex_multiplier 100

 // ÄÄÄÄÄ Variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 int Music_Playing;
 int LastMusicPos;
 int Quit;

 char *Background;
 char *Buffer;
 char *Screen;
 char *BitMap;
 char Pal[768];
 char Palette[768];

 int Skurr;

 char *Voxel_HeightMap;
 char *Voxel_ColorMap;

 int voxel_xx,voxel_yy;
 float voxel_ddir;


 // -----

 char string[256];
 char *str;
 unsigned short int x;

 // -----

 float angle,angleadder;
 float scale,scaleadder;

 // -----

 char *sp_map;
 char *sp_map_texture;
 float u,v,fu,fv;

 // --- data for 3d ---

 sCamera *C;
 sObject *O;
 sMaterial *M;

 int TekstIndex;
 #define NumTekst 36
 char *Tekst[NumTekst] = {
  "lights have faded",
  "euphoria is dead",
  "past and present",
  "exalt of the weird",
  "chimerias petrified",
  "eclipse and suspicion",
  "deafened stream",
  "babylon fell",
  "distraction and envy",
  "babylon fell",
  "festal perishing",
  "babylon fell",
  "dreams drift",
  "in the frozen wind",
  "and mysteries",
  "are reborn",
  "we rose from sand",
  "and stone",
  "to follow",
  "the light's allure",
  "tears drift",
  "in the shadows sleep",
  "turn innocence",
  "into excess",
  "fragments of",
  "a dying world",
  "and destiny",
  "lies beneath",
  "sceptres of bombast",
  "tracks into myth",
  "fetish and feud",
  "dance of recesses",
  "fierce laughter",
  "banned into stone",
  "eyes of guilt",
  "covered by sand" };

 // -------------------------------------------------------------------------

void flash (char *pal, int num)
 {
  int i;

  for (i=0;i<256;i++) SetRGB(i,63,63,63);
  for (i=0;i<num;i++) WaitVR();
  for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[i*3+1],pal[i*3+2]);
 }

 // -------------------------------------------------------------------------

void DrawLogo( char *Buffer )
 {
  int x,y;
  unsigned char *p;
  char c;

  p = Logo;
  for (y=0;y<(32*320);y+=320)
  {
   for (x=150;x<320;x++)
   {
    c = *p++;
    if (c!=0) Buffer[y+x] = c;
   }
  }
 }

 // -------------------------------------------------------------------------



void Init_3D()
 {
  FILE *fp;
  int i;

  // "engine-init"
  printf("ş Initializing 3D engine.\n");
  _Buffer = (char *)malloc(64000);
  _Screen = (char *)0xA0000;
  MinX = 0;  MaxX = 319;
  MinY = 0;  MaxY = 199;
  ScreenWidth = 320;
  ScreenHeight = 200;
  TopClip   = 0;
  BotClip   = 199;
  LeftClip  = 0;
  RightClip = 319;
  World = NewWorld(1);
  #define maxpolys 1000
  World->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  World->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  // Load 3DS file
  printf("ş Loading 3DS file.\n");
  // Load3DS("blob1.3ds",World);
  Parse3DS( Blob, 24695, World );

  // Calc face/vertex normals, apply spherical mapping
  printf("ş Init objects.\n");
  O = World->Objects;
  while (O!=NULL)
  {
   Object_Center(O);
   for (i=0;i<O->NumVertices;i++)
   {
    O->Vertices[i].Pos.x *= vertex_multiplier;
    O->Vertices[i].Pos.y *= vertex_multiplier;
    O->Vertices[i].Pos.z *= vertex_multiplier;
   }
   Object_CalcFaceNormals(O);
   Object_CalcVertexNormals(O);

   O->Pos.x = 0;
   O->Pos.y = 0;
   O->Pos.z = 0;

   O = O->Next;
  }
  // load a default texture for all materials
  printf("ş Loading textures.\n");
  M = World->Materials;
  while (M!=NULL)
  {
   // M->Tex1 = (char *)malloc(65536);
   // fp = fopen("texture.pix","rb");
   M->Tex1 = Tex_Pic;
   // fread(M->Tex1,1,65536,fp);
   // fclose(fp);
   M=M->Next;
  }
  // fp = fopen("texture.pal","rb");
  memcpy(Pal,Tex_Pal,768);
  // fread(Pal,1,768,fp);
  // fclose(fp);
  // Camera setup
  printf("ş Set up camera.\n");
  if (World->Cameras == NULL)
  {
   printf("  No camera in 3DS file. Set up default amera.\n");
   World->Camera = NewCamera(1);
   // ... etc ...
  }
  else World->Camera = World->Cameras;
  World->Camera->UpVector.x = 0;
  World->Camera->UpVector.y = 1;
  World->Camera->UpVector.z = 0;
  World->Camera->Pos.x = 0;
  World->Camera->Pos.y = 0;
  World->Camera->Pos.z = -800;
  World->Camera->Target.x = -200;
  World->Camera->Target.y = -100;
  World->Camera->Target.z = 0;
  World->Camera->CAMERA_keyframe = 0;
  World->Camera->Type = 1;
  World->Camera->FOV = 60;
 }

 // -------------------------------------------------------------------------

void R_Setup( sWorld *World)
 {
  sObject *O;
  sCamera *C;
  float TempFloat;

  C = World->Camera;
  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  Camera_CreateMatrix_UVN( C );

  #define SCREEN_WIDTH 320
  #define SCREEN_HEIGHT 200
  #define ASPECT_RATIO 1.2
  #define DEG2RAD(deg) ((deg)*PI/180.0)
  #define FOV2FOC(width,angle) (((width)*cos((angle)/2.0))/(sin((angle)/2.0)*2))
  #define FocalDistaceX = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV))
  #define FocalDistaneY = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV))
  World->ViewDistanceX = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV));
  World->ViewDistanceY = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV));

  O = World->Objects;
  while (O!=NULL)
  {
   Object_CreateMatrix_Free(O,World);
   O = O->Next;
  }
  Handle_Object( World->Objects, World );
  SortWorldObject( World );
 }

 // -------------------------------------------------------------------------

void R_Draw( sWorld *World)
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
   Poly_Tex( World->RenderList[i] );
  }
 }


//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² MAIN ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 void main(void)
 {
  int i;
  FILE *fp;

  // disable keyboard
// outp(0x21,(inp(0x21)|2));

  printf("INTRO - Xenon 1997..\n");

 // ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

  // Initialize music

  Music_Playing=0;
  Music_Init();

  // initialize all kinds of tables

  Font_Setup();
  Init_Mul320Table();

  // memory allocation

  Buffer     = (char *)malloc(64000);
  BitMap     = (char *)malloc(65536);
  Background = (char *)malloc(65536);
  Voxel_HeightMap = (char *)malloc(65536);
  memset(Voxel_HeightMap,0,65536);
  Voxel_ColorMap  = (char *)malloc(65536);
  memcpy(Voxel_ColorMap,VoxelPic,65536);

  // calculation

  memset(Background,0,65536);
  CalcBackground(Background,0);
  Smooth(Background);

  // --- voxel init ---

  MakeMulTable();        // voxel mul tables
  createfractalmap(Voxel_HeightMap,0,0,255,255);
  for (i=0;i<5;i++) smoothmap(Voxel_HeightMap);

  memcpy(Voxel_ColorMap,VoxelPic,65536);

  // --- sphere mapping init ---

  sp_map = (char *)malloc(320*200*2);
  Init_SphereMap(sp_map);

  fp = fopen("sphere.map","wb");
  fwrite(sp_map,1,128000,fp);
  fclose(fp);

  sp_map_texture = VoxelPic;

  Skurr_Init();

  // --- 3d variables ---

  Init_3D();
  printf("ş 3D engine ready for action...\n");

  // copy background to rotozoom buffer

  memcpy(BitMap,Background,64000);

  // start music

  Music_Start();
  Music_Playing=1;

  // set variables

  Quit = 0;
  Screen     = (char *)0xA0000;

  // --- init gfx mode ---

  SetMode(0x13);
  for (i=0;i<18;i++)
  {
   Palette[i*3]   = i*3;
   Palette[i*3+1] = i*1;
   Palette[i*3+2] = i*2;
   // SetRGB(i,i*3,i*1,i*2);
  }
  for (i=0;i<16;i++)
  {
   Palette[(i+32)*3]   = i*4;
   Palette[(i+32)*3+1] = i*4;
   Palette[(i+32)*3+2] = i*4;
   // SetRGB(i+32,i*4,i*4,i*4);
  }

 // ±±±±± Main ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// --- Part 1 --- rotozoom w/writer ---

  angle = 0;
  scale = 512;
  angleadder = 0;
  scaleadder = 0;

  flash(&Palette,5);

  while (Music_GetPos() < 0x0400)
  {
   // x = Music_GetPos();
   // sprintf(string,"%x %x",(x>>8),(x&25));
   // Font_DrawString(Screen,100,100,string);
   DrawRotoZoom(Buffer,BitMap,sin(angle)*1, 400+sin(scale)*100);
        if (Music_GetPos() >= 0x0320) str = "design ... what design???";
   else if (Music_GetPos() >= 0x0300) str = "gfx ... what gfx???";
   else if (Music_GetPos() >= 0x0220) str = "music ... mike x";
   else if (Music_GetPos() >= 0x0200) str = "code ... axon";
   else if (Music_GetPos() >= 0x0120) str = "tonstad 97";
   else if (Music_GetPos() >= 0x0100) str = "an intro";
   else if (Music_GetPos() >= 0x0020) str = "...still alive...";
   else str = "xenon development";

   i = Font_StringWidth(str);
   Font_DrawString(Buffer,160-(i/2),90,str);

   WaitVR();
   memcpy(Screen,Buffer,64000);
   angle += 0.003;
   scale += 0.03;

   if (inp(0x60) == 1) goto EndOfIntro;
  }

// --- Part 2 --- voxel ---

  flash(VoxelPal,5);

  //for (i=0;i<256;i++)
  //  SetRGB(i,VoxelPal[i*3],VoxelPal[i*3+1],VoxelPal[i*3+2]);
  ////  SetRGB(i,i/4,i/4,i/4);

  voxel_xx = 128;
  voxel_yy = 128;
  voxel_ddir = 0;
  lastzobs = 255;
  TekstIndex = 0;
  Skurr = SkurrNum;

  while (Music_GetPos() < 0x0800)
  {
   memset(Buffer,0,64000);
   DrawVoxel(voxel_xx,voxel_yy,voxel_ddir,Voxel_HeightMap,Voxel_ColorMap,Buffer);
   voxel_ddir+=0.005;
   if (voxel_ddir>359) voxel_ddir-=360;
   if (voxel_ddir<0) voxel_ddir+=360;
   voxel_xx+=1;
   voxel_xx&=255;
   voxel_yy+=1;
   voxel_yy&=255;

   str = Tekst[TekstIndex];
   i = Font_StringWidth(str);
   Font_DrawString(Buffer,160-(i/2),60,str);

   i = Music_GetPos();
   if (((i & 3) == 0) && (i != LastMusicPos))
   {
    TekstIndex++;
    if (TekstIndex >= NumTekst) TekstIndex = 0;
   }
   LastMusicPos = i;

   DrawLogo(Buffer);

   i = Music_GetPos()&0xff;
   if (i==0) Skurr = SkurrNum;
   WaitVR();
   Skurr_Draw(Screen,Buffer,Skurr);
   Skurr--;

   if (inp(0x60) == 1) goto EndOfIntro;
  }

// --- Part 3 --- voxel + 3d + "lyrics" ---

  _Buffer = Buffer;

  flash(VoxelPal,5);

  while (Music_GetPos() < 0x0c00)
  {
   memset(Buffer,0,64000);
   DrawVoxel(voxel_xx,voxel_yy,voxel_ddir,Voxel_HeightMap,Voxel_ColorMap,Buffer);
   voxel_ddir+=0.005;
   if (voxel_ddir>359) voxel_ddir-=360;
   if (voxel_ddir<0) voxel_ddir+=360;
   voxel_xx+=1;
   voxel_xx&=255;
   voxel_yy+=1;
   voxel_yy&=255;

   O = World->Objects;
   Object_AddRot(O,0.01,0.006,0.003);
   R_Setup( World );
   R_Draw( World );

   DrawLogo(Buffer);

   i = Music_GetPos()&0xff;
   if ((i==20)||(i==26)||(i==52)||(i==58)) Skurr = SkurrNum;
   WaitVR();
   Skurr_Draw(Screen,Buffer,Skurr);
   Skurr--;
 //  WaitVR();
 //  memcpy(Screen,Buffer,64000);
   if (kbhit()) goto EndOfIntro;
  }

// --- Part 4 --- spherical mapping + greetings ---

  flash(VoxelPal,5);
  memset((char *)0xA0000,0,64000);

  while (Music_GetPos() < 0x0f00)
  {
   u = sin(fu)*255;
   v = cos(fv)*255;
   fu+= 0.006;
   fv+= 0.010;
   DrawSphere(sp_map,Buffer,u,v,sp_map_texture);

 // doomsday fudge index sorrox
 // infernal klf nocturnal $een
 // inf spin sublogic talent tls

        if (Music_GetPos() >= 0x0e20) str = "funcom";
   else if (Music_GetPos() >= 0x0e00) str = "doomsday, fudge, spin";
   else if (Music_GetPos() >= 0x0d20) str = "infernal, seen, sorrox";
   else if (Music_GetPos() >= 0x0d00) str = "tls, index, sublogic, klf";
   else if (Music_GetPos() >= 0x0c20) str = "nocturnal, talent, inf";
   else str = "greetings to";

   i = Font_StringWidth(str);
   Font_DrawString(Buffer,160-(i/2),90,str);

   DrawLogo(Buffer);
   WaitVR();
   memcpy(Screen,Buffer,64000);

   if (inp(0x60) == 1) goto EndOfIntro;
  }

// --- Part 5 --- stort 3d object ---

  _Buffer = Buffer;
  World->Camera->Pos.x = 0;
  World->Camera->Pos.y = 0;
  World->Camera->Pos.z = -250;
  World->Camera->Target.x = 0;
  World->Camera->Target.y = 0;
  World->Camera->Target.z = 0;

  flash(VoxelPal,5);
  LastMusicPos = 0;

  while (Music_GetPos() < 0x1110)
  {
   memset(Buffer,0,64000);

   O = World->Objects;
   Object_AddRot(O,0.02,0.012,0.006);
   R_Setup( World );
   R_Draw( World );

   i = Music_GetPos();
   if (((i & 15) == 0) && (i != LastMusicPos)) flash(VoxelPal,3);
   LastMusicPos = i;

   DrawLogo(Buffer);

   i = Music_GetPos()&0xff;
   if ((i==20)||(i==26)||(i==52)||(i==58)) Skurr = SkurrNum;
   WaitVR();
   Skurr_Draw(Screen,Buffer,Skurr);
   Skurr--;

   // WaitVR();
   // memcpy(Screen,Buffer,64000);

   if (inp(0x60) == 1) goto EndOfIntro;
  }

// --- Part 6 --- end writer ---


  flash(&Palette,10);

  memcpy((char *)0xA0000,Background,64000);

  str = "this was the lousy";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2),40,str);
  str = "party version.";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2),60,str);



  str = "a proper version will";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2), 90,str);
  str = "maybe be released";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2),110,str);
  str = "later...   ...maybe";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2),130,str);
  str = "read the .nfo file";
  i = Font_StringWidth(str);
  Font_DrawString((char *)0xA0000,160-(i/2),160,str);

  for (i=0;i<70*5;i++) WaitVR();

 // ±±±±± Exit ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

EndOfIntro:

  // enable keyboard
//  outp(0x21,(inp(0x21)&253));

  free(Buffer);
  free(BitMap);
  free(Background);
  free(Voxel_HeightMap);
  free(Voxel_ColorMap);
  free(sp_map);

  SetMode(0x3);
  if (Music_Playing == 1) Music_Stop();
  if (Music_Playing == 1) Music_DeInit();
 }



