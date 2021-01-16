/* --------------------------- 3DSRDR.C -------------------------------

    .3DS file format exerciser v1.4
    by Bezzegh D‚nes, aka. DiVeR
    e-mail: diver@inf.bme.hu
    http://www.inf.bme.hu/~diver

    If you are not interested in materials,
    then there is not much new for you. :-(

    Changes since v1.3:

    - Fixed SmooListReader's bug when group>15.
    - New data-chunk type: WORD /CHUNK_WRD/
    - New Material chunks:
	CHUNK_REFMAP	      CHUNK_REFMASK	     CHUNK_SHININESS
	CHUNK_SPECULARMAP     CHUNK_SPECULARMASK     CHUNK_SHSTRENGTH
	CHUNK_OPACITYMAP      CHUNK_OPACITYMASK      CHUNK_TRANSP
	CHUNK_TEXTURE2	      CHUNK_TEXTURE2MASK     CHUNK_TRFALLOFF
	CHUNK_SHININESSMAP    CHUNK_SHININESSMASK    CHUNK_REFLECTBL
	CHUNK_SELFILLUMMAP    CHUNK_SELFILLUMMASK    CHUNK_SELFILLUM
	CHUNK_TEXTUREMASK     CHUNK_BLUR	     CHUNK_SHADETYPE
	CHUNK_BUMPMASK	      CHUNK_TROTATION	     CHUNK_FLAG2SIDE
	CHUNK_USCALE	      CHUNK_VSCALE	     CHUNK_TFLAGS
	CHUNK_UOFFSET	      CHUNK_VOFFSET
	CHUNK_BLACKTINT       CHUNK_WHITETINT
	CHUNK_RTINT	      CHUNK_GTINT	     CHUNK_BTINT
    - New "not so useful" Material chunks:
	CHUNK_FACEMAP	      CHUNK_WIREFRAME	     CHUNK_WIRETHICK
	CHUNK_SOFTEN	      CHUNK_FLAGIN	     CHUNK_FLAGUNIT
	CHUNK_FLAGADD
    - New Light chunks:
	CHUNK_LIGHTR1	      CHUNK_LIGHTR2	     CHUNK_LIGHTMUL
    - Added CHUNK_SHP and CHUNK_LFT. Why not?
    - That's all!

    Did I leave something from material out?

   ------------------------------ v1.3 --------------------------------

    .3DS file format exerciser v1.3.
    by Mats Byggmastar, aka. MRI/Doomsday
    e-mail: mri@penti.sit.fi

    All credits goes to Jare/Iguana as this is just a modified
    version of v1.2 he released.

    You can use the following method to dig up new data:
	- Make a simple .3ds file and convert it with this reader
	  using -dump mode.
	- Go back into 3D Studio and add that little feature to the
	  scene and convert the .3ds file again using -dump mode.
	- Compare the output from both conversions and you'll
	  find the chunk where the data was modified or added.

    "The Unofficial 3dStudio 3DS File Format v1.0" by Jeff Lewis
    is also a great help. I found that file at:

	  www.viewpoint.com/avalon/format_specs.html


    .Mats Byggmastar  15.2.1997  Jakobstad, Finland


   --------------------------- 3DSRDR.C -------------------------------

    .3DS file format exerciser v1.2.
    Written by Javier Arevalo, AKA Jare/Iguana.

    I compile this with Watcom/32, but I guess it should work with
	any compiler and OS combination for which the typedefs are
	valid i.e. any that I know for PCs... Try it and see.
	Oh, and also check the #pragma pack() thing.

    - DISCLAIMER -

    I hope I have not broken any patents or trade secrets by releasing
	this info. This is purely a mind exercise to break into a file
	format that is quite useful to know. As far as I have been told
	a file format is not subject to anything such as copyright or
	patent, so I have done this because I believe I'm allowed to.

    I PLACE THIS FILE IN THE PUBLIC DOMAIN, SO EVERYTHING CONTAINED HERE
	IS TOTALLY FREE FOR YOU TO EXPLORE AND USE. I DISCLAIM ANY AND ALL
	EVENTS COMING OUT OF ANY POSSIBLE USE (OR LACK OF USE) OR EXISTANCE
	OF THIS FILE. I WON'T BE LIABLE FOR ANYTHING RELATED TO THIS FILE,
	OR ANY PRIOR OR FUTURE VERSION OF IT.

    All trademarks mentioned are property of their respective holders.

    - Merits -

    Heavily based on info on the file 3DS_08.TXT by Jim Pitts
      (jp5@ukc.ac.uk)

    Basic material-related stuff digged up by Jare.
    Track info stuff too.

    Thanks to the Egerter brothers of WGT fame and to Walken/Impact studios
	for extra info, Rex Deathstar for support. And definitely to
	Xanthome/Darkzone for you know why. And of course, respect to
	Avatar/Legend Design for being here before all of us.

    For a cool example of actual reading of 3DS files, look no
	further than 3DSCO20.ZIP by Mats Byggmastar aka. MRI. I
	personally prefer using a table-driven modification of this
	code, but both approaches are quite ok and his is much faster
	to write and follow.

    Now only lack is someone to explain how to make use of all this
	stuff i.e. how exactly is data stored, how spline interpolations
	are performed, what are those things called quaternions, etc. And
	also, maybe, dig the rest of the chunks until we are actually able
	to write 3DS files instead of just being bored reading. There's
	lots to do.

    If you decide to work on this further, please make your findings
	public like we have already done, ok? Upload it to
	x2ftp.oulu.fi, THE place for programming info, and/or to
	ftp.cdrom.com. But please PUBLISH it!

    - Change log -

    V 1.2:
	- Added change log to have some idea what's going on.
	- Added pivot point reading inside tracks stuff.
	- Info about spline flags on keyframes.
	- Added face edge visibility info.
	- Finally!! Those flags that mark when the texture is wrapping
	  around inside a face. This happens when you apply spherical
	  or cylindrical coordinates, the faces along the 0§ axis don't
	  get proper mapping coords. Someone describe how to fix this?
	- Added -quiet parm, only displays minimal chunk info.
	- Object parent number is stored in CHUNK_TRACKOBJNAME.
	  This makes reference to the node number in CHUNK_OBJNUMBER.
	- Object number changed to unsigned. Parent 65535 means none.
	- Added CHUNK_PRJ and CHUNK_MLI to allow inspecting .PRJ and
	  .MLI files (they're basically the same chunks as .3DS).
	- Added banner to identify myself, and disclaimer for "just in
	  case" possibilities.
	- Corrected possible bug when chunklen == 0 (it was not a
	  chunk).
	- Added several name descriptions of chunks. Use diff to find
	  all the new chunks.
*/

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

