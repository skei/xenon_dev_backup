 // RayTracer

//
// --- include --------------------------------------------------------------
//

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>

 #include "vbe20.h"

//
// --- defines --------------------------------------------------------------
//

#define PLANE 2
#define SPHERE 3

 // HANDY VECTOR MACROS
#define Length(A) (float)sqrt(A.x*A.x + A.y*A.y + A.z*A.z)
#define DotProd(A,B) (A.x*B.x + A.y*B.y + A.z*B.z)


//
// --- types ----------------------------------------------------------------
//

typedef struct NormRGBtypeTag
 {
	float Red, Green, Blue;
 } NormRGBtype;

 // ---

typedef struct VectorTypeTag
 {
	float x,y,z;
 } VectorType;

 // ---

typedef struct LightSourceTypeTag
 {
  float x,y,z;        // location of or direction to light
  float Intensity;    // normalized intensity of the light source
  int IsActive;       // light active/inactive flag (1=on, 0=off)
 } LightSourceType;

 // ---

typedef struct PlaneTypeTag
 {
  VectorType Norm;
  float Dist;
 } PlaneType;

 // ---

typedef struct SphereTypeTag
 {
  VectorType Center;
  float Radius;
 } SphereType;

 // ---

typedef struct ObjectTypeTag
 {
  int Type;
  NormRGBtype Color;
  int IsActive;       // object active/inactive flag (1=active, 0=inactive)
  void* Obj;
 } ObjectType;

//
// --- Variables ------------------------------------------------------------
//

  float inv;

  //unsigned short int *Screen;
  unsigned char *Screen;

  VBEINFO *vbeInfo;
  VBESURFACE *VbeScreen;
  int VesaMode;


int SCREENWIDTH;
int SCREENHEIGHT;
int WIDTH;                      // output image dimensions
int HEIGHT;
int VIEWERDISTANCE;
LightSourceType* LightSources;  // the light sources array
int NumLightSources;
ObjectType* Objects;            // the object array
int NumObjects;
float AmbientFactor;            // the ambient light factor
VectorType CamOrigin;           // camera initialization parameters
VectorType ViewRefPt;
VectorType ViewUp;
float DiffuseFactor = 0.45;     // diffuse illum. coefficient
float SpecularFactor = 0.9;     // specular illum. coefficient and exponent
float SpecularExponent = 27.0;

//
// --- Function prototypes --------------------------------------------------
//

 void RayTraceScene(void);
 NormRGBtype TraceRay(VectorType Start, VectorType Dir, float DistToViewPlane);
 float ShadingFactor(VectorType HitPt, VectorType Norm, VectorType ViewDir);
 int NotInShadow(VectorType Start, VectorType Dir, float DistToLight);
 int RayPlaneIntersect(VectorType Start, VectorType Dir, PlaneType* Plane,
                       float* HitDist);
 int RaySphereIntersect(VectorType Start, VectorType Dir, SphereType* Sphere,
                        float* HitDist);
 void ReadInModel(char* FileName);
 void FreeModel();
 void GetCameraVectors(VectorType CamOrigin, VectorType ViewRefPt,
                       VectorType ViewUp,
                       VectorType* CamX, VectorType* CamY, VectorType* CamZ);

//
// ±±± Functions ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 //--------------------------------------------------------------------------
 // MAIN RAYTRACER ROUTINE: Defines the camera coordinate system and the
 // viewplane window. Rays are traced for each pixel through this window and
 // drawn into the framebuffer.
 //---------------------------------------------------------------------------
