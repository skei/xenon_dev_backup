#define Qt_Norm(q) ((q).x*(q).x+(q).y*(q).y+(q).z*(q).z);
#define X 0
#define Y 1
#define Z 2
#define MAXFRAMES 	5000
#define MAXCAMERAS	1
#define MAXOBJECTS	5
#define MAX_Z				40000
#define STEPPERPERS 16	 											// how often perspective
#define STEPONE     1/STEPPERPERS
#define ZBUFFER			TRUE

#define DEPTH			  15
#define NUMCOLORS		(1 << DEPTH)							// number of colors on screen
#define BYTESPP   	((DEPTH+1) >> 3)          // bytes per pixel
#define pi        	3.1415

#undef  NULL
#define NULL 0L

typedef float MATRIX[4][3];
typedef float VECT[4];

typedef float quat[4];
typedef float mmatrix[4][4];

static MATRIX Identity={{1.0,0.0,0.0}
					   					 ,{0.0,1.0,0.0}
										   ,{0.0,0.0,1.0}
										   ,{0.0,0.0,0.0}};

// Structures

struct sCamera
{
  VECT Pos,Aim;
	float Fov,Focal,Roll;
	MATRIX cMatrix;
};

struct sObject
{
	MATRIX oMatrix;
};

struct sLight
{
	VECT Pos;
	VECT RGBVal;
};

struct sCamera sizen;
struct sObject sizenO;
struct sLight sizenL;

typedef struct sCamera* CamPTR;
typedef struct sObject* ObjPTR;
typedef struct sLight*  LigPTR;

BPTR tempptr;
BPTR texture;


// pointers
UWORD  MAX_X,MAX_Y;
UWORD  MAX_X2,MAX_Y2;
CamPTR CameraFrames;					// keyframes to camera
CamPTR Cameras;								// values to cameras
ObjPTR ObjectFrames;					// keyframes to object
LigPTR LightFrames;						// keyframes to light
VECT   LightV;
VECT	 NormalV;
float  light;
LPTR   rVertex[MAXOBJECTS];		// rotated points
LPTR   Vertex[MAXOBJECTS];		// points
LPTR   tVertex[MAXOBJECTS];		// texture points
LPTR	 Normals;								// normals for each vertex
UWPTR  Face[MAXOBJECTS];			// faces

BPTR   VESAptr;                // ptr to video memory
UWPTR  screen;								// fake screen
BPTR	 pic;										// texture
BPTR	 temp;
UWPTR  Table;									// table for shading rgb

// radix vars

unsigned short int Numbers[2512];
unsigned short int Stack1[2512];
unsigned short int Stack2[2512];
unsigned short int DStack1[2512];
unsigned short int DStack2[2512];
unsigned short int DrawList[2512];
unsigned short int a,b,c,d,e;

// standard variables

char 	 keys[256];
char 	 key;
ULONG  SSIZE;
float	 WinSize;
UWORD  CLIPX1,CLIPX2;
UWORD  CLIPY1,CLIPY2;
char   ASCNAME[80];
char   VUENAME[80];
char   PICNAME[80];
ULONG  RotationTime,DrawTime,SortTime,LineTime,TotalTime;
BPTR 	 fontbuf;
WORD	 FontLoaded=FALSE;
BYTE	 WAIT;
UWORD  StartFrame;
UWORD  EndFrame;
UWORD  FrameNum;
LONG   i,j,f;
LONG   X1,Y1,Z1,X2,Y2,Z2,X3,Y3,Z3;
LONG   tX1,tY1,tX2,tY2,tX3,tY3;
MATRIX rMatrix;
LONG   FaceNum[MAXOBJECTS];
LONG   VertexNum[MAXOBJECTS];
UWORD  ActiveCamera;
ULONG  AllocatedMem;
char	 RGB[768];
LONG	 lig1,lig2,lig3;
UWORD	 F1,F2,F3;
UWORD  NUMINTER;
UWORD  ObjectCount;
UBYTE  BackFace;
UWORD  SCALER;
UBYTE  Mapping;
LONG	 MIN_Z;
LONG   ee;
UWORD	 itimer;
UWORD  oldtimer;
UWORD  step;
UWORD	 FastStep;
UWORD	 SlowStep;
UWORD  pFaces;
BYTE	 debugmode;
char	 Mapped;
long	maxx,minx,maxy,miny,maxz,minz;
long	ttt;

LONG	 LightX,LightY;