typedef struct {
    word    id;
    dword   len;
} TChunkHeader, *PChunkHeader;

#pragma pack()

enum {
    CHUNK_RGBF	    = 0x0010,
    CHUNK_RGBB	    = 0x0011,
//    CHUNK_RBGB2     = 0x0012,       // ?? NOT HLS.
    CHUNK_WRD	    = 0x0030,

    CHUNK_PRJ	    = 0xC23D,
    CHUNK_MLI	    = 0x3DAA,
    CHUNK_SHP	    = 0x2D2D,
    CHUNK_LFT	    = 0x2D3D,

    CHUNK_MAIN	    = 0x4D4D,
	CHUNK_OBJMESH	= 0x3D3D,
	    CHUNK_BKGCOLOR  = 0x1200,
	    CHUNK_AMBCOLOR  = 0x2100,
	    CHUNK_OBJBLOCK  = 0x4000,
		CHUNK_TRIMESH	= 0x4100,
		    CHUNK_VERTLIST  = 0x4110,
		    CHUNK_VERTFLAGS = 0x4111,
		    CHUNK_FACELIST  = 0x4120,
		       CHUNK_FACEMAT   = 0x4130,
		    CHUNK_MAPLIST   = 0x4140,
		    CHUNK_SMOOLIST  = 0x4150,
		    CHUNK_TRMATRIX  = 0x4160,
		    CHUNK_MESHCOLOR = 0x4165,
		    CHUNK_TXTINFO   = 0x4170,
		CHUNK_LIGHT	= 0x4600,
		    CHUNK_LIGHTR1  = 0x4659,
		    CHUNK_LIGHTR2  = 0x465A,
		    CHUNK_LIGHTMUL = 0x465B,
		    CHUNK_SPOTLIGHT = 0x4610,
		CHUNK_CAMERA	= 0x4700,
		CHUNK_HIERARCHY = 0x4F00,
	CHUNK_VIEWPORT = 0x7001,
	CHUNK_MATERIAL	       = 0xAFFF,
	    CHUNK_MATNAME      = 0xA000,
	    CHUNK_AMBIENT      = 0xA010,
	    CHUNK_DIFFUSE      = 0xA020,
	    CHUNK_SPECULAR     = 0xA030,
	    CHUNK_SHININESS    = 0xA040,
	    CHUNK_SHSTRENGTH   = 0xA041,
	    CHUNK_TRANSP       = 0xA050,
	    CHUNK_TRFALLOFF    = 0xA052,
	    CHUNK_REFLECTBL    = 0xA053,
	    CHUNK_FLAG2SIDE    = 0xA081,
	    CHUNK_FLAGADD      = 0xA083,
	    CHUNK_SELFILLUM    = 0xA084,
	    CHUNK_WIREFRAME    = 0xA085, // Flag
	    CHUNK_WIRETHICK    = 0xA087, // Float
	    CHUNK_FACEMAP      = 0xA088,   // Flag
	    CHUNK_FLAGIN       = 0xA08A,
	    CHUNK_SOFTEN       = 0xA08C,    // Flag
	    CHUNK_FLAGUNIT     = 0xA08E,  // Very useless
	    CHUNK_SHADETYPE    = 0xA100,
	    CHUNK_TEXTURE      = 0xA200,
	    CHUNK_TEXTUREMASK  = 0xA33E,
	    CHUNK_TEXTURE2     = 0xA33A,
	    CHUNK_TEXTUREMASK2 = 0xA340,
	    CHUNK_REFMAP       = 0xA220,
	    CHUNK_REFMASK      = 0xA34C,
	    CHUNK_OPACITYMAP   = 0xA210,
	    CHUNK_OPACITYMASK  = 0xA342,
	    CHUNK_SPECULARMAP  = 0xA204,
	    CHUNK_SPECULARMASK = 0xA348,
	    CHUNK_SHININESSMAP = 0xA33C,
	    CHUNK_SHININESSMASK= 0xA346,
	    CHUNK_SELFILLUMMAP = 0xA33D,
	    CHUNK_SELFILLUMMASK= 0xA34A,
	    CHUNK_BUMPMAP      = 0xA230,
	    CHUNK_BUMPMASK     = 0xA344,
	    CHUNK_MAPFILE      = 0xA300,
	      CHUNK_TFLAGS     = 0xA351, // Flags
	      CHUNK_BLUR       = 0xA353, // Float
	      CHUNK_USCALE     = 0xA354, // Float
	      CHUNK_VSCALE     = 0xA356, // Float
	      CHUNK_UOFFSET    = 0xA358, // Float
	      CHUNK_VOFFSET    = 0xA35A, // Float
	      CHUNK_TROTATION  = 0xA35C, // Float
	      CHUNK_BLACKTINT  = 0xA360, // Float
	      CHUNK_WHITETINT  = 0xA362, // Float
	      CHUNK_RTINT      = 0xA364, // Float
	      CHUNK_GTINT      = 0xA366, // Float
	      CHUNK_BTINT      = 0xA368, // Float
	CHUNK_KEYFRAMER = 0xB000,
	    CHUNK_AMBIENTKEY	= 0xB001,
	    CHUNK_TRACKINFO = 0xB002,
		CHUNK_TRACKOBJNAME  = 0xB010,
		CHUNK_TRACKPIVOT    = 0xB013,
		CHUNK_TRACKPOS	    = 0xB020,
		CHUNK_TRACKROTATE   = 0xB021,
		CHUNK_TRACKSCALE    = 0xB022,
		CHUNK_TRACKMORPH    = 0xB026,
		CHUNK_TRACKHIDE     = 0xB029,
		CHUNK_OBJNUMBER     = 0xB030,
	    CHUNK_TRACKCAMERA = 0xB003,
		CHUNK_TRACKFOV	= 0xB023,
		CHUNK_TRACKROLL = 0xB024,
	    CHUNK_TRACKCAMTGT = 0xB004,
	    CHUNK_TRACKLIGHT  = 0xB005,
	    CHUNK_TRACKLIGTGT = 0xB006,
	    CHUNK_TRACKSPOTL  = 0xB007,
	    CHUNK_FRAMES    = 0xB008,


};