void RayTraceScene(void)
 {
  VectorType CamX, CamY, CamZ;
  VectorType dCamX,dCamY;
  VectorType RayStart;
  VectorType RayEnd;
  VectorType RayDir;
  int CENTERX,CENTERY;
  int X,Y;
  int Xtrans,Ytrans;
  float dX,dY,dZ;
  float Dist;
  NormRGBtype Color;

  //-------------------------------------------------------------------------
	// GET NORMALIZED CAMERA AXIS VECTORS
	//-------------------------------------------------------------------------
  GetCameraVectors(CamOrigin, ViewRefPt, ViewUp, &CamX, &CamY, &CamZ);

	//-------------------------------------------------------------------------
	// CAST RAYS FOR EACH PIXEL THROUGH THE CAMERA VIEWPLANE WINDOW THAT LIES
	// ON THE CamXCamY PLANE.
	//-------------------------------------------------------------------------

	// FIND THE START OF ALL RAYS BEING TRACED (the EyePoint)
  RayStart.x = VIEWERDISTANCE*CamZ.x + CamOrigin.x;
  RayStart.y = VIEWERDISTANCE*CamZ.y + CamOrigin.y;
  RayStart.z = VIEWERDISTANCE*CamZ.z + CamOrigin.z;

  // TRACE A RAY FOR EACH PIXEL IN THE FRAMEBUFFER
  CENTERX = WIDTH/2;
  CENTERY = HEIGHT/2;
  //for (Y=(HEIGHT-1); Y>=0; Y--)
  for (Y=0;Y<HEIGHT;Y++)
  {
   for (X=0; X<WIDTH; X++)
   {
    // FIND THE TRANSLATIONS ALONG CamY AND CamX TO THE PIXEL POSITION
    Xtrans = (X-CENTERX);
    Ytrans = (Y-CENTERY);
    dCamX.x = Xtrans*CamX.x;
    dCamX.y = Xtrans*CamX.y;
    dCamX.z = Xtrans*CamX.z;
    dCamY.x = Ytrans*CamY.x;
    dCamY.y = Ytrans*CamY.y;
    dCamY.z = Ytrans*CamY.z;

    // FIND THE RAY END (Vector from RayStart to RayEnd defines direction)
    // RayEnd FOUND BY PERFORMING TRANSLATIONS ALONG CamX, CamY, AND THEN
    // WITH RESPECT TO CamOrigin
    RayEnd.x = dCamX.x + dCamY.x + CamOrigin.x;
    RayEnd.y = dCamX.y + dCamY.y + CamOrigin.y;
    RayEnd.z = dCamX.z + dCamY.z + CamOrigin.z;

    // FIND NORMALIZED RAY DIRECTION
    dX = RayEnd.x - RayStart.x;
    dY = RayEnd.y - RayStart.y;
    dZ = RayEnd.z - RayStart.z;
    Dist=(float)sqrt(dX*dX + dY*dY + dZ*dZ);  // DistToViewPlane
    RayDir.x = dX/Dist;
    RayDir.y = dY/Dist;
    RayDir.z = dZ/Dist;

    // TRACE THE RAY
    Color = TraceRay(RayStart,RayDir,Dist);
    //Screen[Y*WIDTH+X] = (   (((unsigned int)(Color.Red  *31))<<10)
    //                      + (((unsigned int)(Color.Green*31))<< 5)
    //                      + (((unsigned int)(Color.Blue *31))<< 0));
    Screen[(Y*SCREENWIDTH*3)+(X*3)  ] = (unsigned char)(Color.Red*255);
    Screen[(Y*SCREENWIDTH*3)+(X*3)+1] = (unsigned char)(Color.Green*255);
    Screen[(Y*SCREENWIDTH*3)+(X*3)+2] = (unsigned char)(Color.Blue*255);
   }
  }
 }

 //--------------------------------------------------------------------------
 // Given the camera origin (CamOrigin), a camera reference point (ViewRefPt,
 // the vector from the CamOrigin to the ViewRefPt defines the direction the
 // camera is facing), and a camera UP vector (ViewUp, defines what is UP in
 // the camera coordinate system, this function returns the X,Y, and Z axes
 // of the camera coordinate system. These axes are the following orthogonal
 // vectors:
 // CamZ (Z-axis defined as the direction from the CamOrigin to the Eye;
 // opposite direction the camera is facing), CamY (Y-axis defined as the
 // cross-product between CamZ and CamX), and CamX (X-axis or "the horizon"
 // defined as the cross-product between ViewUp and CamZ). Each of these
 // vector will be normalized.
 //--------------------------------------------------------------------------
