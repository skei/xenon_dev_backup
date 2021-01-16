template <class T> class cBuffer
 {
 public:
  cBuffer ( int bpp );
  ~cBuffer();

 private:

 int bpp;
 T *Data;

 };

 // ---

template <class T> class cViewPort
 {
 public:
  // Initializes everything... Not setting mode
  cViewPort( int width, int height, int bpp );
  ~cViewPort();

  Init();   // set mode, etc...
  Exit();   // restore mode

  BlitBuffer();                   // Blit buffer to screen
  FillBuffer( T value );          // Fill buffer with value
  FillBufferBitMap( T *BitMap );  // Fill buffer with bitmap

 private:
  int OldMode;
  T *ScreenPtr;
  cBuffer Buffer;
 };

 // ---

void main(void)
{
 cViewPort <char> SCREEN(320,200,8);
}
