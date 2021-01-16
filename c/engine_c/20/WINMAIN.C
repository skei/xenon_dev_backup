#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "types.h"

extern void main_init(void);
extern void main_process(void);
extern void main_exit(void);

#pragma warning (disable: 4068)
#pragma warning (disable: 4100)
#pragma argsused

#if !defined(__WIN32__) && !defined(_WIN32)
#define EXPORT16 __export
#else
#define EXPORT16
#endif

char szAppName[] = "Engine - Tor-Helge Skei";
static HANDLE hResource;
static HINSTANCE hInstance;
BITMAPINFO BitmapInfo;			
static HWND MainWindow;

LRESULT CALLBACK EXPORT16 WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam); 

//===================
// PROTOTYPES
//===================

BOOL Register(HINSTANCE hInst);
HWND Create(HINSTANCE hInst, int nCmdShow);

screen_S buffer;
unsigned short int colortable[256];			// To simulate a 256-color mode...
								// use *p = colortable[color] instead of *p = color

// ------------------

/*****************/
/* Windows stuff */
/*****************/

// -----

// in: r,g,b (0..255)... Out: Word 15 bit color..
WORD gethicolor(char a,char b,char c)
{
 WORD r;
 r = (WORD)(((a&0xf8)<<7) + ((b&0xf8)<<2) + (c>>3));
 return r;
}

// -----

// Initialize & allocate buffer, init BitMapInfo
BOOL Buffer_Init(int x_res, int y_res, char *palette)
{
 int i;
 x_res = (x_res/2)*2;	// To fix modulo bug
 buffer.screen = (unsigned short int *)malloc(x_res*y_res*2);
 if(buffer.screen != NULL)
 {
  buffer.width = x_res;
  buffer.height = y_res;

  BitmapInfo.bmiHeader.biSize          = sizeof(BitmapInfo.bmiHeader);	// Size of struct
  BitmapInfo.bmiHeader.biWidth         = buffer.width;					// Width of bitmap
  BitmapInfo.bmiHeader.biHeight        = -buffer.height;				// Height of bitmap. Negative means top-down
  BitmapInfo.bmiHeader.biPlanes        = 1;								// Must be 1
  BitmapInfo.bmiHeader.biBitCount      = 16;							// Number of bits per pixel
  BitmapInfo.bmiHeader.biCompression   = BI_RGB;						// Uncompressed
  BitmapInfo.bmiHeader.biSizeImage     = 0;								// 0 for BI_RGB bitmaps
  BitmapInfo.bmiHeader.biXPelsPerMeter = 2000;	// ????					// Pixels per meter	horizontal
  BitmapInfo.bmiHeader.biYPelsPerMeter = 1000;	// ????					// Pixels per meter vertical
  BitmapInfo.bmiHeader.biClrUsed       = 0;								// Number of colors used in bitmap
  BitmapInfo.bmiHeader.biClrImportant  = 0;								// Important colors
// Calc colortable (for 8->16 bit conversion)
  for (i=0;i<768;i++) palette[i] <<=2;
  for (i=0;i<256;i++) colortable[i] = gethicolor(palette[i*3],palette[i*3+1],palette[i*3+2]);
//set palette
/*
	for (i=0;i<255;i++)
	{
	 BitmapInfo.bmiColors[i].rgbRed   = palette[i*3  ];
	 BitmapInfo.bmiColors[i].rgbGreen = palette[i*3+1];
	 BitmapInfo.bmiColors[i].rgbBlue  = palette[i*3+2];
	}
*/
	return TRUE;
 }
 else
 {
  MessageBox(NULL, "ERROR: Unable to allocate memory for viewport!", "ERROR", MB_OK);
  return FALSE;	
 }
}

// Free buffer & deallocate memory

void Buffer_CleanUp()
{
 free(buffer.screen);		 /* Her.... Exception: Access violation */ 
 buffer.screen=NULL;
 buffer.width=0;
 buffer.height=0;
}

// -----

void Buffer_Clear(char c)
{
 screen_S *vp = &buffer;
 memset(vp->screen,c,(SCREEN_WIDTH*SCREEN_HEIGHT*2)); 
}

// -----

BOOL transfer_to_window(HDC PaintDC, int win_xsize, int win_ysize)
{
 HDC BitmapDC;
 BITMAP BitData;
 HBITMAP OldBitmap;
 HBITMAP Bitmap;
 int debug_temp;
 char debug_str[100];

 if(buffer.width && buffer.height)
 {
  Bitmap = CreateCompatibleBitmap(PaintDC, buffer.width, buffer.height);	// Create a temorary compatible bitmap	
  debug_temp=SetDIBits(PaintDC, Bitmap, 0, buffer.height, buffer.screen, &BitmapInfo, DIB_RGB_COLORS);
  if(!debug_temp)
  {
// sprintf(debug_str, "ERROR: Not able to transfer rendered bitmap to DIB (SetDIBits). Error code %i\n",GetLastError());
   MessageBox(NULL, debug_str, "ERROR", MB_OK);
  }
  BitmapDC = CreateCompatibleDC(PaintDC);					// Create a temorary compatible DC
  OldBitmap = SelectBitmap(BitmapDC, Bitmap);				// Select the roc bitmap into this DC
  GetObject(Bitmap, sizeof(BitData), &BitData);				// Get logical information about the roc bitmap
  
// THS: HVis vi setter buffer-størrelsen til 9*64K (768*768), og setter opp StretchBlt til å bruke
// bare det midterste rektanglet, får vi clipping gratis!
// Det som må forandres er:
// BitmapDC, 0,0, BitData.bmWidth, BitData.bmHeight, SRCCOPY) til
// BitmapDC, 256,256, 256, 256, SRCCOPY) x1,y1,width,height
// pluss setting av buffer til 768x768

	StretchBlt(PaintDC, 0,0, win_xsize, win_ysize,			// Blit it onscreen	
  BitmapDC, 0,0, BitData.bmWidth, BitData.bmHeight, SRCCOPY);
  SelectBitmap(BitmapDC, OldBitmap);						// Restore DC
  DeleteDC(BitmapDC);										// Delete temporary DC	
  DeleteBitmap(Bitmap);										// Delete temporary bitmap
  return TRUE;
 }
 else return FALSE;											// Has not been initialised!
}