void GetCameraVectors(VectorType CamOrigin, VectorType ViewRefPt,
                      VectorType ViewUp, VectorType* CamX,
                      VectorType* CamY, VectorType* CamZ)
 {
	float Length;

	// CALC NORMALIZED CamZ VECTOR (FROM ViewRefPt TO CamOrigin)
	CamZ->x = CamOrigin.x - ViewRefPt.x;
	CamZ->y = CamOrigin.y - ViewRefPt.y;
	CamZ->z = CamOrigin.z - ViewRefPt.z;
	Length = (float)sqrt(CamZ->x*CamZ->x + CamZ->y*CamZ->y + CamZ->z*CamZ->z);
	CamZ->x /= Length;
	CamZ->y /= Length;
	CamZ->z /= Length;

	// CALC NORMALIZED CamX VECTOR (BASICALLY THE "HORIZON")
	// AS THE CROSS-PRODUCT BETWEEN ViewUp AND CamZ
	CamX->x = ViewUp.y * CamZ->z - ViewUp.z * CamZ->y;
	CamX->y = ViewUp.z * CamZ->x - ViewUp.x * CamZ->z;
	CamX->z = ViewUp.x * CamZ->y - ViewUp.y * CamZ->x;
	Length = (float)sqrt(CamX->x*CamX->x + CamX->y*CamX->y + CamX->z*CamX->z);
	CamX->x /= Length;
	CamX->y /= Length;
	CamX->z /= Length;

	// CALC NORMALIZED CamY VECTOR (CAMERA UP VECTOR)
	// AS THE CROSS-PRODUCT BETWEEN CamZ AND CamX
	CamY->x = CamZ->y * CamX->z - CamZ->z * CamX->y;
	CamY->y = CamZ->z * CamX->x - CamZ->x * CamX->z;
	CamY->z = CamZ->x * CamX->y - CamZ->y * CamX->x;
	Length = (float)sqrt(CamY->x*CamY->x + CamY->y*CamY->y + CamY->z*CamY->z);
	CamY->x /= Length;
	CamY->y /= Length;
	CamY->z /= Length;
 }

 //--------------------------------------------------------------------------
 // Given a Ray (Start Point and Direction) and a Plane, determines if the
 // ray intersects the infinite plane. If it does intersect, then TRUE (1) is
 // returned and the distance to the HitPt is calculated (HitDist); otherwise,
 // the function returns FALSE (0) and no HitDist is calculated.
 //--------------------------------------------------------------------------
int RayPlaneIntersect(VectorType Start, VectorType Dir, PlaneType* Plane,
                      float* HitDist)
 {
  VectorType Norm = Plane->Norm;
	float Denom = Norm.x*Dir.x + Norm.y*Dir.y + Norm.z*Dir.z;
	if (Denom!=0)
  {
   float Numer = -(   Norm.x*Start.x + Norm.y*Start.y + Norm.z*Start.z
                    + Plane->Dist);
   *HitDist = Numer/Denom;
   if (*HitDist > 1) return 1;
	}
	return 0;
 }

 //--------------------------------------------------------------------------
 // Given a Ray (Start Point and Direction) and a Sphere, determines if the
 // ray intersects the Sphere. If it does intersect, then TRUE (1) is
 // returned and the distance to the closest HitPt is calculated (HitDist);
 // otherwise, the function returns FALSE (0) and no HitDist is calculated.
 //--------------------------------------------------------------------------