// ------------------------------------

    // Forward declaration.
void ChunkReader (FILE *f, int ind, long p);

void SkipReader (FILE *f, int ind, long p) {
}

void RGBFReader (FILE *f, int ind, long p) {
    float c[3];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    Red: %f, Green: %f, Blue: %f\n", ind, "", c[0], c[1], c[2]);
}

void RGBBReader (FILE *f, int ind, long p) {
    byte c[3];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    Red: %d, Green: %d, Blue: %d\n", ind, "", c[0], c[1], c[2]);
}

void WRDReader (FILE *f, int ind, long p) {
    word value;
    if (fread(&value, sizeof(value), 1, f) != 1) return;
    printf("%*s    Value: %d\n", ind, "", value);
}

void FloatReader (FILE *f, int ind, long p) {
    float value;
    if (fread(&value, sizeof(value), 1, f) != 1) return;
    printf("%*s    Value: %f\n", ind, "", value);
}

void TextFlagReader (FILE *f, int ind, long p) {
    int i=0;
    byte flag1, flag2;

    printf("%*s  ", ind, "");
    if (fread(&flag1, sizeof(flag1), 1, f) != 1) return;
    if (fread(&flag2, sizeof(flag2), 1, f) != 1) return;
    i++;
    printf("Source:");
    if((flag1 & 0xc0)==0x80) printf("RGB luma tint\n");
    else{
      if((flag1 & 0xc0)==0xc0) printf("Alpha tint\n");
      else{
	if((flag2 & 0x02)==0x02) printf("RGB tint\n");
	else printf("RGB\n");
      }
    }
    printf("%*s  ", ind, "");
    printf("Filtering:");
    if (flag1 & 0x40) printf("Summed arial\n");
    else printf("Pyramidal\n");
    printf("%*s  ", ind, "");
    printf("Other parameters:");
    switch((flag1 & 0x11)){
      case 0x11 : printf("Decal"); break;
      case 0x00 : printf("Tile"); break;
      case 0x10 :
      case 0x01 : printf("Tile & Decal");
    }
    if(flag2 & 1) printf(" - Ignoge map alpha");
    if(flag1 & 8) printf(" - Negative");
    if(flag1 & 2) printf(" - Mirror");
    printf("\n%*s  ", ind, "");
    printf("%02X ", flag1); printf("%02X ", flag2);
    printf("\n");
}

