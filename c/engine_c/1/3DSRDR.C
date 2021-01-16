 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>

 #ifndef PI
  #define PI 3.141592687
 #endif

 typedef unsigned char  byte;
 typedef unsigned short word;
 typedef unsigned long  dword;

 typedef signed char  sbyte;
 typedef signed short sword;
 typedef signed long  sdword;

 #pragma pack(2)

 typedef struct
 {
  word    id;
  dword   len;
 } TChunkHeader, *PChunkHeader;

 #pragma pack()

 enum
 {
  CHUNK_RGBF          = 0x0010,
  CHUNK_RGBB          = 0x0011,
  //CHUNK_RBGB2       = 0x0012,       // ?? NOT HLS.
  CHUNK_WRD           = 0x0030,

  CHUNK_PRJ           = 0xC23D,
  CHUNK_MLI           = 0x3DAA,
  CHUNK_SHP           = 0x2D2D,
  CHUNK_LFT           = 0x2D3D,

  CHUNK_MAIN          = 0x4D4D,
  CHUNK_OBJMESH       = 0x3D3D,
  CHUNK_BKGCOLOR      = 0x1200,
  CHUNK_AMBCOLOR      = 0x2100,
  CHUNK_OBJBLOCK      = 0x4000,
  CHUNK_TRIMESH       = 0x4100,
  CHUNK_VERTLIST      = 0x4110,
  CHUNK_VERTFLAGS     = 0x4111,
  CHUNK_FACELIST      = 0x4120,
  CHUNK_FACEMAT       = 0x4130,
  CHUNK_MAPLIST       = 0x4140,
  CHUNK_SMOOLIST      = 0x4150,
  CHUNK_TRMATRIX      = 0x4160,
  CHUNK_MESHCOLOR     = 0x4165,
  CHUNK_TXTINFO       = 0x4170,
  CHUNK_LIGHT         = 0x4600,
  CHUNK_LIGHTR1       = 0x4659,
  CHUNK_LIGHTR2       = 0x465A,
  CHUNK_LIGHTMUL      = 0x465B,
  CHUNK_SPOTLIGHT     = 0x4610,
  CHUNK_CAMERA        = 0x4700,
  CHUNK_HIERARCHY     = 0x4F00,
  CHUNK_VIEWPORT      = 0x7001,
  CHUNK_MATERIAL      = 0xAFFF,
  CHUNK_MATNAME       = 0xA000,
  CHUNK_AMBIENT       = 0xA010,
  CHUNK_DIFFUSE       = 0xA020,
  CHUNK_SPECULAR      = 0xA030,
  CHUNK_SHININESS     = 0xA040,
  CHUNK_SHSTRENGTH    = 0xA041,
  CHUNK_TRANSP        = 0xA050,
  CHUNK_TRFALLOFF     = 0xA052,
  CHUNK_REFLECTBL     = 0xA053,
  CHUNK_FLAG2SIDE     = 0xA081,
  CHUNK_FLAGADD       = 0xA083,
  CHUNK_SELFILLUM     = 0xA084,
  CHUNK_WIREFRAME     = 0xA085, // Flag
  CHUNK_WIRETHICK     = 0xA087, // Float
  CHUNK_FACEMAP       = 0xA088,   // Flag
  CHUNK_FLAGIN        = 0xA08A,
  CHUNK_SOFTEN        = 0xA08C,    // Flag
  CHUNK_FLAGUNIT      = 0xA08E,  // Very useless
  CHUNK_SHADETYPE     = 0xA100,
  CHUNK_TEXTURE       = 0xA200,
  CHUNK_TEXTUREMASK   = 0xA33E,
  CHUNK_TEXTURE2      = 0xA33A,
  CHUNK_TEXTUREMASK2  = 0xA340,
  CHUNK_REFMAP        = 0xA220,
  CHUNK_REFMASK       = 0xA34C,
  CHUNK_OPACITYMAP    = 0xA210,
  CHUNK_OPACITYMASK   = 0xA342,
  CHUNK_SPECULARMAP   = 0xA204,
  CHUNK_SPECULARMASK  = 0xA348,
  CHUNK_SHININESSMAP  = 0xA33C,
  CHUNK_SHININESSMASK = 0xA346,
  CHUNK_SELFILLUMMAP  = 0xA33D,
  CHUNK_SELFILLUMMASK = 0xA34A,
  CHUNK_BUMPMAP       = 0xA230,
  CHUNK_BUMPMASK      = 0xA344,
  CHUNK_MAPFILE       = 0xA300,
  CHUNK_TFLAGS        = 0xA351, // Flags
  CHUNK_BLUR          = 0xA353, // Float
  CHUNK_USCALE        = 0xA354, // Float
  CHUNK_VSCALE        = 0xA356, // Float
  CHUNK_UOFFSET       = 0xA358, // Float
  CHUNK_VOFFSET       = 0xA35A, // Float
  CHUNK_TROTATION     = 0xA35C, // Float
  CHUNK_BLACKTINT     = 0xA360, // Float
  CHUNK_WHITETINT     = 0xA362, // Float
  CHUNK_RTINT         = 0xA364, // Float
  CHUNK_GTINT         = 0xA366, // Float
  CHUNK_BTINT         = 0xA368, // Float
  CHUNK_KEYFRAMER     = 0xB000,
  CHUNK_AMBIENTKEY    = 0xB001,
  CHUNK_TRACKINFO     = 0xB002,
  CHUNK_TRACKOBJNAME  = 0xB010,
  CHUNK_TRACKPIVOT    = 0xB013,
  CHUNK_TRACKPOS      = 0xB020,
  CHUNK_TRACKROTATE   = 0xB021,
  CHUNK_TRACKSCALE    = 0xB022,
  CHUNK_TRACKMORPH    = 0xB026,
  CHUNK_TRACKHIDE     = 0xB029,
  CHUNK_OBJNUMBER     = 0xB030,
  CHUNK_TRACKCAMERA   = 0xB003,
  CHUNK_TRACKFOV      = 0xB023,
  CHUNK_TRACKROLL     = 0xB024,
  CHUNK_TRACKCAMTGT   = 0xB004,
  CHUNK_TRACKLIGHT    = 0xB005,
  CHUNK_TRACKLIGTGT   = 0xB006,
  CHUNK_TRACKSPOTL    = 0xB007,
  CHUNK_FRAMES        = 0xB008,
 };

 // ------------------------------------

    // Forward declaration.