int RaySphereIntersect(VectorType Start, VectorType Dir, SphereType* Sphere,
                       float* HitDist)
 {
  float b = (float)2.0*(Dir.x*(Start.x-Sphere->Center.x)
                      + Dir.y*(Start.y-Sphere->Center.y)
                      + Dir.z*(Start.z-Sphere->Center.z));
  float c = (Start.x - Sphere->Center.x) * (Start.x - Sphere->Center.x)
            + (Start.y - Sphere->Center.y) * (Start.y - Sphere->Center.y)
            + (Start.z - Sphere->Center.z) * (Start.z - Sphere->Center.z)
            - Sphere->Radius * Sphere->Radius;
	float discriminant = b*b-4*c;
	if (discriminant >= 0)
	{
   float sd=(float)sqrt(discriminant);
   float Dist0 = (-b-sd)/2;
   float Dist1 = (-b+sd)/2;
   if ((Dist0 > 1) || (Dist1 > 1))
   {
    if (Dist0 > 1) *HitDist=Dist0;
    if ((Dist1 < Dist0) && (Dist1 > 1)) *HitDist=Dist1;
    return 1;
   }
	}
	return 0;
 }

 //==========================================================================
 // FREE THE OBJECTS, OBJECT ARRAY, AND LIGHT SOURCE ARRAY
 //==========================================================================
void FreeModel()
 {
  int i;
  free(LightSources);
  for (i=0; i<NumObjects; i++) free(Objects[i].Obj);
	free(Objects);
 }

 //==========================================================================
 // READS IN THE RAY TRACER INITIALIZATIONS AND MODELS
 //==========================================================================