void LoadVUE(char *filename)
{
	FILE *fp;
  char temp[180];
	char temp2[180];
	char done;
	char ObjectName[20];
	char LightName[20];
  float tempreal;
	UWORD rr;
	UWORD objnum;
	UWORD i;
	UWORD j;
	UWORD j2;
	UWORD r;
	char name[20];

	if ((fp = fopen(filename,"rt")) == NULL)
  {
		printf("þ Error loading VUE file!\n");
		exit(1);
	}
	StartFrame=0xffff;
	EndFrame=0;
	done=0;
	while (done==0)
	{
		if (fscanf(fp,"%s",name) == EOF)
		{
		  done=1;
		}
		else
		{
			if (strcmp(name,"frame")==0) 			// is this the frame number?
			{
//  			_clearscreen(_GCLEARSCREEN);
				fscanf(fp,"%d",&FrameNum);
//  	 		printf("Frame = %d\n",FrameNum);
				if (FrameNum > EndFrame)
				{
	 				EndFrame=FrameNum;
				}
				if (FrameNum < StartFrame)
				{
	 				StartFrame=FrameNum;
				}

			}
			if (strcmp(name,"transform")==0) 	// is this the transformation matrix?
			{
//				printf("Objectinfo :\n");
				fscanf(fp,"%s",ObjectName);
//				printf("  Objectname = %s\n",ObjectName);
				for (i=0;i<4;i++)
					for (j=0;j<3;j++)
					{
						fscanf(fp,"%e",&tempreal);
  	        ObjectFrames[FrameNum].oMatrix[i][j]=tempreal;
//						printf("  Matrix = %f\n",tempreal);
					}
			}
			if (strcmp(name,"light")==0)		 	// is this the ligth values?
			{
//				printf("Ligthinfo :\n");
				fscanf(fp,"%s",LightName);
//				printf("  Lightname = %s\n",LightName);
				for (i=0;i<3;i++)
				{
					fscanf(fp,"%e",&tempreal);
					LightFrames[FrameNum].Pos[i]=tempreal*SCALER;
//					printf("  Light pos = %f\n",tempreal);
				}
				for (i=0;i<3;i++)
				{
					fscanf(fp,"%e",&tempreal);
					LightFrames[FrameNum].RGBVal[i]=tempreal;
//					printf("  RGBVal = %f\n",tempreal);
				}
			}
			if (strcmp(name,"camera")==0) 	 	// is this the camera values?
			{
//				printf("Camerainfo :\n");
				for (i=0;i<3;i++)
				{
					fscanf(fp,"%e",&tempreal);	// read camera's position
					CameraFrames[FrameNum].Pos[i]=tempreal*SCALER;
//					printf("  Position %d = %f\n",i,tempreal);
				}
				for (i=0;i<3;i++)
				{
					fscanf(fp,"%e",&tempreal);	// read camera's target
					CameraFrames[FrameNum].Aim[i]=tempreal*SCALER;
//					printf("  Target %d = %f\n",i,tempreal);
				}
				fscanf(fp,"%e",&tempreal);		// read camera's roll
				CameraFrames[FrameNum].Roll=tempreal;
//				printf("  Roll = %f\n",tempreal);

				fscanf(fp,"%e",&tempreal);		// read camera's FOV
				CameraFrames[FrameNum].Fov=tempreal;
//				printf("  Fov = %f\n",tempreal);
			}
		}
	}
//	printf("Starting frame = %d\n",StartFrame);
//	printf("Ending frame = %d\n",EndFrame);
  fclose(fp);
}

