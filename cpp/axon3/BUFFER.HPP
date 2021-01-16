template <class T>
class BUFFER
 {
  public:

   BUFFER(int width = 320, int height = 200, int bpp = 8)
   {
    _width = width;
    _height = height;
    _bpp = bpp;

   ~BUFFER();
  private:
   int _width,_height,_bpp;
   T *_buffer;
 };

