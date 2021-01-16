#include <windows.h>
#include <ddraw.h>

/*
char unsigned *GetOffscreenBits(void)
long GetOffscreenStride(void)
int OffscreenLock(void)
int OffscreenUnlock(void)
void SwapBuffer(void)
void SwapRect(int Left, int Top, int Right, int Bottom)
int BeginFullScreen(int Width, int Height, int Depth, char *pal)
void EndFullScreen(void)
*/

void main_init()

//****************************************************************************
// Internal globals

static HWND Window =0;
static HINSTANCE ghInstance = 0;

static LPDIRECTDRAW pDirectDraw =0;
static LPDIRECTDRAWSURFACE pPrimarySurface =0;
static LPDIRECTDRAWSURFACE pOffscreenSurface =0;
static LPDIRECTDRAWPALETTE pPalette =0;

static int PageFlip =0;

static char unsigned *pBits =0;
static long Stride =0;

//****************************************************************************
// Application entry point

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpszCmdParam, int nCmdShow)
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR, int)
{
 // Store this away for future use
 ghInstance = hInstance;
// Transfer control to the demo
// main_init();
// main_process();
// main_exit();
 //	DemoMain();
 return 0;
}

//****************************************************************************
// Offscreen access

char unsigned *GetOffscreenBits(void)
{
 return pBits;
}

// -----

long GetOffscreenStride(void)
{
 return Stride;
}

int OffscreenLock(void)
{
 DDSURFACEDESC SurfaceDesc;
 int ReturnValue;
 HRESULT DDReturn;

 ReturnValue = 0;
 pBits = 0;
 Stride = 0;
 ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
 SurfaceDesc.dwSize = sizeof(SurfaceDesc);
 // Loop until an error occurs or the lock succeeds
 DDReturn = DD_OK;
 while (1)
 {
  // Attempt the lock
//  DDReturn = pOffscreenSurface->Lock(0, &SurfaceDesc, 0, 0);
  DDReturn = IDirectDraw_Lock(pOffscreenSurface,0, &SurfaceDesc, 0, 0);
	if (DDReturn == DD_OK)
  {
   // Successful lock - store bits and stride
   pBits = (char unsigned *)SurfaceDesc.lpSurface;
   Stride = SurfaceDesc.lPitch;
   ReturnValue = 1;
   break;
  }
  else if (DDReturn == DDERR_SURFACELOST)
  {
   // Attempt to restore the surfaces
//   DDReturn = pPrimarySurface->Restore();
   DDReturn = IDirectDraw_Restore(pPrimarySurface);
   if (DDReturn == DD_OK)
//   DDReturn = pOffscreenSurface->Restore();
   DDReturn = IDirectDraw_Restore(pOffscreenSurface);
   if (DDReturn != DD_OK)
   {
    // Surfaces could not be restored - lock fails
    break;
   }
  }
  else if (DDReturn != DDERR_WASSTILLDRAWING)
  {
   // Some other error happened - fail
   break;
  }
 }
 return ReturnValue;
}

// -----

int OffscreenUnlock(void)
{
 int ReturnValue = 0;
 // Loop until an error occurs or the unlock succeeds
 HRESULT DDReturn = DD_OK;
 while (1)
 {
  // Attempt the unlock
//  DDReturn = pOffscreenSurface->Unlock(0);
	IDirectDraw_Unlock(pOffscreenSurface,0);
  if (DDReturn == DD_OK)
  {
   // Unlock succeeds
   ReturnValue = 1;
   break;
  }
  else if (DDReturn == DDERR_SURFACELOST)
  {
   // Attempt to restore the surfaces
//   DDReturn = pPrimarySurface->Restore();
	 IDirectDraw_Restore(pPrimarySurface);
//   if (DDReturn == DD_OK) DDReturn = pOffscreenSurface->Restore();
   if (DDReturn == DD_OK) DDReturn = IDirectDraw_Restore(pOffscreenSurface);
   if (DDReturn != DD_OK)
   {
    // Surfaces could not be restored - unlock fails
    break;
   }
  }
  else if (DDReturn != DD_OK)
  {
   // Some other error happened - unlock fails
   break;
  }
 }
 return ReturnValue;
}