void ReadInModel(char* FileName)
 {
  int LightCount;
  int ObjectCount;
  char c;
  int i;

  // OPEN THE FILE
 	FILE* FilePtr = fopen(FileName, "r");
	if (FilePtr==NULL)
	{
   printf("ERROR: unable to open model file %s!\n", FileName);
   exit(1);
	}
  LightCount=0;
  ObjectCount=0;
	// LOOP THROUGH SCRIPT FILE INITIALIZING THE RAYTRACER APPROPRIATELY
  while ((c=getc(FilePtr)) != EOF)
	{
   // HANDLE THIS CHARACTER APPRIATELY
   switch (c)
   {
    // COMMENT
    case '#': { // IGNORE UNTIL NEWLINE IS REACHED
               for ( ;c!='\n';c=getc(FilePtr));
               break;
              }
    // VIEWPLANE WINDOW PARAMETERS
    case 'V': {
               for (i=0; i<15; i++) c=getc(FilePtr); // ADVANCE PAST "iewPlaneWindow"
               fscanf(FilePtr, "%d %d %d %d %d",
                &WIDTH, &HEIGHT, &VIEWERDISTANCE,&SCREENWIDTH,&SCREENHEIGHT);
               break;
              }
    // CAMERA VIEWING PARAMETERS
    case 'C': {
               for (i=0; i<6; i++) c=getc(FilePtr);  // ADVANCE PAST "amera"
               fscanf(FilePtr, "%f %f %f", &CamOrigin.x, &CamOrigin.y, &CamOrigin.z);
               fscanf(FilePtr, "%f %f %f", &ViewRefPt.x, &ViewRefPt.y, &ViewRefPt.z);
               fscanf(FilePtr, "%f %f %f", &ViewUp.x, &ViewUp.y, &ViewUp.z);
               break;
              }
    // AMBIENT LIGHT FACTOR
    case 'A': {
               for (i=0; i<18; i++) c=getc(FilePtr); // ADVANCE PAST "mbientLightFactor"
               fscanf(FilePtr, "%f", &AmbientFactor);
               break;
              }
    // NUMsomething
    case 'N': {
               for (i=0; i<3; i++) c=getc(FilePtr);  // ADVANCE PAST "um"
               if (c=='L') // get NumLightSources
               {
                for (i=0; i<12; i++) c=getc(FilePtr); // ADVANCE PAST "ightSources"
                fscanf(FilePtr, "%d", &NumLightSources);

                // ALLOCATE LightSources Array
                LightSources = (LightSourceType*)malloc(sizeof(LightSourceType)*NumLightSources);
               }
               else
               if (c=='O') // get NumObjects
               {
                for (i=0; i<7; i++) c=getc(FilePtr);  // ADVANCE PAST "bjects"
                fscanf(FilePtr, "%d", &NumObjects);
                // ALLOCATE LightSources Array
                Objects = (ObjectType*)malloc(sizeof(ObjectType)*NumObjects);
               }
               break;
              }
    // LIGHT SOURCE
    case 'L': {
               // ARE THERE MORE LIGHTS THAN SPECIFIED
               if (LightCount >= NumLightSources)
               {
                printf("ERROR: more light sources given than specified!");
                exit(1);
               }
               for (i=0; i<11; i++) c=getc(FilePtr); // ADVANCE PAST "ightSource"
               // READ IN LIGHT VECTOR (LOCATION OR DIRECTION)
               fscanf(FilePtr, "%f %f %f", &LightSources[LightCount].x,
                                           &LightSources[LightCount].y,
                                           &LightSources[LightCount].z);
               // READ IN NORMALIZED LIGHT INTENSITY and IsActive FLAG
               fscanf(FilePtr, "%f %d %d", &LightSources[LightCount].Intensity,
                                           &LightSources[LightCount].IsActive);
               // INCREMENT LightCount
               LightCount++;
               break;
              }
    // A SPHERE OBJECT
    case 'S': { // SPHERE
               // ARE THERE MORE OBJECTS THAN SPECIFIED
               if (ObjectCount >= NumObjects)
               {
                printf("ERROR: more objects given than specified!");
                exit(1);
               }
               for (i=0; i<6; i++) c=getc(FilePtr);  // ADVANCE PAST "phere"
               // READ IN OBJECT COLOR
               fscanf(FilePtr, "%f %f %f", &Objects[ObjectCount].Color.Red,
                                           &Objects[ObjectCount].Color.Green,
                                           &Objects[ObjectCount].Color.Blue);
               // READ IN IsActive FLAG
               fscanf(FilePtr, "%d", &Objects[ObjectCount].IsActive);
               // SET THE OBJECT TYPE
               Objects[ObjectCount].Type = SPHERE;
               // ALLOCATE SPACE FOR THE OBJECT
               // THEN READ IN APPROPRIATE PARAMETERS
               Objects[ObjectCount].Obj = (void*)malloc(sizeof(SphereType));
               // CENTER (X,Y,Z) AND RADIUS
               fscanf(FilePtr, "%f %f %f %f",&(((SphereType*)Objects[ObjectCount].Obj)->Center.x),
                                             &(((SphereType*)Objects[ObjectCount].Obj)->Center.y),
                                             &(((SphereType*)Objects[ObjectCount].Obj)->Center.z),
                                             &(((SphereType*)Objects[ObjectCount].Obj)->Radius));
               // INCREMENT LightCount
               ObjectCount++;
               break;
              }
    case 'P': { // PLANE
               // ARE THERE MORE OBJECTS THAN SPECIFIED
               if (ObjectCount >= NumObjects)
               {
                printf("ERROR: more objects given than specified!");
                exit(1);
               }
               for (i=0; i<5; i++) c=getc(FilePtr);  // ADVANCE PAST "lane"
               // READ IN OBJECT COLOR
               fscanf(FilePtr, "%f %f %f", &Objects[ObjectCount].Color.Red,
                                           &Objects[ObjectCount].Color.Green,
                                           &Objects[ObjectCount].Color.Blue);
               // READ IN IsActive FLAG
               fscanf(FilePtr, "%d", &Objects[ObjectCount].IsActive);
               // SET THE OBJECT TYPE
               Objects[ObjectCount].Type = PLANE;
               // ALLOCATE SPACE FOR THE OBJECT
               // THEN READ IN APPROPRIATE PARAMETERS
               Objects[ObjectCount].Obj = (void*)malloc(sizeof(PlaneType));
               // CENTER (X,Y,Z) AND RADIUS
               fscanf(FilePtr, "%f %f %f %f",&(((PlaneType*)Objects[ObjectCount].Obj)->Norm.x),
                                             &(((PlaneType*)Objects[ObjectCount].Obj)->Norm.y),
                                             &(((PlaneType*)Objects[ObjectCount].Obj)->Norm.z),
                                             &(((PlaneType*)Objects[ObjectCount].Obj)->Dist));
               // INCREMENT LightCount
               ObjectCount++;
               break;
              }
   }
	}
  fclose(FilePtr);
  if (LightCount!=NumLightSources)
	{
   printf("ERROR: specified more light sources than given!");
   exit(1);
	}
  if (ObjectCount!=NumObjects)
	{
   printf("ERROR: specified more objects than given!");
   exit(1);
	}
 }

 //--------------------------------------------------------------------------
 // Given the HitPt and the Normal to the surface,
 // calculates the shading factor of the HitPt considering the "how much" it
 // faces the light source and whether or not it is in shadow. Since this
 // routine can handle an array of LightSources (multiple sources), each
 // source must be considered in turn and the final intensity is determined
 // by the sum total of each intensity calculated. Also includes the
 // possibility of having no light sources (only ambient light).
 //--------------------------------------------------------------------------