void ChunkReader (FILE *f, int ind, long p);

void SkipReader (FILE *f, int ind, long p) {}

void RGBFReader (FILE *f, int ind, long p)
 {
  float c[3];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %f %f %f\n",c[0],c[1],c[2]);
 }

void RGBBReader (FILE *f, int ind, long p)
 {
  byte c[3];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %d %d %d\n",c[0], c[1], c[2]);

 }

void WRDReader (FILE *f, int ind, long p)
 {
  word value;
  if (fread(&value, sizeof(value), 1, f) != 1) return;
  printf("  %d\n",value);
 }

void FloatReader (FILE *f, int ind, long p)
 {
  float value;
  if (fread(&value, sizeof(value), 1, f) != 1) return;
  printf("  %f\n",value);
 }

void TextFlagReader (FILE *f, int ind, long p)
 {
  int i=0;
  byte flag1, flag2;

  if (fread(&flag1, sizeof(flag1), 1, f) != 1) return;
  if (fread(&flag2, sizeof(flag2), 1, f) != 1) return;
  printf("  %02X %02X\n", flag1,flag2);
  i++;
  printf("  Source ");
  if((flag1 & 0xc0)==0x80) printf("RGB_luma_tint\n");
  else
  {
   if((flag1 & 0xc0)==0xc0) printf("Alpha_tint\n");
   else
   {
    if((flag2 & 0x02)==0x02) printf("RGB_tint\n");
    else printf("RGB\n");
   }
  }
  printf("  Filtering ");
  if (flag1 & 0x40) printf("Summed_arial\n");
  else printf("Pyramidal\n");
  printf("  Other_parameters ");
  switch((flag1 & 0x11))
  {
   case 0x11 : printf("Decal"); break;
   case 0x00 : printf("Tile"); break;
   case 0x10 :
   case 0x01 : printf("Tile_&_Decal");
  }
  if(flag2 & 1) printf(" Ignore_map_alpha");
  if(flag1 & 8) printf(" Negative");
  if(flag1 & 2) printf(" Mirror");
  printf("\n");
 }

