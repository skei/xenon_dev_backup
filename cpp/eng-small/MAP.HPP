 #ifndef map_hpp_included
 #define map_hpp_included
 // -------------------------------------------------------------------------

class c_MAP
 {

 public:

  c_MAP();
  c_MAP(int w,int h,int b);
  c_MAP(int w,int h,int b, void *ptr);
  ~c_MAP();

  void Clear(void);
  void BlitTo(c_MAP &dest);
  void *GetPtr(void);

 private:

  int width;
  int height;
  int bpp;
  int map_size;

  void *map;
 };

 // -------------------------------------------------------------------------
 #endif