void LoadASC(char *filename)
{
	FILE *fp2;
	char done;
	UWORD j;
	UWORD j2;
	unsigned int f1,f2,f3;
	float xx,yy,zz,u,v;
	char name[120];
	char temp[100];
	char temp2[100];
	BYTE ObjNumm;

	maxx=-100000;
	minx=100000;
	maxy=-100000;
	miny=100000;
	maxz=-100000;
	minz=100000;

	if ((fp2 = fopen(filename,"rt")) == NULL)
  {
		printf("þ Error loading ASC file!\n");
		exit(1);
	}
 	Mapped=FALSE;
	j=0;
	j2=0;
	done=0;
	ObjNumm=0;
	while (done==0)
	{
		if (fscanf(fp2,"%s",name) == EOF)
		{
		  done=1;
		}
		else
		{
			if (strcmp(name,"Vertices:")==0)
			{
				fscanf(fp2,"%d",&j);
				fscanf(fp2,"%s",temp);
				fscanf(fp2,"%d",&j2);
				fscanf(fp2,"%s",name);
				VertexNum[ObjNumm]=j;
				FaceNum[ObjNumm]=j2;
				if ((tVertex[ObjNumm]=(LPTR)getmem(VertexNum[ObjNumm]*3*4+20))==NULL)
				{
					printf("þ Error allocating memory\n");
					exit(1);
				}
				if ((rVertex[ObjNumm]=(LPTR)getmem(VertexNum[ObjNumm]*3*4+20))==NULL)
				{
					printf("þ Error allocating memory\n");
					exit(1);
				}
				if ((Vertex[ObjNumm]=(LPTR)getmem(VertexNum[ObjNumm]*3*4+20))==NULL)
				{
					printf("þ Error allocating memory\n");
					exit(1);
				}
				if ((Face[ObjNumm]=(UWPTR)getmem(FaceNum[ObjNumm]*3*2+20))==NULL)
				{
					printf("þ Error allocating memory\n");
					exit(1);
				}
			}
			if (strcmp(name,"Mapped")==0)
			{
				Mapped=TRUE;
			}
			if (strcmp(name,"Vertex")==0)
			{
				fscanf(fp2,"%s",temp);
				if (strcmp(temp,"list:")==0)
				{
					i=0;
					while (i<VertexNum[ObjNumm])
					{
						fscanf(fp2,"%s",temp);
						if (strcmp(temp,"Vertex")==0)
						{
							fscanf(fp2,"%s",temp);
							fscanf(fp2,"%2s%e",temp,&xx);
							fscanf(fp2,"%2s%e",temp,&yy);
							fscanf(fp2,"%2s%e",temp,&zz);
							if (Mapped==TRUE)
							{
								fscanf(fp2,"%2s%e",temp,&u);
								fscanf(fp2,"%2s%e",temp,&v);
							}
							else
							{
				 				u=((float)xx+3000)/256;
				 				v=((float)yy+3000)/256;
							}
							if (Mapping==0)
							{
				 				u=((float)xx+3000)/256;
				 				v=((float)yy+3000)/256;
							}
							xx*=SCALER;
							yy*=SCALER;
							zz*=SCALER;
							if (xx > maxx)
								maxx=xx;
							if (xx < minx)
								minx=xx;
							if (yy > maxy)
								maxy=yy;
							if (yy < miny)
								miny=yy;
							if (zz > maxz)
								maxz=zz;
							if (zz < minz)
								minz=zz;
							Vertex[ObjNumm][i*3+0]=xx;
							Vertex[ObjNumm][i*3+1]=yy;
							Vertex[ObjNumm][i*3+2]=zz;
							tVertex[ObjNumm][i*3+0]=(u*256.0);
							tVertex[ObjNumm][i*3+1]=(v*256.0);
							i++;
						}
					}
				}
			}
			if (strcmp(name,"Face")==0)
			{
				fscanf(fp2,"%s",temp);
				if (strcmp(temp,"list:")==0)
				{
					i=0;
					while (i<FaceNum[ObjNumm])
					{
						fscanf(fp2,"%s",temp);
						if (strcmp(temp,"Face")==0)
						{
							fscanf(fp2,"%s",temp);
							fscanf(fp2,"%2s%d",temp,&f1);
							fscanf(fp2,"%2s%d",temp,&f2);
							fscanf(fp2,"%2s%d",temp,&f3);
							Face[ObjNumm][i*3+0]=f1*3;
							Face[ObjNumm][i*3+1]=f2*3;
							Face[ObjNumm][i*3+2]=f3*3;
							i++;
						}
					}
				}
				ObjNumm++;
			}
		}
	}
	ObjectCount=ObjNumm;
  fclose(fp2);
}

