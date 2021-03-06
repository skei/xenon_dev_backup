#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// 컴컴� define 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

// sWindow.flags
#define WinFlag_Focus 1
#define WinFlag_Hidden 2

// sWindow.type
#define WinType_CloseButton 1
#define WinType_ResizeButton 2
#define WinType_TitleBar 4

// 컴컴� typedef 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct
{
 int ID;
 int flags;
 int type;
 int x1,y1,x2,y2;
 unsigned char *DrawBuffer;
} sWindow;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