void SHTReader (FILE *f, int ind, long p)
 {
  word value;

  if (fread(&value, sizeof(value), 1, f) != 1) return;
  switch(value)
  {
   case 1: printf("  Flat\n"); break;
   case 2: printf("  Gouraud\n"); break;
   case 3: printf("  Phong\n"); break;
   case 4: printf("  Metal\n"); break;
  }
 }

void ASCIIZReader (FILE *f, int ind, long p)
 {
  int c;

	// Read ASCIIZ name
  printf("  ");
  while ( (c = fgetc(f)) != EOF && c != '\0')
	putchar(c);
  printf("\n");
 }

void ObjBlockReader (FILE *f, int ind, long p)
 {
  int c;

	// Read ASCIIZ object name
  ASCIIZReader(f, ind, p);
	// Read rest of chunks inside this one.
  ChunkReader(f, ind, p);
 }

void VertListReader (FILE *f, int ind, long p)
 {
  word nv;
  float c[3];

  if (fread(&nv, sizeof(nv), 1, f) != 1) return;
  printf("  %d\n",nv);
  while (nv-- > 0)
  {
   if (fread(&c, sizeof(c), 1, f) != 1) return;
   printf("  %f %f %f\n",c[0], c[1], c[2]);
  }
 }

void FaceListReader (FILE *f, int ind, long p)
 {
  word nv;
  word c[3];
  word flags;

  if (fread(&nv, sizeof(nv), 1, f) != 1) return;
  printf("  %d\n",nv);
  while (nv-- > 0)
  {
   if (fread(&c, sizeof(c), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %d %d %d 0x%X ",c[0], c[1], c[2], flags);
   printf("%d %d %d %d %d\n",
            (flags & 0x04) != 0, (flags & 0x02) != 0, (flags & 0x01) != 0,
            (flags & 0x08) != 0, (flags & 0x10) != 0);
  }
	// Read rest of chunks inside this one.
  ChunkReader(f, ind, p);
 }

void FaceMatReader (FILE *f, int ind, long p)
 {
  int c;
  word n, nf;

	// Read ASCIIZ material name
  ASCIIZReader(f, ind, p);
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %d\n", n);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   printf("  %d\n", nf);
  }
 }

void MapListReader (FILE *f, int ind, long p)
 {
  word nv;
  float c[2];

  if (fread(&nv, sizeof(nv), 1, f) != 1) return;
  printf("  %d\n", nv);
  while (nv-- > 0)
  {
   if (fread(&c, sizeof(c), 1, f) != 1) return;
   printf("  %f %f\n", c[0], c[1]);
  }
 }

void SmooListReader (FILE *f, int ind, long p)
 {
  dword s;
  int i;

  while (ftell(f) < p)
  {
   if (fread(&s, sizeof(s), 1, f) != 1) return;
   //for (i = 0; i < 32; i++)
   // if (s & ((dword)1 << i)) printf("%d, ", i + 1);
   printf("  %d\n",s);
  }
 }

void TrMatrixReader(FILE *f, int ind, long p)
 {
  float rot[9];
  float trans[3];

  if (fread(&rot, sizeof(rot), 1, f) != 1) return;
  printf("  %f %f %f\n", rot[0], rot[1], rot[2]);
  printf("  %f %f %f\n", rot[3], rot[4], rot[5]);
  printf("  %f %f %f\n", rot[6], rot[7], rot[8]);
  if (fread(&trans, sizeof(trans), 1, f) != 1) return;
  printf("  %f %f %f\n",trans[0],trans[1],trans[2]);
 }

void LightReader(FILE *f, int ind, long p)
 {
  float c[3];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %f %f %f\n", c[0], c[1], c[2]);
	// Read rest of chunks inside this one.
  ChunkReader(f, ind, p);
 }

void SpotLightReader(FILE *f, int ind, long p)
 {
  float c[5];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %f %f %f\n", c[0], c[1], c[2]);
  printf("  %f %f\n",c[3], c[4]);
 }

void CameraReader(FILE *f, int ind, long p)
 {
  float c[8];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %f %f %f\n",c[0], c[1], c[2]);
  printf("  %f %f %f\n", c[3], c[4], c[5]);
  printf("  %f %f\n",c[6], c[7]);
 }

void MatNameReader (FILE *f, int ind, long p)
 {
  int c;

	// Read ASCIIZ object name
  ASCIIZReader(f, ind, p);
 }

void MapFileReader(FILE *f, int ind, long p)
 {
  int c;

	// Read ASCIIZ filename
  ASCIIZReader(f, ind, p);
 }

void FramesReader(FILE *f, int ind, long p)
 {
  dword c[2];
  if (fread(&c, sizeof(c), 1, f) != 1) return;
  printf("  %ld %ld\n",ind, "", c[0], c[1]);
 }

void TrackObjNameReader(FILE *f, int ind, long p)
 {
  int c;
  word w[2];
  word parent;

	// Read ASCIIZ name
  ASCIIZReader(f, ind, p);
  if (fread(&w, sizeof(w), 1, f) != 1) return;
  if (fread(&parent, sizeof(parent), 1, f) != 1) return;
  printf("  0x%X 0x%X %d\n",w[0], w[1], parent);
 }

void PivotPointReader(FILE *f, int ind, long p)
 {
  float pos[3];

  if (fread(&pos, sizeof(pos), 1, f) != 1) return;
  printf("  %f %f %f\n",pos[0], pos[1], pos[2]);
 }

 /*
    Key info flags for position, rotation and scaling:
    Until I know the meaning of each bit in flags I assume all mean
    a following float data.
 */

 // NOTE THIS IS NOT A CHUNK, but A PART OF SEVERAL CHUNKS
void SplineFlagsReader(FILE *f, int ind, word flags)
 {
  int i;
  float dat;

  printf("  ");
  for (i = 0; i < 16; i++)
  {
   if (flags & (1 << i))
   {
    if (fread(&dat, sizeof(dat), 1, f) != 1) return;
    //if (i < sizeof(flagnames)/sizeof(*flagnames))
    //printf("%*s   %-15s = %f\n",ind, "", flagnames[i], dat);
    //else printf("%*s   %-15s = %f\n",ind, "", "Unknown", dat);
    printf("%f ",dat);
   }
  }
  printf("\n");
 }

void TrackPosReader(FILE *f, int ind, long p)
 {
  word n, nf;
  float pos[3];
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n",n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  //printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i", nf, flags);
   SplineFlagsReader(f, ind, flags);
   if (fread(&pos, sizeof(pos), 1, f) != 1) return;
   printf("  %f %f %f\n",pos[0], pos[1], pos[2]);
  }
 }