void SHTReader (FILE *f, int ind, long p) {
    word value;
    if (fread(&value, sizeof(value), 1, f) != 1) return;
    switch(value){
      case 1: printf("%*s    Flat\n", ind, ""); break;
      case 2: printf("%*s    Gouraud\n", ind, ""); break;
      case 3: printf("%*s    Phong\n", ind, ""); break;
      case 4: printf("%*s    Metal\n", ind, ""); break;
    }
}

void ASCIIZReader (FILE *f, int ind, long p) {
    int c;

	// Read ASCIIZ name
    while ( (c = fgetc(f)) != EOF && c != '\0')
	putchar(c);
    printf("\"\n");
}

void ObjBlockReader (FILE *f, int ind, long p) {
    int c;

	// Read ASCIIZ object name
    printf("%*sObject name \"", ind, "");
    ASCIIZReader(f, ind, p);
	// Read rest of chunks inside this one.
    ChunkReader(f, ind, p);
}

void VertListReader (FILE *f, int ind, long p) {
    word nv;
    float c[3];

    if (fread(&nv, sizeof(nv), 1, f) != 1) return;
    printf("%*sVertices: %d\n", ind, "", nv);
    while (nv-- > 0) {
	if (fread(&c, sizeof(c), 1, f) != 1) return;
	printf("%*s    X: %f, Y: %f, Z: %f\n", ind, "", c[0], c[1], c[2]);
    }
}

void FaceListReader (FILE *f, int ind, long p) {
    word nv;
    word c[3];
    word flags;

    if (fread(&nv, sizeof(nv), 1, f) != 1) return;
    printf("%*sFaces: %d\n", ind, "", nv);
    while (nv-- > 0) {
	if (fread(&c, sizeof(c), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  A %d, B %d, C %d, 0x%X:",
	       ind, "", c[0], c[1], c[2], flags);
//	  printf("%*s	 AB: %d, BC: %d, CA: %d, UWrap %d, VWrap %d\n",
//		 ind, "",
	printf(" AB %d BC %d CA %d UWrap %d VWrap %d\n",
	       (flags & 0x04) != 0, (flags & 0x02) != 0, (flags & 0x01) != 0,
	       (flags & 0x08) != 0, (flags & 0x10) != 0);
    }
	// Read rest of chunks inside this one.
    ChunkReader(f, ind, p);
}

void FaceMatReader (FILE *f, int ind, long p) {
    int c;
    word n, nf;

	// Read ASCIIZ material name
    printf("%*sMaterial name for faces: \"", ind, "");
    ASCIIZReader(f, ind, p);

    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sFaces with this material: %d\n", ind, "", n);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	printf("%*s    Face %d\n",
	       ind, "", nf);
    }
}

void MapListReader (FILE *f, int ind, long p) {
    word nv;
    float c[2];

    if (fread(&nv, sizeof(nv), 1, f) != 1) return;
    printf("%*sVertices: %d\n", ind, "", nv);
    while (nv-- > 0) {
	if (fread(&c, sizeof(c), 1, f) != 1) return;
	printf("%*s    U: %f, V: %f\n", ind, "", c[0], c[1]);
    }
}

void SmooListReader (FILE *f, int ind, long p) {
    dword s;
    int i;

    while (ftell(f) < p) {
	if (fread(&s, sizeof(s), 1, f) != 1) return;
	printf("%*sSmoothing groups: ", ind, "");
	for (i = 0; i < 32; i++)
	    if (s & ((dword)1 << i))
		printf("%d, ", i + 1);
	printf("\n");
    }
}

