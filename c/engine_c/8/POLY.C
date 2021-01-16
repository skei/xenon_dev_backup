

signed int u_frac_adder;        // xxxx.0000
signed int v_frac_color_adder;  // xxxx.xx:xx
signed char u_int_adder;
signed char v_int_adder;
char *ShadeTable; // 256 shades * 256 colors

 // eax 0
 // ebx
 // ecx v.frac       c.int : c.frac
 // edx textureû     v.int : u.int
 // esi u.frac
 // edi ----  screenposý  ----
 // ebp ---- line-lengthü ----
 //
 // û 64k aligned...
 // ý right edge of line
 // ü negaive line length * bpp


 /* On entry:
  ebx   negative line length * bpp
  ecx   v.frac << 16 + color(8:8)
  edx   texture + (v<<8)+u
  esi   u.frac << 16
  edi   screen pos, riht edge
  + add adders set up...
 */

#pragma aux FillSpan=             \
 " push ebp                     ",\
 " xor eax                      ",\
 " mov ebp,ebx                  ",\
 "L1:                           ",\
 " mov al,[edx]                 ",\
 " add esi,[u_frac_adder]       ",\
 " adc dl,[u_int_adder]         ",\
 " mov ah,ch                    ",\
 " add ecx,[v_frac_color_adder] ",\
 " mov bl,[ShadeTable+eax]      ",\
 " adc dh,[v_int_adder          ",\
 " mov [edi+ebp],bl             ",\
 " inc ebp                      ",\
 " jnz L1                       ",\
 " pop ebp                      ",\

 #define spanlength 16

inline void TextureLine()
 {
  int width;
  float r,invu,invv,invz;
  int u1,v1,u2,v2;

  width = x2-x1+1;
  if (width > spanlength)
  {
   invz = 1/z;
   invu = (u*invz);
   invv = (v*invz);
   u1 = float2fixed(u);
   v1 = float2fixed(v);
   invu += invu_adder; // precalced & multiplied by spanlength
   invv += invv_adder;
   invz += invz_adder;
   r = 1/invz;
   u2 = float2fixed(invu*r);
   v2 = float2fixed(invv*r);
   while (width > spanlength)
   {
    invu+=invu_adder;
    invv+=invv_adder;
    invz+=invz_adder;
    r=1/invz;
    // set up adders, etc, for FillSpan
    FillSpan();
    u1=u2;
    v1=v2;
    u2=invu*r;
    v2=invv*r;
    width-=16;
   }
  }
  if (width > 0)
  {
  }
 }