float ShadingFactor(VectorType HitPt, VectorType Norm, VectorType ViewDir)
 {
  int i;
  float FinalIntensity;
  VectorType LightDir;
  float DistToLight;
  float NormDotLight;
  VectorType ReflectedDir;
  float ReflectedDotView;
  float PowerTerm;

  FinalIntensity = (float)0;

  // ADD AMBIENT, DIFFUSE, AND SPECULAR INTENSITY CONTRIBUTIONS TO
  // FinalIntensity FROM EACH LIGHT SOURCE (FINAL SHADING FACTOR IS THE SUM
  // OF ALL OF THESE)
  for (i=0; i < NumLightSources; i++)
  {
   if (LightSources[i].IsActive) // CONSIDER ONLY LIGHT SOURCES THAT ARE "ON"
   {
    // FIND THE NORMALIZED HitPt-TO-LIGHT VECTOR (Light direction vector)
    LightDir.x = LightSources[i].x-HitPt.x;
    LightDir.y = LightSources[i].y-HitPt.y;
    LightDir.z = LightSources[i].z-HitPt.z;
    DistToLight = Length(LightDir);
    if (DistToLight==0) DistToLight=1;  // CHEAT TO PREVENT DIV BY 0
    inv = 1/DistToLight;
    LightDir.x*=inv;// /= DistToLight;        // NORMALIZE LIGHT DIRECTION VECTOR
    LightDir.y*=inv;// /= DistToLight;
    LightDir.z*=inv;// /= DistToLight;

    // CALCULATE DOT PRODUCT BETWEEN NORMAL AND LIGHT DIR VECTOR
    NormDotLight = DotProd(Norm, LightDir);

    // IF DOT PROD IS >= 0, SURFACE IS FACING LIGHT; MUST CHECK IF IN SHADOW.
    // IF DOT PROD IS < 0, SURFACE IS NOT FACING LIGHT AND IS SELF-SHADOWED
    // IF FACING LIGHT AND NOT IN SHADOW, MUST COMPUTE ILLUMINATION OF POINT
    if (NormDotLight >= 0)
    {
     if (NotInShadow(HitPt, LightDir, DistToLight))
     {
      //------------------------------------------------------------
      // ADD IN DIFFUSE REFLECTION COMPONENT (Lambert's Law)
      //------------------------------------------------------------
      FinalIntensity += DiffuseFactor * LightSources[i].Intensity * NormDotLight;

      //------------------------------------------------------------
      // ADD IN SPECULAR REFLECTION COMPONENT
      //------------------------------------------------------------
      ReflectedDir.x = 2*Norm.x*NormDotLight - LightDir.x;
      ReflectedDir.y = 2*Norm.y*NormDotLight - LightDir.y;
      ReflectedDir.z = 2*Norm.z*NormDotLight - LightDir.z;
      ReflectedDotView = DotProd(ReflectedDir, ViewDir);
      if (ReflectedDotView < 0.0) PowerTerm=0.0;
      else PowerTerm = 0.5;//PowerTerm=(float)pow(ReflectedDotView, SpecularExponent);
      FinalIntensity += SpecularFactor * LightSources[i].Intensity * PowerTerm;
     }
    }
   }
  }
  // ADD IN AMBIENT LIGHT COMPONENT
  FinalIntensity += AmbientFactor;
  // CLAMP FINAL INTENSITY VALUE TO 1
  if (FinalIntensity>1.0) FinalIntensity=1.0;
  return (FinalIntensity);
 }

 //--------------------------------------------------------------------------
 // Given a Ray (Start Point and Direction) and a Distance to a light source,
 // determines if any object intersects the ray within the Distance given. If
 // so, then FALSE (0) is returned (Start point is in shadow);
 // otherwise, TRUE (1) is returned (not in shadow).
 //--------------------------------------------------------------------------