void TrMatrixReader(FILE *f, int ind, long p) {
    float rot[9];
    float trans[3];
    if (fread(&rot, sizeof(rot), 1, f) != 1) return;
    printf("%*sRotation matrix:\n", ind, "");
    printf("%*s    %f, %f, %f\n", ind, "", rot[0], rot[1], rot[2]);
    printf("%*s    %f, %f, %f\n", ind, "", rot[3], rot[4], rot[5]);
    printf("%*s    %f, %f, %f\n", ind, "", rot[6], rot[7], rot[8]);
    if (fread(&trans, sizeof(trans), 1, f) != 1) return;
    printf("%*sTranslation matrix: %f, %f, %f\n",
	   ind, "", trans[0], trans[1], trans[2]);
}

void LightReader(FILE *f, int ind, long p) {
    float c[3];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    X: %f, Y: %f, Z: %f\n", ind, "", c[0], c[1], c[2]);
	// Read rest of chunks inside this one.
    ChunkReader(f, ind, p);
}

void SpotLightReader(FILE *f, int ind, long p) {
    float c[5];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    Target X: %f, Y: %f, Z: %f; Hotspot %f, Falloff %f\n",
	   ind, "", c[0], c[1], c[2], c[3], c[4]);
}

void CameraReader(FILE *f, int ind, long p) {
    float c[8];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    Position: X: %f, Y: %f, Z: %f\n", ind, "", c[0], c[1], c[2]);
    printf("%*s    Target: X: %f, Y: %f, Z: %f\n", ind, "", c[3], c[4], c[5]);
    printf("%*s    Bank: %f, Lens: %f\n", ind, "", c[6], c[7]);
}

void MatNameReader (FILE *f, int ind, long p) {
    int c;

	// Read ASCIIZ object name
    printf("%*sMaterial name \"", ind, "");
    ASCIIZReader(f, ind, p);
}

void MapFileReader(FILE *f, int ind, long p) {
    int c;

	// Read ASCIIZ filename
    printf("%*sMap filename \"", ind, "");
    ASCIIZReader(f, ind, p);
}

void FramesReader(FILE *f, int ind, long p) {
    dword c[2];
    if (fread(&c, sizeof(c), 1, f) != 1) return;
    printf("%*s    Start: %ld, End: %ld\n",
	   ind, "", c[0], c[1]);
}

void TrackObjNameReader(FILE *f, int ind, long p) {
    int c;
    word w[2];
    word parent;

	// Read ASCIIZ name
    printf("%*sTrack object name \"", ind, "");
    ASCIIZReader(f, ind, p);
    if (fread(&w, sizeof(w), 1, f) != 1) return;
    if (fread(&parent, sizeof(parent), 1, f) != 1) return;
    printf("%*sObject name data: Flags 0x%X, 0x%X, Parent %d\n",
	   ind, "", w[0], w[1], parent);
}

void PivotPointReader(FILE *f, int ind, long p) {
    float pos[3];

    if (fread(&pos, sizeof(pos), 1, f) != 1) return;
    printf("%*s  Pivot at X: %f, Y: %f, Z: %f\n",
	   ind, "",
	   pos[0], pos[1], pos[2]);
}

    /* Key info flags for position, rotation and scaling:
	Until I know the meaning of each bit in flags I assume all mean
	a following float data.
    */

	// NOTE THIS IS NOT A CHUNK, but A PART OF SEVERAL CHUNKS
void SplineFlagsReader(FILE *f, int ind, word flags) {
    int i;
    float dat;

    for (i = 0; i < 16; i++) {
	static const char *flagnames[] = {
	    "Tension",
	    "Continuity",
	    "Bias",
	    "Ease To",
	    "Ease From",
	};
	if (flags & (1 << i)) {
	    if (fread(&dat, sizeof(dat), 1, f) != 1) return;
	    if (i < sizeof(flagnames)/sizeof(*flagnames))
		printf("%*s		%-15s = %f\n",
		       ind, "", flagnames[i], dat);
	    else
		printf("%*s		%-15s = %f\n",
		       ind, "", "Unknown", dat);
	}
    }
}

void TrackPosReader(FILE *f, int ind, long p) {
    word n, nf;
    float pos[3];
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sPosition keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	if (fread(&pos, sizeof(pos), 1, f) != 1) return;
	printf("%*s		X: %f, Y: %f, Z: %f\n",
	       ind, "", pos[0], pos[1], pos[2]);
    }
}

void TrackRotReader(FILE *f, int ind, long p) {
    word n, nf;
    float pos[4];
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sRotation keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	if (fread(&pos, sizeof(pos), 1, f) != 1) return;
	printf("%*s		Angle: %f§, X: %f, Y: %f, Z: %f\n",
	       ind, "", pos[0]*180.0/PI, pos[1], pos[2], pos[3]);
    }
}