//****************************************************************************
// Buffer swapping

void SwapBuffer(void)
{
 if (PageFlip)
 {
  // We can use page flipping to draw the surface
//  pPrimarySurface->Flip(0, DDFLIP_WAIT);
	IDirectDraw_Flip(pPrimarySurface,0,DDFLIP_WAIT);
 }
 else
 {
  // We have to blt the offscreen image to the screen
  RECT BltRect;
  GetClientRect(Window, &BltRect);
//  pPrimarySurface->BltFast(0, 0,
	IDirectDraw_BltFast(pPrimarySurface,0,0,
   pOffscreenSurface, &BltRect, DDBLTFAST_WAIT);

 }
}

// -----

void SwapRect(int Left, int Top, int Right, int Bottom)
{
 // Blt the requested rectangle to the screen
 RECT BltRect = { Left, Top, Right, Bottom };
// pPrimarySurface->BltFast(Left, Top,
 IDirectDraw_BltFast(pPrimarySurface,Left,Top,
 pOffscreenSurface, &BltRect, DDBLTFAST_WAIT);
}

//****************************************************************************
// Full Screen initialization

int BeginFullScreen(int Width, int Height, int Depth, char *pal)
{
 HRESULT DDReturn;
 DDSURFACEDESC SurfaceDesc;
 PALETTEENTRY PaletteColors[256];
 int i;

 // Create a Width x Height popup window using the STATIC control
 // class so we don't have to implement a window procedure right now
 Window = CreateWindow("STATIC", "FullScreen", WS_POPUP, 0, 0, Width, Height, 0, 0, ghInstance, 0);
 if (!Window) goto Failure;
 ShowWindow(Window, SW_SHOWNORMAL);
 UpdateWindow(Window);
 // Connect to DirectDraw, if not already connected
 if (!pDirectDraw) DirectDrawCreate(0, &pDirectDraw, 0);
 if (!pDirectDraw) goto Failure;
 // Set up DirectDraw for full-screen exclusive mode at the
 // requested resolution
// DDReturn = pDirectDraw->SetCooperativeLevel(Window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
 DDReturn = IDirectDraw_SetCooperativeLevel(pDirectDraw,Window,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
 if (DDReturn != DD_OK) goto Failure;
// DDReturn = pDirectDraw->SetDisplayMode(Width, Height, Depth);
 DDReturn = IDirectDraw_SetDisplayMode(pDirectDraw,Width,Height,Depth);
 if (DDReturn != DD_OK) goto Failure;
 // Create the primary surface
 // Try to get a triple-buffered one we can page flip
 PageFlip = 1;
 ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
 SurfaceDesc.dwSize = sizeof(SurfaceDesc);
 SurfaceDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
 SurfaceDesc.dwBackBufferCount = 2;
 SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
// DDReturn = pDirectDraw->CreateSurface(&SurfaceDesc, &pPrimarySurface, 0);
 DDReturn = IDirectDraw_CreateSurface(pDirectDraw,&SurfaceDesc, &pPrimarySurface, 0);
 if (DDReturn != DD_OK)
 {
  // We couldn't get a triple buffer, try a double-buffer
  SurfaceDesc.dwBackBufferCount = 1;
//  DDReturn = pDirectDraw->CreateSurface(&SurfaceDesc, &pPrimarySurface, 0);
  DDReturn = IDirectDraw_CreateSurface(pDirectDraw,&SurfaceDesc, &pPrimarySurface, 0);
  if (DDReturn != DD_OK)
  {
   // We couldn't get a page-flip-able surface at all.
   PageFlip = 0;
   // Just go for a no-frills primary surface
   SurfaceDesc.dwFlags = DDSD_CAPS;
   SurfaceDesc.dwBackBufferCount = 0;
   SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
//   DDReturn = pDirectDraw->CreateSurface(&SurfaceDesc, &pPrimarySurface, 0);
   DDReturn = IDirectDraw_CreateSurface(pDirectDraw,&SurfaceDesc, &pPrimarySurface, 0);
   if (DDReturn != DD_OK) goto Failure;
  }
 }
 if (PageFlip)
 {
  // Get the attached page-flip-able surface as the
  // offscreen buffer
  DDSCAPS caps;
  caps.dwCaps = DDSCAPS_BACKBUFFER;
//  DDReturn = pPrimarySurface->GetAttachedSurface(&caps, &pOffscreenSurface);
   DDReturn = IDirectDraw_GetAttachedSurface(pPrimarySurface,&caps, &pOffscreenSurface);
 }
 else
 {
  // Create a second surface for the offscreen buffer
  ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
  SurfaceDesc.dwSize = sizeof(SurfaceDesc);
  SurfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  SurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  SurfaceDesc.dwWidth = Width;
  SurfaceDesc.dwHeight = Height;
//  DDReturn = pDirectDraw->CreateSurface(&SurfaceDesc, &pOffscreenSurface, 0);
  DDReturn = IDirectDraw_CreateSurface(pDirectDraw,&SurfaceDesc, &pOffscreenSurface, 0);
 }
 if (DDReturn != DD_OK) goto Failure;
 // Set up a palette - a grey wash in 0..255
 for (i=0; i<256; ++i)
 {
  PaletteColors[i].peRed   = pal[i*3];
  PaletteColors[i].peGreen = pal[i*3+1];
  PaletteColors[i].peBlue  = pal[i*3+2];
  PaletteColors[i].peFlags = PC_RESERVED;
 }
// DDReturn = pDirectDraw->CreatePalette(DDPCAPS_8BIT, PaletteColors, &pPalette, 0);
 DDReturn = IDirectDraw_CreatePalette(pDirectDraw,DDPCAPS_8BIT, PaletteColors, &pPalette, 0);

 if (DDReturn != DD_OK) goto Failure;
 // Attach the palette to the surface
// DDReturn = pPrimarySurface->SetPalette(pPalette);
 DDReturn = IDirectDraw_SetPalette(pPrimarySurface,pPalette);

 if (DDReturn != DD_OK) goto Failure;
 // Success!
 return 1;
Failure:
 return 0;
}

//****************************************************************************
// Full Screen clean-up

void EndFullScreen(void)
{
 // Set us back to the original mode
// if (pDirectDraw) pDirectDraw->RestoreDisplayMode();
 if (pDirectDraw) IDirectDraw_RestoreDisplayMode(pDirectDraw);

 if (Window)
 {
  DestroyWindow(Window);
  Window = 0;
 }
 if (pOffscreenSurface && !PageFlip)
 {
//  pOffscreenSurface->Release();
  IDirectDraw_Release(pOffscreenSurface);
  pOffscreenSurface = 0;
 }
 if (pPrimarySurface)
 {
//  pPrimarySurface->Release();
  IDirectDraw_Release(pPrimarySurface);
  pPrimarySurface = 0;
 }
 if (pPalette)
 {
//  pPalette->Release();
  IDirectDraw_Release(pPalette);
  pPalette = 0;
 }
 if (pDirectDraw)
 {
//	pDirectDraw->Release();
  IDirectDraw_Release(pDirectDraw);
  pDirectDraw = 0;
 }
}

// -----

BOOL Buffer_Init(int x_res, int y_res, char *palette)
{
 BeginFullScreen(x_res, y_res, 8, palette);
}

void Buffer_CleanUp();
void Buffer_Clear(char c);
WORD gethicolor(char a,char b,char c);