void TrackRotReader(FILE *f, int ind, long p)
 {
  word n, nf;
  float pos[4];
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n",n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  // printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i", nf, flags);
   SplineFlagsReader(f, ind, flags);
   if (fread(&pos, sizeof(pos), 1, f) != 1) return;
   printf("  %f %f %f %f\n",pos[0], pos[1], pos[2], pos[3]);
  }
 }

void TrackScaleReader(FILE *f, int ind, long p)
 {
  word n, nf;
  float pos[3];
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n", n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  // printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i",nf, flags);
   SplineFlagsReader(f, ind, flags);
   if (fread(&pos, sizeof(pos), 1, f) != 1) return;
   printf("  %f %f %f\n", pos[0], pos[1], pos[2]);
  }
 }

void TrackFovReader(FILE *f, int ind, long p)
 {
  word n, nf;
  float fov;
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n", n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  // printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i", nf, flags);
   SplineFlagsReader(f, ind, flags);
   if (fread(&fov, sizeof(fov), 1, f) != 1) return;
   printf("  %f\n", fov);
  }
 }

void TrackRollReader(FILE *f, int ind, long p)
 {
  word n, nf;
  float roll;
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n", n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  // printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i", nf, flags);
   SplineFlagsReader(f, ind, flags);
   if (fread(&roll, sizeof(roll), 1, f) != 1) return;
   printf("  %f\n",roll);
  }
 }