void TrackScaleReader(FILE *f, int ind, long p) {
    word n, nf;
    float pos[3];
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sScale keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	if (fread(&pos, sizeof(pos), 1, f) != 1) return;
	printf("%*s	       X: %f, Y: %f, Z: %f\n",
	       ind, "", pos[0], pos[1], pos[2]);
    }
}

void TrackFovReader(FILE *f, int ind, long p) {
    word n, nf;
    float fov;
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sFOV keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	if (fread(&fov, sizeof(fov), 1, f) != 1) return;
	printf("%*s	       FOV: %f\n",
	       ind, "", fov);
    }
}

void TrackRollReader(FILE *f, int ind, long p) {
    word n, nf;
    float roll;
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sRoll keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	if (fread(&roll, sizeof(roll), 1, f) != 1) return;
	printf("%*s	       Roll: %f\n",
	       ind, "", roll);
    }
}

void TrackMorphReader(FILE *f, int ind, long p) {
    word n, nf;
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sMorph keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    while (n-- > 0) {
	if (fread(&nf, sizeof(nf), 1, f) != 1) return;
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	if (fread(&flags, sizeof(flags), 1, f) != 1) return;
	printf("%*s  Frame %3d: Flags 0x%X\n", ind, "", nf, flags);
	SplineFlagsReader(f, ind, flags);
	printf("%*s		Object name: \"", ind, "");
	ASCIIZReader(f, ind, p);
    }
}

void TrackHideReader(FILE *f, int ind, long p) {
    word n, nf;
    word frame;
    word unkown;
    word flags;
    int i;

    for(i=0; i<5; i++) {
	if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	printf("%*sUnknown #%d: 0x%x\n", ind, "", i, unkown);
    }
    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sHide keys: %d\n", ind, "", n);
    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
    printf("%*sUnknown: 0x%x\n", ind, "", unkown);
    ind += 2;
    while (n-- > 0) {
	if (fread(&frame, sizeof(frame), 1, f) != 1) return;
	printf("%*sFrame: %d\n", ind, "", (dword) frame);
	for(i=0; i<2; i++) {
	    if (fread(&unkown, sizeof(unkown), 1, f) != 1) return;
	    printf("%*s  Unknown #%d: 0x%x\n", ind, "", i, unkown);
	}
    }
}



void ObjNumberReader(FILE *f, int ind, long p) {
    word n;

    if (fread(&n, sizeof(n), 1, f) != 1) return;
    printf("%*sObject number: %d\n", ind, "", n);
}


// ------------------------------------

