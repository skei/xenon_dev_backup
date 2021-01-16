 #ifndef map_hpp_included
 #define map_hpp_included
 // -------------------------------------------------------------------------

 // Generic MAP class - bitmaps, buffers, screen...

class c_MAP
{

// ---
public:
// ---

c_MAP();
c_MAP(int w,int h,int b);
c_MAP(int w,int h,int b, void *ptr);
~c_MAP();

void Clear(void);
void BlitTo(c_MAP &dest);
inline void *GetPtr(void) { return m_map; }

// ---
private:
// ---

int m_width;
int m_height;
int m_bpp;
int m_mapsize;
void *m_map;

};

 // -------------------------------------------------------------------------
 #endif