void TrackMorphReader(FILE *f, int ind, long p)
 {
  word n, nf;
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   // printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n", n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  //printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  while (n-- > 0)
  {
   if (fread(&nf, sizeof(nf), 1, f) != 1) return;
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   if (fread(&flags, sizeof(flags), 1, f) != 1) return;
   printf("  %i %i",nf, flags);
   SplineFlagsReader(f, ind, flags);
   //printf("%*s   Object name: \"", ind, "");
   ASCIIZReader(f, ind, p);
  }
 }

void TrackHideReader(FILE *f, int ind, long p)
 {
  word n, nf;
  word frame;
  word unkown;
  word flags;
  int i;

  for(i=0; i<5; i++)
  {
   if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
   //printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
  }
  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %i\n", ind, "", n);
  if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
  //printf("%*sUnknown: 0x%x\n", ind, "", unkown);
  ind += 2;
  while (n-- > 0)
  {
   if (fread(&frame, sizeof(frame), 1, f) != 1) return;
   printf("  %i\n",(dword)frame);
   for(i=0; i<2; i++)
   {
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    // printf("%*s  Unknown #%d: 0x%x\n", ind, "", i, unkown);
   }
  }
 }

void ObjNumberReader(FILE *f, int ind, long p)
 {
  word n;

  if (fread(&n, sizeof(n), 1, f) != 1) return;
  printf("  %d\n", n);
 }

// ------------------------------------