int NotInShadow(VectorType Start, VectorType Dir, float DistToLight)
 {
	int i=0;
	int StillTracing=1;
	float HitDist;

	// IF THE RAY INTERSECTS ANY OBJECT, THE POINT IS IN SHADOW
	while ((i < NumObjects) && StillTracing)
  {
   if (Objects[i].IsActive)  // CONSIDER ONLY "ACTIVE" OBJECTS
   {
    switch (Objects[i].Type)
    {
     case PLANE: {
                  if (RayPlaneIntersect(Start, Dir, (PlaneType*)Objects[i].Obj, &HitDist))
                  {
                   if (HitDist <= DistToLight) StillTracing=0;
                  }
                  break;
                 }
     case SPHERE: {
                   if (RaySphereIntersect(Start, Dir, (SphereType*)Objects[i].Obj, &HitDist))
                   {
                    if (HitDist <= DistToLight) StillTracing=0;
                   }
                   break;
                  }
    }
   }
   i++;  // NEXT OBJECT
	}
	return (StillTracing);
 }

 //--------------------------------------------------------------------------
 // Given a Ray (Start Point and Direction), determines the Norm RGB color of
 // the closest intersection of the Ray with all objects in the object list.
 // If the Ray does not intersect an object, then the background color is
 // returned.
 //--------------------------------------------------------------------------