void MakeCamMatrix(VECT pos, VECT aim, MATRIX m)
{
  MATRIX	RotX,RotY;
	VECT		d,dp,d2;
	float		sinX,cosX,sinY,cosY;

	memmove(&RotX,&Identity,sizeof(MATRIX));
	memmove(&RotY,&Identity,sizeof(MATRIX));

  dp[Y]=0.0;
  dp[X]=d[X]=aim[X]-pos[X];
  d[Y]=aim[Y]-pos[Y];
  dp[Z]=d[Z]=aim[Z]-pos[Z];

  Normalize(d);
  Normalize(dp);

	if (!dp[X]&&!dp[Z])
 	{
		sinY=0.0;
	  cosY=1.0;
	}
	else
	{
		sinY=-dp[X];
	  cosY=dp[Z];
	}
	RotY[Z][X]= sinY;
	RotY[X][Z]=-sinY;
	RotY[X][X]= cosY;
	RotY[Z][Z]= cosY;

	d2[X]=(d[X]*RotY[X][X])+(d[Y]*RotY[Y][X])+(d[Z]*RotY[Z][X]);
	d2[Y]=(d[X]*RotY[X][Y])+(d[Y]*RotY[Y][Y])+(d[Z]*RotY[Z][Y]);
	d2[Z]=(d[X]*RotY[X][Z])+(d[Y]*RotY[Y][Z])+(d[Z]*RotY[Z][Z]);

	if (!d2[Y]&&!d2[Z])
	{
		sinX=0.0;
	  cosX=1.0;
	}
	else
	{
		sinX=d2[Y];
	  cosX=d2[Z];
	}

	RotX[Y][Z]= sinX;
	RotX[Z][Y]=-sinX;
	RotX[Y][Y]= cosX;
	RotX[Z][Z]= cosX;

	MatMult(RotY,RotX,m);
}

void MatMult(MATRIX a, MATRIX b, MATRIX c)
{
	int j;
  MATRIX temp;

  for (j=0;j<3;j++)
  {
		temp[0][j]=a[0][0]*b[0][j] + a[0][1]*b[1][j] + a[0][2]*b[2][j];
    temp[1][j]=a[1][0]*b[0][j] + a[1][1]*b[1][j] + a[1][2]*b[2][j];
    temp[2][j]=a[2][0]*b[0][j] + a[2][1]*b[1][j] + a[2][2]*b[2][j];
    temp[3][j]=a[3][0]*b[0][j] + a[3][1]*b[1][j] + a[3][2]*b[2][j]+b[3][j];
	}
  memmove(c,temp,sizeof(MATRIX));
}

void Normalize(VECT v)
{
	float Length;

  Length=sqrt((v[X]*v[X])+(v[Y]*v[Y])+(v[Z]*v[Z]));
	if (Length<1e-5)
  {
		v[X]=0;
    v[Y]=0;
    v[Z]=0;
	}
	else
	{
		Length=1.0/Length;
    v[X]*=Length;
    v[Y]*=Length;
    v[Z]*=Length;
	}
}

void qtomatrix(quat q,mmatrix m)
{
}

void ProcessObject(void)
{
	float x,y,z,xx,yy,zz;

	for (j=0;j<ObjectCount;j++)
	{
		for (i=0;i<(VertexNum[j]+1);i++)
		{
			x=(-Vertex[j][i*3+0]);//-ObjectFrames[FrameNum].oMatrix[3][0];
			y=(Vertex[j][i*3+2]);//-ObjectFrames[FrameNum].oMatrix[3][2];
			z=(Vertex[j][i*3+1]);//-ObjectFrames[FrameNum].oMatrix[3][1];

      xx=x-Cameras[ActiveCamera].Pos[0];
    	yy=y-Cameras[ActiveCamera].Pos[1];
    	zz=z-Cameras[ActiveCamera].Pos[2];

			x=(xx*Cameras[ActiveCamera].cMatrix[0][0]+
         yy*Cameras[ActiveCamera].cMatrix[1][0]+
         zz*Cameras[ActiveCamera].cMatrix[2][0]);
			y=(xx*Cameras[ActiveCamera].cMatrix[0][1]+
         yy*Cameras[ActiveCamera].cMatrix[1][1]+
         zz*Cameras[ActiveCamera].cMatrix[2][1]);
			z=(xx*Cameras[ActiveCamera].cMatrix[0][2]+
         yy*Cameras[ActiveCamera].cMatrix[1][2]+
         zz*Cameras[ActiveCamera].cMatrix[2][2]);

			rVertex[j][(i*3+0)]=-x;
			rVertex[j][(i*3+1)]=-y;
			rVertex[j][(i*3+2)]=z;
		}
	}
}

float CalcFOV(float aFOV)
{
	float temp;

	temp=((CLIPX2-CLIPX1) >> 1)/(sin((aFOV/360)*pi)/cos((aFOV/360)*pi));
	return(temp);
}

void SetCameraPos(int cnum,float xx,float yy,float zz)
{
	Cameras[cnum].Pos[X]=xx;
	Cameras[cnum].Pos[Y]=yy;
	Cameras[cnum].Pos[Z]=zz;
}