struct {
    word id;
    const char *name;
    void (*func)(FILE *f, int ind, long p);
} ChunkNames[] = {
    {CHUNK_RGBF,	"RGB float",	    RGBFReader},
    {CHUNK_RGBB,	"RGB byte",	    RGBBReader},
    {CHUNK_WRD, 	"WORD", 	    WRDReader},

    {CHUNK_PRJ, 	"Project",	    NULL},
    {CHUNK_MLI, 	"Material Library", NULL},
    {CHUNK_SHP, 	"Shape file", NULL},
    {CHUNK_LFT, 	"Loft file", NULL},

    {CHUNK_MAIN,	"Main", 	    NULL},
    {CHUNK_OBJMESH,	"Object Mesh",	    NULL},
    {CHUNK_BKGCOLOR,	"Background color", NULL},
    {CHUNK_AMBCOLOR,	"Ambient color",    NULL},
    {CHUNK_OBJBLOCK,	"Object Block",     ObjBlockReader},
    {CHUNK_TRIMESH,	"Tri-Mesh",	    NULL},
    {CHUNK_VERTLIST,	"Vertex list",	    VertListReader},
    {CHUNK_VERTFLAGS,	"Vertex flag list", SkipReader},
    {CHUNK_FACELIST,	"Face list",	    FaceListReader},
    {CHUNK_MESHCOLOR,	"Mesh color",	    SkipReader},
    {CHUNK_FACEMAT,	"Face material",    FaceMatReader},
    {CHUNK_MAPLIST,	"Mappings list",    MapListReader},
    {CHUNK_TXTINFO,	"Texture info",     SkipReader},
    {CHUNK_SMOOLIST,	"Smoothings",	    SmooListReader},
    {CHUNK_TRMATRIX,	"Matrix",	    TrMatrixReader},
    {CHUNK_LIGHT,	"Light",	    LightReader},
    {CHUNK_LIGHTR1,	"Light Range radius1",	       FloatReader},
    {CHUNK_LIGHTR2,	"Light Range radius2",	       FloatReader},
    {CHUNK_LIGHTMUL,	"Light Multyplity",	       FloatReader},
    {CHUNK_SPOTLIGHT,	"Spotlight",	    SpotLightReader},
    {CHUNK_CAMERA,	"Camera",	    CameraReader},
    {CHUNK_HIERARCHY,	"Hierarchy",	    NULL},

    {CHUNK_VIEWPORT,	"Viewport info",    SkipReader},

    {CHUNK_MATERIAL,	 "Material",		 NULL},
    {CHUNK_MATNAME,	 "Material name",	 MatNameReader},
    {CHUNK_AMBIENT,	 "Ambient color",	 NULL},
    {CHUNK_DIFFUSE,	 "Diffuse color",	 NULL},
    {CHUNK_SPECULAR,	 "Specular color",	 NULL},
    {CHUNK_SHININESS,	 "Shininess",		 NULL},
    {CHUNK_SHSTRENGTH,	 "Shininess strength",	 NULL},
    {CHUNK_TRANSP,	 "Transparency",	 NULL},
    {CHUNK_TRFALLOFF,	 "Transparency falloff", NULL},
    {CHUNK_REFLECTBL,	 "Reflection blur",	 NULL},
    {CHUNK_SELFILLUM,	 "Self Illumination",	 NULL},
    {CHUNK_FLAG2SIDE,	 "2Side: ON /flag/",	 SkipReader},
    {CHUNK_FLAGADD,	 "Transparency: Add /flag/",		    SkipReader},
    {CHUNK_WIREFRAME,	 "WireFrame: ON /flag/", SkipReader},
    {CHUNK_WIRETHICK,	 "WireFrame thickness",  FloatReader},
    {CHUNK_FLAGIN,	 "Transparency falloff: In /flag/",	    SkipReader},
    {CHUNK_FLAGUNIT,	 "WireFrame thickness: Unit /flag/",	    SkipReader},
    {CHUNK_FACEMAP,	 "Self Illumination: FaceMap /flag/",	    SkipReader},
    {CHUNK_SOFTEN,	 "Shininess: Soften /flag/",	    SkipReader},
    {CHUNK_SHADETYPE,	 "Shadeing type",		    SHTReader},
    {CHUNK_TEXTURE,	 "Texture map", 		    NULL},
    {CHUNK_SPECULARMAP,  "Specular map",		    NULL},
    {CHUNK_OPACITYMAP,	 "Opacity map", 		    NULL},
    {CHUNK_TEXTURE2,	 "Texture map 2",		    NULL},
    {CHUNK_SHININESSMAP, "Shininess map",		    NULL},
    {CHUNK_SELFILLUMMAP, "Self Illumination map",	    NULL},
    {CHUNK_REFMAP,	 "Reflection map",		    NULL},
    {CHUNK_BUMPMAP,	 "Bump map",			    NULL},
    {CHUNK_TEXTUREMASK,  "Texture mask",		    NULL},
    {CHUNK_SPECULARMASK, "Specular mask",		    NULL},
    {CHUNK_OPACITYMASK,  "Opacity mask",		    NULL},
    {CHUNK_TEXTUREMASK2, "Texture2 mask",		    NULL},
    {CHUNK_SHININESSMASK,"Shininess mask",		    NULL},
    {CHUNK_SELFILLUMMASK,"Self Illumination mask",	    NULL},
    {CHUNK_REFMASK,	 "Reflection mask",		    NULL},
    {CHUNK_BUMPMASK,	 "Bump mask",			    NULL},
    {CHUNK_TFLAGS,	 "Texture flags",		    TextFlagReader},
    {CHUNK_BLUR,	 "Texture Blur",		    FloatReader},
    {CHUNK_USCALE,	 "U scale (1/x)",		    FloatReader},
    {CHUNK_VSCALE,	 "V scale (1/x)",		    FloatReader},
    {CHUNK_UOFFSET,	 "U offset",			    FloatReader},
    {CHUNK_VOFFSET,	 "V offset",			    FloatReader},
    {CHUNK_TROTATION,	 "Texture Rotation angle",	    FloatReader},
    {CHUNK_BLACKTINT,	 "Black Tint",			    RGBBReader},
    {CHUNK_WHITETINT,	 "White Tint",			    RGBBReader},
    {CHUNK_RTINT,	 "Red Tint",			    RGBBReader},
    {CHUNK_GTINT,	 "Green Tint",			    RGBBReader},
    {CHUNK_BTINT,	 "Blue Tint",			    RGBBReader},
    {CHUNK_MAPFILE,	 "Map filename",		    MapFileReader},

    {CHUNK_KEYFRAMER,	"Keyframer data",   NULL},
    {CHUNK_AMBIENTKEY,	"Ambient key",	    NULL},
    {CHUNK_TRACKINFO,	"Track info",	    NULL},
    {CHUNK_FRAMES,	"Frames",	    FramesReader},
    {CHUNK_TRACKOBJNAME,"Track Obj. Name",  TrackObjNameReader},
    {CHUNK_TRACKPIVOT,	"Pivot point",	    PivotPointReader},
    {CHUNK_TRACKPOS,	"Position keys",    TrackPosReader},
    {CHUNK_TRACKROTATE, "Rotation keys",    TrackRotReader},
    {CHUNK_TRACKSCALE,	"Scale keys",	    TrackScaleReader},
    {CHUNK_TRACKMORPH,	"Morph keys",	    TrackMorphReader},
    {CHUNK_TRACKHIDE,	"Hide keys",	    TrackHideReader},
    {CHUNK_OBJNUMBER,	"Object number",    ObjNumberReader},

    {CHUNK_TRACKCAMERA, "Camera track", 	    NULL},
    {CHUNK_TRACKCAMTGT, "Camera target track",	    NULL},
    {CHUNK_TRACKLIGHT,	"Pointlight track",	    NULL},
    {CHUNK_TRACKLIGTGT, "Pointlight target track",  NULL},
    {CHUNK_TRACKSPOTL,	"Spotlight track",	    NULL},
    {CHUNK_TRACKFOV,	"FOV track",	    TrackFovReader},
    {CHUNK_TRACKROLL,	"Roll track",	    TrackRollReader},
};