//===================
// MAIN
//===================

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
 MSG Msg;
 hInstance=hInst;

 if(!hPrevInstance)
  if(!Register(hInst))
   return FALSE;

 main_init();

 MainWindow = Create(hInst, nCmdShow);
 if(!MainWindow)
  return FALSE;

 while( TRUE )
 {
  /* Get any message from the system	*/
  if( PeekMessage(&Msg,NULL,0,0,PM_REMOVE) )
  {
   /* If WM_QUIT is recieved we quit the application */
   if( Msg.message == WM_QUIT )
   {
    break;
   }
   /* Translate and dispatch the message */
   TranslateMessage(&Msg);
   DispatchMessage(&Msg);
  }
  else
  {
   main_process();
   RedrawWindow(MainWindow, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);

  }
 }
 main_exit();
} 
 
// while(GetMessage(&Msg, NULL, 0, 0))
// {
//  TranslateMessage(&Msg);
//  DispatchMessage(&Msg);
//}
//return Msg.wParam;
//}


//------------------------
// Register the window
//------------------------
BOOL Register(HINSTANCE hInst) {
	WNDCLASS WndClass;

	WndClass.style			= CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc	= WndProc;
	WndClass.cbClsExtra		= 0;
	WndClass.cbWndExtra		= 0;
	WndClass.hInstance		= hInst;
	WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	WndClass.lpszMenuName	= NULL;/*MAKEINTRESOURCE(IDR_MENU1);*/
	WndClass.lpszClassName	= szAppName;

	return (RegisterClass(&WndClass) != 0);
}


//-------------------------
// Create the window
//-------------------------

HWND Create(HINSTANCE hInstance, int nCmdShow) {
	HWND hWindow = CreateWindowEx(0, szAppName, szAppName,
									WS_OVERLAPPEDWINDOW,
									10,10,										 // X and Y starting position
									SCREEN_WIDTH,SCREEN_HEIGHT,// WIDTH and HEIGHT of window
									NULL, NULL, hInstance, NULL);

	if(hWindow == NULL)
		return hWindow;

	ShowWindow(hWindow, nCmdShow);
	UpdateWindow(hWindow);

	return hWindow;
}


//---------------------------------
// Prototypes for message handlers 
//---------------------------------
#define Window1_DefProc DefWindowProc

BOOL Window1_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void Window1_OnDestroy(HWND hwnd);
void Window1_OnPaint(HWND hwnd);
void Window1_OnChar(HWND hwnd, TCHAR ch, int cRepeat);

// Other prototypes

// BOOL CALLBACK QuitBoxProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);
// BOOL CALLBACK AboutBoxProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam);

//------------------------------------------------------------
// The window proc: This is where the messages are processed
//------------------------------------------------------------

LRESULT CALLBACK EXPORT16 WndProc(HWND hWindow, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		HANDLE_MSG(hWindow, WM_CREATE, Window1_OnCreate);		// Constructor
		HANDLE_MSG(hWindow, WM_DESTROY, Window1_OnDestroy);		// Destructor
		HANDLE_MSG(hWindow, WM_PAINT, Window1_OnPaint);			// Refresh
		HANDLE_MSG(hWindow, WM_CHAR, Window1_OnChar);
		default:
			return Window1_DefProc(hWindow, Message, wParam, lParam);
	}
}

//--------------------
// Messages handlers
//--------------------
#pragma argsused

// WM_CREATE
BOOL Window1_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
 return TRUE;
}

// WM_DESTROY
void Window1_OnDestroy(HWND hwnd)
{
 PostQuitMessage(0);
}

// WM_PAINT
void Window1_OnPaint(HWND hwnd)
{
 PAINTSTRUCT PaintStruct;
 RECT TempRect;
 HDC PaintDC = BeginPaint(hwnd, &PaintStruct);
 GetClientRect(hwnd, &TempRect);
// MessageBox(NULL,"Inside OnPaint","DEBUG",MB_OK);
 transfer_to_window(PaintDC, TempRect.right, TempRect.bottom);
 EndPaint(hwnd, &PaintStruct);
}

// WM_CHAR
void Window1_OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
// CharLowerBuff(&ch,1);
 switch(ch)
 {
 }
}
