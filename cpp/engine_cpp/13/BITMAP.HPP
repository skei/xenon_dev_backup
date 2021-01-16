 #ifndef bitmap_hpp_included
 #define bitmap_hpp_included
 // -------------------------------------------------------------------------

class c_BITMAP
 {

 public:

  c_BITMAP();
  c_BITMAP(int w,int h,int b);
  c_BITMAP(int w,int h,int b, void *ptr);
  ~c_BITMAP();

  void Clear(void);
  void BlitTo(c_BITMAP &dest);
  void *GetPtr(void);

 private:

  int width;
  int height;
  int bpp;
  int bitmap_size;

  void *bitmap;
 };

 // -------------------------------------------------------------------------
 #endif