int FindChunk(word id) {
    int i;
    for (i = 0; i < sizeof(ChunkNames)/sizeof(ChunkNames[0]); i++)
	if (id == ChunkNames[i].id)
	    return i;
    return -1;
}

// ------------------------------------

int Verbose = 0;
int Quiet   = 0;
int Dump    = 0;

void ChunkReader(FILE *f, int ind, long p) {
    TChunkHeader h;
    int n, i;
    byte d;
    long pc;

    while (ftell(f) < p) {
	pc = ftell(f);
	if (fread(&h, sizeof(h), 1, f) != 1) return;
	if (h.len == 0) return;
	n = FindChunk(h.id);
	if (n < 0) {
	    if (Verbose) {
		printf("%*sUnknown chunk: 0x%04X, offset 0x%lX, size: %d bytes.",
			ind, "", h.id, pc, h.len);
		if (!Dump)
		    printf("\n");
	    }
	    if (Dump) {
		fseek(f, pc + 6, SEEK_SET);
		for (i=0; i<h.len-6; i++) {
		    if ((i & 0xf) == 0) printf("\n%*s  ", ind, "");
		    if (fread(&d, sizeof(d), 1, f) != 1) return;
		    printf("%02X ", d);
		}
		printf("\n");
	    } else
		fseek(f, pc + h.len, SEEK_SET);
	} else {
	    if (!Quiet || ChunkNames[n].func == NULL)
		printf("%*sChunk type \"%s\", offset 0x%lX, size %d bytes\n",
		       ind, "", ChunkNames[n].name, pc, h.len);
	    pc = pc + h.len;
	    if (ChunkNames[n].func != NULL)
		ChunkNames[n].func(f, ind + 2, pc);
	    else
		ChunkReader(f, ind + 2, pc);
	    fseek(f, pc, SEEK_SET);
	}
	if (ferror(f))
	    break;
    }
}

// ------------------------------------

void Banner(void) {
    printf("3DSRDR, 3D Studio formats reader v1.2 by Javier Arevalo AKA Jare/Iguana.\n"
	   "Modified by Mats Byggmastar aka. MRI/Doomsday (v1.3)\n"
	   "& Modified again by Bezzegh D‚nes aka. DiVeR /diver@inf.bme.hu/ (v1.4)\n");
}

void main(int argc, char *argv[]) {
    FILE *f;
    long p;

    if (argc < 2) {
	Banner();
	printf("Usage: 3DSRDR file.3DS (or .PRJ or .MLI) [-quiet, -verbose or -dump]\n");
	exit(1);
    }

    f = fopen(argv[1], "rb");
    if (f == NULL) {
	Banner();
	printf("Can't open %s!\n", argv[1]);
	exit(1);
    }

    if (argc > 2 && strcmp(argv[2], "-quiet") == 0)
	Quiet = 1;
    if (argc > 2 && strcmp(argv[2], "-verbose") == 0)
	Verbose = 1;
    if (argc > 2 && strcmp(argv[2], "-dump") == 0) {
	Verbose = 1;
	Dump = 1;
    }

    if (!Quiet)
	Banner();

	// Find file size.
    fseek(f, 0, SEEK_END);
    p = ftell(f);
    fseek(f, 0, SEEK_SET);
	// Go!
    ChunkReader(f, 0, p);
}

