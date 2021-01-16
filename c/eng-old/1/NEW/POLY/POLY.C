char *HWritePagePtr;  // (char *)0xa0000
int HLeftClip;        // 0
int HRightClip;       // 320
int HTopClip;         // 0
int HBotClip;         // 200

// --------------------------------------------------------------------------

int shl10idiv(int x, int y);
#pragma aux shl10idiv = \
 " mov   edx, eax "\
 " shl   eax, 10  "\
 " sar   edx, 22  "\
 " idiv  ebx      "\
 parm [eax] [ebx] \
 modify exact [eax edx] \
 value [eax]

// --------------------------------------------------------------------------

struct vertex
{
 int x,y;    // screen coordinates (integers)
 int u,v;    // vertex u,v (26:6 bit fixed point)
};

// --------------------------------------------------------------------------

static vertex *left_array[3], * right_array[3];
static int left_section, right_section;
static int left_section_height, right_section_height;
static int left_u, delta_left_u, left_v, delta_left_v;
static int left_x, delta_left_x, right_x, delta_right_x;
static int vertclip, horzclip;

// --------------------------------------------------------------------------

static int RightSection(void)
{
 vertex * v1 = right_array[ right_section ];
 vertex * v2 = right_array[ right_section-1 ];
 int height;

 if(!vertclip)
 {
  height = v2->y - v1->y;
  if(height == 0) return 0;
  delta_right_x = ((v2->x - v1->x) << 16) / height;
  right_x = v1->x << 16;
 }
 else
 {
  int y1 = v1->y;
  int y2 = v2->y;
  int oheight = height = y2-y1;
  if((y2 -= HBotClip) > 0) height -= y2;
  y2 = 0;
  if(y1 < HTopClip) y2 = HTopClip - y1;
  if((height -= y2) <= 0) return height;
  delta_right_x = ((v2->x - v1->x) << 16) / oheight;
  right_x = (v1->x << 16) + delta_right_x * y2;
 }
 right_section_height = height;
 return height;
}

// --------------------------------------------------------------------------

static int LeftSection(void)
{
    vertex * v1 = left_array[ left_section ];
    vertex * v2 = left_array[ left_section-1 ];
    int height;

    if(!vertclip)
    {
        height = v2->y - v1->y;
        if(height == 0)
            return 0;
        delta_left_x = ((v2->x - v1->x) << 16) / height;
        left_x = v1->x << 16;
        delta_left_u = ((v2->u - v1->u) << 10) / height;
        left_u = v1->u << 10;
        delta_left_v = ((v2->v - v1->v) << 10) / height;
        left_v = v1->v << 10;
    }
    else
    {
        int y1 = v1->y;
        int y2 = v2->y;
        int oheight = height = y2-y1;   // Unclipped height

        if((y2 -= HBotClip) > 0)
            height -= y2;               // Skip pixels at end
        y2 = 0;
        if(y1 < HTopClip)
            y2 = HTopClip - y1;
        if((height -= y2) <= 0)         // Skip pixels at start
            return height;
        // y2 = pixels skipped at start

        delta_left_x = ((v2->x - v1->x) << 16) / oheight;
        left_x = (v1->x << 16) + delta_left_x * y2;
        delta_left_u = ((v2->u - v1->u) << 10) / oheight;
        left_u = (v1->u << 10) + delta_left_u * y2;
        delta_left_v = ((v2->v - v1->v) << 10) / oheight;
        left_v = (v1->v << 10) + delta_left_v * y2;
    }

    left_section_height = height;
    return height;
}
// --------------------------------------------------------------------------


void DrawTextureTriangle(vertex * vtx, char * bitmap)
{
 vertex *v1 = vtx;
 vertex *v2 = vtx+1;
 vertex *v3 = vtx+2;

 if(v1->y > v2->y) { vertex * v=v1; v1=v2; v2=v; }
 if(v1->y > v3->y) { vertex * v=v1; v1=v3; v3=v; }
 if(v2->y > v3->y) { vertex * v=v2; v2=v3; v3=v; }
 int top;
 if((top = v1->y) < HTopClip)
 {
  top = HTopClip;
  vertclip = 1;
 }
 else if(v3->y > HBotClip) vertclip = 1;
 else vertclip = 0;
 int height = v3->y - v1->y;
 if(height == 0) return;
 int temp = ((v2->y - v1->y)<<16)/height;
 int longest = temp*(v3->x - v1->x)+((v1->x - v2->x)<<16);
 if(longest == 0) return;
 if(longest < 0)
 {
  right_array[0] = v3;
  right_array[1] = v2;
  right_array[2] = v1;
  right_section  = 2;
  left_array[0]  = v3;
  left_array[1]  = v1;
  left_section   = 1;
  if(LeftSection() <= 0) return;
  if(RightSection() <= 0)
  {
   right_section--;
   if(RightSection() <= 0) return;
  }
  if(longest > -0x1000) longest = -0x1000;
 }
 else
 {
  left_array[0]  = v3;
  left_array[1]  = v2;
  left_array[2]  = v1;
  left_section   = 2;
  right_array[0] = v3;
  right_array[1] = v1;
  right_section  = 1;
  if(RightSection() <= 0) return;
  if(LeftSection() <= 0)
  {
   left_section--;
   if(LeftSection() <= 0) return;
  }
  if(longest < 0x1000) longest = 0x1000;
 }
 if(v1->x > HRightClip ||
    v1->x < HLeftClip  ||
    v2->x > HRightClip ||
    v2->x < HLeftClip  ||
    v3->x > HRightClip ||
    v3->x < HLeftClip) horzclip = 1;
 else horzclip = 0;
 int dudx = shl10idiv(temp*(v3->u - v1->u)+((v1->u - v2->u)<<16),longest);
 int dvdx = shl10idiv(temp*(v3->v - v1->v)+((v1->v - v2->v)<<16),longest);
 char * destptr = (char *) (top * 320 + HWritePagePtr);
 for(;;)
 {
  int x1, x2, width, u, v;
  if(!horzclip)
  {
   x1 = left_x >> 16;
   width = (right_x >> 16) - x1;
   u = left_u;
   v = left_v;
  }
  else
  {
   x1 = left_x >> 16;
   x2 = right_x >> 16;
   int owidth = width = x2-x1;
   if((x2 -= HRightClip) > 0) width -= x2;
   x2 = 0;
   if(x1 < HLeftClip)
   {
    x2 = HLeftClip - x1;
    x1 = HLeftClip;
   }
   if((width -= x2) <= 0) goto linedone;
   u = (left_u + dudx * x2);
   v = (left_v + dvdx * x2);
  }
  if(width > 0)
  {
   char * dest = destptr + x1;
   int du = dudx;
   int dv = dvdx;
   do
   {
    *dest++ = bitmap[ ((v>>8) & 0xff00) + ((u>>16) & 0x00ff) ];
    u += du;
    v += dv;
   } while(--width);
  }
linedone:
  destptr += 320;
  if(--left_section_height == 0)
  {
   if(--left_section <= 0) return;
   if(LeftSection() <= 0) return;
  }
  else
  {
   left_x += delta_left_x;
   left_u += delta_left_u;
   left_v += delta_left_v;
  }
  if(--right_section_height == 0)
  {
   if(--right_section <= 0) return;
   if(RightSection() <= 0) return;
  }
  else
  {
   right_x += delta_right_x;
  }
 }
}