NormRGBtype TraceRay(VectorType Start, VectorType Dir, float DistToViewPlane)
 {
  NormRGBtype Color = {0,0,0};
  float OldDist=999999;
  float HitDist;
  VectorType ViewDir;
  int i;
  VectorType HitPt;
  VectorType Norm;
  float ShadeFactor;

  // FIND THE VIEW DIRECTION VECTOR (FROM HIT POINT TO EyePoint)
  ViewDir.x = -Dir.x;
  ViewDir.x = -Dir.y;
  ViewDir.x = -Dir.z;
  for (i=0; i<NumObjects; i++)
  {
   if (Objects[i].IsActive)  // CONSIDER ONLY "ACTIVE" OBJECTS
   switch (Objects[i].Type)
   {
    case PLANE: {
                 if (RayPlaneIntersect(Start, Dir, (PlaneType*)Objects[i].Obj, &HitDist))
                 {
                  if ( (HitDist < OldDist) && (HitDist >= DistToViewPlane) )
                  {
                   OldDist=HitDist;
                   // FIND THE HIT POINT
                   HitPt.x = Start.x + Dir.x*HitDist;
                   HitPt.y = Start.y + Dir.y*HitDist;
                   HitPt.z = Start.z + Dir.z*HitDist;
                   // FIND THE NORMAL TO THE SURFACE
                   Norm = ((PlaneType*)Objects[i].Obj)->Norm;
                   // CALCULATE NORMALIZED RGB COLOR BY MULTIPLYING BY A SHADING FACTOR
                   ShadeFactor = ShadingFactor(HitPt, Norm, ViewDir);
                   Color.Red = Objects[i].Color.Red * ShadeFactor;
                   Color.Green = Objects[i].Color.Green * ShadeFactor;
                   Color.Blue = Objects[i].Color.Blue * ShadeFactor;
                  }
                 }
                 break;
                }
    case SPHERE: {
                  if (RaySphereIntersect(Start, Dir, (SphereType*)Objects[i].Obj, &HitDist))
                  {
                   if ( (HitDist < OldDist) && (HitDist >= DistToViewPlane) )
                   {
                    OldDist = HitDist;
                    // FIND THE HIT POINT
                    HitPt.x = Start.x + Dir.x*HitDist;
                    HitPt.y = Start.y + Dir.y*HitDist;
                    HitPt.z = Start.z + Dir.z*HitDist;
                    // FIND THE NORMAL TO THE SURFACE
                    inv = 1/((SphereType*)Objects[i].Obj)->Radius;
                    Norm.x = (HitPt.x-((SphereType*)Objects[i].Obj)->Center.x)*inv;// / ((SphereType*)Objects[i].Obj)->Radius;
                    Norm.y = (HitPt.y-((SphereType*)Objects[i].Obj)->Center.y)*inv;// / ((SphereType*)Objects[i].Obj)->Radius;
                    Norm.z = (HitPt.z-((SphereType*)Objects[i].Obj)->Center.z)*inv;// / ((SphereType*)Objects[i].Obj)->Radius;
                    // CALCULATE NORMALIZED RGB COLOR BY MULTIPLYING BY A SHADING FACTOR
                    ShadeFactor = ShadingFactor(HitPt, Norm, ViewDir);
                    Color.Red = Objects[i].Color.Red * ShadeFactor;
                    Color.Green = Objects[i].Color.Green * ShadeFactor;
                    Color.Blue = Objects[i].Color.Blue * ShadeFactor;
                   }
                  }
                  break;
                 }
   }
  }
  return Color;
 }


//
// ±±± MAIN ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void main()
 {
	// READ IN THE RAYTRACER MODEL AND SCRIPT FILE
  ReadInModel("ray.dat");

  // --- init VESA ---

  // Detect the VBE bios
  if( (vbeInfo = vbeDetect()) == NULL )
  {
   printf("Can't find VESA bios extentions\n");
   exit(0);
  }
  // Check for VBE 2.0
  if( vbeInfo->VbeVersion < 0x200 )
  {
   printf("VESA bios extention 2.0 or better needed\n");
   exit(0);
  }

  VbeScreen = vbeOpen(SCREENWIDTH,SCREENHEIGHT,24);
  if(VbeScreen == NULL)
  {
   printf("Could not set %ix%i %ibpp mode\n",SCREENWIDTH,SCREENHEIGHT,24);
   exit(0);
  }
  Screen = (void *)VbeScreen->lfb;
  VesaMode = VbeScreen->vbeMode;

  // RAYTRACE THE SCENE
  while (!kbhit())
  {
   RayTraceScene();
   // VIEWERDISTANCE -= 5;
   Objects[1].Type = SPHERE;
   Objects[1].Color.Red = 0;
   Objects[1].Color.Green = 0.5;
   Objects[1].Color.Blue = 1;
   Objects[1].IsActive = 1;
   ((SphereType*)Objects[1].Obj)->Center.x = 600;
   ((SphereType*)Objects[1].Obj)->Center.y = 400;
   ((SphereType*)Objects[1].Obj)->Center.z = 4000;
   ((SphereType*)Objects[1].Obj)->Radius = 200;
  }
  getch();

  vmode(3);
  VbeClose(VbeScreen);
  vmode(3);

  // FREE THE RAYTRACE MODEL
	FreeModel();



 }