struct // ChunkNames
 {
  word id;
  const char *name;
  void (*func)(FILE *f, int ind, long p);
 } ChunkNames[] =
 {
  {CHUNK_RGBF,          "RGB_float",                        RGBFReader},
  {CHUNK_RGBB,          "RGB_byte",                         RGBBReader},
  {CHUNK_WRD,           "WORD",                             WRDReader},

  {CHUNK_PRJ,           "Project",                          NULL},
  {CHUNK_MLI,           "Material_Library",                 NULL},
  {CHUNK_SHP,           "Shape_file",                       NULL},
  {CHUNK_LFT,           "Loft_file",                        NULL},

  {CHUNK_MAIN,          "Main",                             NULL},
  {CHUNK_OBJMESH,       "Object_Mesh",                      NULL},
  {CHUNK_BKGCOLOR,      "Background_color",                 NULL},
  {CHUNK_AMBCOLOR,      "Ambient_color",                    NULL},
  {CHUNK_OBJBLOCK,      "Object_Block",                     ObjBlockReader},
  {CHUNK_TRIMESH,       "Tri_Mesh",                         NULL},
  {CHUNK_VERTLIST,      "Vertex_list",                      VertListReader},
  {CHUNK_VERTFLAGS,     "Vertex_flag_list",                 SkipReader},
  {CHUNK_FACELIST,      "Face_list",                        FaceListReader},
  {CHUNK_MESHCOLOR,     "Mesh_color",                       SkipReader},
  {CHUNK_FACEMAT,       "Face_material",                    FaceMatReader},
  {CHUNK_MAPLIST,       "Mappings-list",                    MapListReader},
  {CHUNK_TXTINFO,       "Texture_info",                     SkipReader},
  {CHUNK_SMOOLIST,      "Smoothings",                       SmooListReader},
  {CHUNK_TRMATRIX,      "Matrix",                           TrMatrixReader},
  {CHUNK_LIGHT,         "Light",                            LightReader},
  {CHUNK_LIGHTR1,       "Light_Range radius1",              FloatReader},
  {CHUNK_LIGHTR2,       "Light_Range radius2",              FloatReader},
  {CHUNK_LIGHTMUL,      "Light_Multyplity",                 FloatReader},
  {CHUNK_SPOTLIGHT,     "Spotlight",                        SpotLightReader},
  {CHUNK_CAMERA,        "Camera",                           CameraReader},
  {CHUNK_HIERARCHY,     "Hierarchy",                        NULL},

  {CHUNK_VIEWPORT,      "Viewport_info",                    SkipReader},

  {CHUNK_MATERIAL,      "Material",                         NULL},
  {CHUNK_MATNAME,       "Material_name",                    MatNameReader},
  {CHUNK_AMBIENT,       "Ambient_color",                    NULL},
  {CHUNK_DIFFUSE,       "Diffuse_color",                    NULL},
  {CHUNK_SPECULAR,      "Specular_color",                   NULL},
  {CHUNK_SHININESS,     "Shininess",                        NULL},
  {CHUNK_SHSTRENGTH,    "Shininess_strength",               NULL},
  {CHUNK_TRANSP,        "Transparency",                     NULL},
  {CHUNK_TRFALLOFF,     "Transparency_falloff",             NULL},
  {CHUNK_REFLECTBL,     "Reflection_blur",                  NULL},
  {CHUNK_SELFILLUM,     "Self Illumination",                NULL},
  {CHUNK_FLAG2SIDE,     "2Side:_ON_/flag/",                 SkipReader},
  {CHUNK_FLAGADD,       "Transparency:_Add_/flag/",         SkipReader},
  {CHUNK_WIREFRAME,     "WireFrame:_ON_/flag/",             SkipReader},
  {CHUNK_WIRETHICK,     "WireFrame_thickness",              FloatReader},
  {CHUNK_FLAGIN,        "Transparency_falloff:_In_/flag/",  SkipReader},
  {CHUNK_FLAGUNIT,      "WireFrame thickness:_Unit_/flag/", SkipReader},
  {CHUNK_FACEMAP,       "Self Illumination:_FaceMap_/flag/",SkipReader},
  {CHUNK_SOFTEN,        "Shininess:_Soften_/flag/",         SkipReader},
  {CHUNK_SHADETYPE,     "Shading_type",                     SHTReader},
  {CHUNK_TEXTURE,       "Texture_map",                      NULL},
  {CHUNK_SPECULARMAP,   "Specular_map",                     NULL},
  {CHUNK_OPACITYMAP,    "Opacity_map",                      NULL},
  {CHUNK_TEXTURE2,      "Texture_map_2",                    NULL},
  {CHUNK_SHININESSMAP,  "Shininess_map",                    NULL},
  {CHUNK_SELFILLUMMAP,  "Self_Illumination_map",            NULL},
  {CHUNK_REFMAP,        "Reflection_map",                   NULL},
  {CHUNK_BUMPMAP,       "Bump_map",                         NULL},
  {CHUNK_TEXTUREMASK,   "Texture_mask",                     NULL},
  {CHUNK_SPECULARMASK,  "Specular_mask",                    NULL},
  {CHUNK_OPACITYMASK,   "Opacity_mask",                     NULL},
  {CHUNK_TEXTUREMASK2,  "Texture2_mask",                    NULL},
  {CHUNK_SHININESSMASK, "Shininess_mask",                   NULL},
  {CHUNK_SELFILLUMMASK, "Self_Illumination mask",           NULL},
  {CHUNK_REFMASK,       "Reflection_mask",                  NULL},
  {CHUNK_BUMPMASK,      "Bump_mask",                        NULL},
  {CHUNK_TFLAGS,        "Texture_flags",                    TextFlagReader},
  {CHUNK_BLUR,          "Texture_Blur",                     FloatReader},
  {CHUNK_USCALE,        "U_scale_(1/x)",                    FloatReader},
  {CHUNK_VSCALE,        "V_scale_(1/x)",                    FloatReader},
  {CHUNK_UOFFSET,       "U_offset",                         FloatReader},
  {CHUNK_VOFFSET,       "V_offset",                         FloatReader},
  {CHUNK_TROTATION,     "Texture_Rotation_angle",           FloatReader},
  {CHUNK_BLACKTINT,     "Black_Tint",                       RGBBReader},
  {CHUNK_WHITETINT,     "White_Tint",                       RGBBReader},
  {CHUNK_RTINT,         "Red_Tint",                         RGBBReader},
  {CHUNK_GTINT,         "Green_Tint",                       RGBBReader},
  {CHUNK_BTINT,         "Blue_Tint",                        RGBBReader},
  {CHUNK_MAPFILE,       "Map_filename",                     MapFileReader},

  {CHUNK_KEYFRAMER,     "Keyframer_data",                   NULL},
  {CHUNK_AMBIENTKEY,    "Ambient_key",                      NULL},
  {CHUNK_TRACKINFO,     "Track_info",                       NULL},
  {CHUNK_FRAMES,        "Frames",                           FramesReader},
  {CHUNK_TRACKOBJNAME,  "Track_Obj_Name",                   TrackObjNameReader},
  {CHUNK_TRACKPIVOT,    "Pivot_point",                      PivotPointReader},
  {CHUNK_TRACKPOS,      "Position_keys",                    TrackPosReader},
  {CHUNK_TRACKROTATE,   "Rotation_keys",                    TrackRotReader},
  {CHUNK_TRACKSCALE,    "Scale_keys",                       TrackScaleReader},
  {CHUNK_TRACKMORPH,    "Morph_keys",                       TrackMorphReader},
  {CHUNK_TRACKHIDE,     "Hide_keys",                        TrackHideReader},
  {CHUNK_OBJNUMBER,     "Object_number",                    ObjNumberReader},

  {CHUNK_TRACKCAMERA,   "Camera_track",                     NULL},
  {CHUNK_TRACKCAMTGT,   "Camera_target_track",              NULL},
  {CHUNK_TRACKLIGHT,    "Pointlight_track",                 NULL},
  {CHUNK_TRACKLIGTGT,   "Pointlight_target track",          NULL},
  {CHUNK_TRACKSPOTL,    "Spotlight_track",                  NULL},
  {CHUNK_TRACKFOV,      "FOV_track",                        TrackFovReader},
  {CHUNK_TRACKROLL,     "Roll_track",                       TrackRollReader},
 };