void SetCameraAim(int cnum,float xx,float yy,float zz)
{
	Cameras[cnum].Aim[X]=xx;
	Cameras[cnum].Aim[Y]=yy;
	Cameras[cnum].Aim[Z]=zz;
}

float Average(float a,float b)
{
	float temp;
	temp=(a+b)/2;
	return(temp);
}

void Interpol(void)
{
	float temp,a,b;
	UWORD j,j2;
	int i;

	if ((EndFrame*2)<MAXFRAMES)
	{
		for (i=EndFrame;i>=1;i--)
		{
			memmove(&CameraFrames[i*2],&CameraFrames[i],sizeof(CameraFrames[i]));
			memmove(&ObjectFrames[i*2],&ObjectFrames[i],sizeof(ObjectFrames[i]));
			memmove(&LightFrames[i*2],&LightFrames[i],sizeof(LightFrames[i]));
		}
		for (i=0;i<=EndFrame;i++)
		{
			for (j=0;j<4;j++)
				for (j2=0;j2<3;j2++)
				{
					a=ObjectFrames[i*2+0].oMatrix[j][j2];
					b=ObjectFrames[i*2+2].oMatrix[j][j2];
					ObjectFrames[i*2+1].oMatrix[j][j2]=Average(a,b);
				}
			for (j=0;j<3;j++)
			{
				a=CameraFrames[i*2+0].Pos[j];
				b=CameraFrames[i*2+2].Pos[j];
				CameraFrames[i*2+1].Pos[j]=Average(a,b);

				a=CameraFrames[i*2+0].Aim[j];
				b=CameraFrames[i*2+2].Aim[j];
				CameraFrames[i*2+1].Aim[j]=Average(a,b);
			}
			a=CameraFrames[i*2+0].Fov;
			b=CameraFrames[i*2+2].Fov;
			CameraFrames[i*2+1].Fov=Average(a,b);

			for (j=0;j<3;j++)
			{
				a=LightFrames[i*2+0].Pos[j];
				b=LightFrames[i*2+2].Pos[j];
				LightFrames[i*2+1].Pos[j]=Average(a,b);
			}
		}
		EndFrame*=2;
	}
}













    SetCameraPos(ActiveCamera,
								 -CameraFrames[FrameNum].Pos[X],
  								CameraFrames[FrameNum].Pos[Z],
	    					  CameraFrames[FrameNum].Pos[Y]);
		SetCameraAim(ActiveCamera,
								 -CameraFrames[FrameNum].Aim[X],
								  CameraFrames[FrameNum].Aim[Z],
								  CameraFrames[FrameNum].Aim[Y]);
		MakeCamMatrix(Cameras[ActiveCamera].Pos,
									Cameras[ActiveCamera].Aim,
									Cameras[ActiveCamera].cMatrix);
		Cameras[ActiveCamera].Focal=CalcFOV(2400/(CameraFrames[FrameNum].Fov));

		ProcessObject();
		radixsort();
		for (j=0;j<ObjectCount;j++)
		{
			l=0;
			DrawTime=tRDTSC();
			for (i=0;i<FaceNum[j];i++)
			{
				f=i*3;
				f=DrawList[i]*3;
				F1=Face[j][f+0];
				F2=Face[j][f+1];
				F3=Face[j][f+2];
				X1=Vertex[j][F1+X];
				Y1=Vertex[j][F1+Y];
				Z1=Vertex[j][F1+Z];
				X2=Vertex[j][F2+X];
				Y2=Vertex[j][F2+Y];
				Z2=Vertex[j][F2+Z];
				X3=Vertex[j][F3+X];
				Y3=Vertex[j][F3+Y];
				Z3=Vertex[j][F3+Z];
				Normal(X2-X1,Y2-Y1,Z2-Z1,X3-X1,Y3-Y1,Z3-Z1);
				LightV[X]=CameraFrames[FrameNum].Pos[X]-X1;
				LightV[Y]=CameraFrames[FrameNum].Pos[Y]-Y1;
				LightV[Z]=CameraFrames[FrameNum].Pos[Z]-Z1;
				light=DotProduct(NormalV,LightV);
				if (BackFace==0)
					light=1.0;
				if (light>0)							// is the face visible?
				{
					Z1=rVertex[j][F1+Z];
					Z2=rVertex[j][F2+Z];
					Z3=rVertex[j][F3+Z];

					if ((Z1>MIN_Z) || (Z2>MIN_Z) || (Z3>MIN_Z))
					{
						X1=rVertex[j][F1+X];
						Y1=rVertex[j][F1+Y];
						X2=rVertex[j][F2+X];
						Y2=rVertex[j][F2+Y];
						X3=rVertex[j][F3+X];
						Y3=rVertex[j][F3+Y];

						NormalV[X]=Normals[F1+X];
						NormalV[Y]=Normals[F1+Y];
						NormalV[Z]=Normals[F1+Z];
						LightV[0]=(LightFrames[FrameNum].Pos[X]-Vertex[j][F1+X]);
						LightV[1]=(LightFrames[FrameNum].Pos[Y]-Vertex[j][F1+Y]);
						LightV[2]=(LightFrames[FrameNum].Pos[Z]-Vertex[j][F1+Z]);
						light=DotProduct(NormalV,LightV);
						if (light<0) light=0;
						if (light>1) light=1;
            lig1=light*63;

						NormalV[X]=Normals[F2+X];
						NormalV[Y]=Normals[F2+Y];
						NormalV[Z]=Normals[F2+Z];
						LightV[0]=(LightFrames[FrameNum].Pos[X]-Vertex[j][F2+X]);
						LightV[1]=(LightFrames[FrameNum].Pos[Y]-Vertex[j][F2+Y]);
						LightV[2]=(LightFrames[FrameNum].Pos[Z]-Vertex[j][F2+Z]);
						light=DotProduct(NormalV,LightV);
						if (light<0) light=0;
						if (light>1) light=1;
            lig2=light*63;

						NormalV[X]=Normals[F3+X];
						NormalV[Y]=Normals[F3+Y];
						NormalV[Z]=Normals[F3+Z];
						LightV[0]=(LightFrames[FrameNum].Pos[X]-Vertex[j][F3+X]);
						LightV[1]=(LightFrames[FrameNum].Pos[Y]-Vertex[j][F3+Y]);
						LightV[2]=(LightFrames[FrameNum].Pos[Z]-Vertex[j][F3+Z]);
						light=DotProduct(NormalV,LightV);
						if (light<0) light=0;
						if (light>1) light=1;
            lig3=light*63;

//						lig1=lig2=lig3=25;

				 		tX1=(tVertex[j][F1+X]);
				 		tY1=(tVertex[j][F1+Y]);
				 		tX2=(tVertex[j][F2+X]);
				 		tY2=(tVertex[j][F2+Y]);
			  		tX3=(tVertex[j][F3+X]);
			  		tY3=(tVertex[j][F3+Y]);
						if ((Z1>MIN_Z) && (Z2>MIN_Z) && (Z3>MIN_Z))
						{
 							X1=((X1*Cameras[ActiveCamera].Focal)/Z1)+(MAX_X >> 1);
							Y1=((Y1*Cameras[ActiveCamera].Focal)/Z1)+(MAX_Y >> 1);
							X2=((X2*Cameras[ActiveCamera].Focal)/Z2)+(MAX_X >> 1);
							Y2=((Y2*Cameras[ActiveCamera].Focal)/Z2)+(MAX_Y >> 1);
							X3=((X3*Cameras[ActiveCamera].Focal)/Z3)+(MAX_X >> 1);
							Y3=((Y3*Cameras[ActiveCamera].Focal)/Z3)+(MAX_Y >> 1);

							if ((X1>=CLIPX1)||(X2>=CLIPX1)||(X3>=CLIPX1))
								if ((X1<CLIPX2)||(X2<CLIPX2)||(X3<CLIPX2))
									if ((Y1>=CLIPY1)||(Y2>=CLIPY1)||(Y3>=CLIPY1))
										if ((Y1<CLIPY2)||(Y2<CLIPY2)||(Y3<CLIPY2))
										{
											if ((lig1<1) && (lig2<1) && (lig3<1))
											{
												FlatTetra(X1,Y1,X2,Y2,X3,Y3,0);
											}
											else
											{
//												TextureTetra(X1,Y1,X2,Y2,X3,Y3,tX1,tY1,tX2,tY2,tX3,tY3,pic,screen,lig1,lig2,lig3);
	      								TetraTexture(X1,Y1,Z1,X2,Y2,Z2,X3,Y3,Z3,tX1,tY1,tX2,tY2,tX3,tY3,lig1,lig2,lig3);
											}
										}
						}
						else
						{
  	  				xCutTetra(X1,Y1,Z1,X2,Y2,Z2,X3,Y3,Z3,tX1,tY1,tX2,tY2,tX3,tY3,lig1,lig2,lig3);
						}
					}
				}
			}
			DrawTime=tRDTSC()-DrawTime;
		}