int FindChunk(word id)
 {
  int i;
  for (i = 0; i < sizeof(ChunkNames)/sizeof(ChunkNames[0]); i++)
  if (id == ChunkNames[i].id) return i;
  return -1;
 }

// ------------------------------------

 int Verbose = 0;
 int Quiet   = 0;
 int Dump    = 0;

void ChunkReader(FILE *f, int ind, long p)
 {
  TChunkHeader h;
  int n, i;
  byte d;
  long pc;

  while (ftell(f) < p)
  {
   pc = ftell(f);
   if (fread(&h, sizeof(h), 1, f) != 1) return;
   if (h.len == 0) return;
   n = FindChunk(h.id);
   if (n < 0)
   {
    printf("0x%04X (unknown)\n",h.id);
    fseek(f, pc + h.len, SEEK_SET);
   }
   else
   {
    if (!Quiet || ChunkNames[n].func == NULL)
    printf("0x%04X %s\n",h.id,ChunkNames[n].name);
    pc = pc + h.len;
    if (ChunkNames[n].func != NULL) ChunkNames[n].func(f, ind + 2, pc);
    else ChunkReader(f, ind + 2, pc);
    fseek(f, pc, SEEK_SET);
   }
   if (ferror(f)) break;
  }
 }

// ------------------------------------

void main(int argc, char *argv[])
 {
  FILE *f;
  long p;

  if (argc < 2)
  {
   printf("Usage: 3DSRDR file.3DS (or .PRJ or .MLI) [-quiet, -verbose or -dump]\n");
   exit(1);
  }
  f = fopen(argv[1], "rb");
  if (f == NULL)
  {
   printf("Can't open %s!\n", argv[1]);
   exit(1);
  }

  if (argc > 2 && strcmp(argv[2], "-quiet") == 0)   Quiet = 1;
  if (argc > 2 && strcmp(argv[2], "-verbose") == 0) Verbose = 1;
  if (argc > 2 && strcmp(argv[2], "-dump") == 0)
  {
   Verbose = 1;
   Dump = 1;
  }

	// Find file size.
  fseek(f, 0, SEEK_END);
  p = ftell(f);
  fseek(f, 0, SEEK_SET);
	// Go!
  ChunkReader(f, 0, p);
 }

