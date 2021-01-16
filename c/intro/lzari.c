 /* Credits for lzari go to Haruhiko Okumura */
 // Modified by Axon/Inf^Xenon
 // Only unpacking stuff here, for inclusion in intro/demo

//
// --- defines ---
//

#define _EOF_    -1
#define N         4096 /* size of ring buffer */
#define F_SS      18   /* upper limit for match_length */
#define F_ARI     60
#define THRESHOLD 2    /* encode string into position and length
                          if match_length is greater than this */
#define NIL       N    /* index for root of binary search trees */

 #define M   15
 // Q1 (= 2 to the M) must be sufficiently large, but not so
 // large as the unsigned long 4 * Q1 * (Q1 - 1) overflows.
 #define Q1  (1UL << M)
 #define Q2  (2 * Q1)
 #define Q3  (3 * Q1)
 #define Q4  (4 * Q1)
 #define MAX_CUM (Q1 - 1)
 #define N_CHAR  (256 - THRESHOLD + F_ARI)
 // character code = 0, 1, ..., N_CHAR - 1

//
// --- variables ---
//

unsigned char    *from_buffer;
long              from_pointer;
long              from_max;
unsigned char*    to_buffer;
long              to_pointer;
//long              to_max;
unsigned long int textsize;
unsigned long int codesize;
//int               match_position;
//int               match_length;
//int lson[N + 1];
//int rson[N + 257];
//int dad[N + 1];
unsigned char     text_buf[N + F_ARI - 1];
unsigned int      _buffer_, _mask_ = 0;

unsigned long int low = 0, high = Q4, value = 0;
//int               shifts = 0;  // counts for magnifying low and high around Q2
int               char_to_sym[N_CHAR];
int               sym_to_char[N_CHAR+1];
unsigned int      sym_freq    [N_CHAR + 1];  // frequency for symbols
unsigned int      sym_cum     [N_CHAR + 1];  // cumulative freq for symbols
unsigned int      position_cum[N + 1     ];  // cumulative freq for positions


//
// --- functions ---
//

void lz_init(void)
 {
  to_pointer = from_pointer = codesize = textsize = 0;
  _buffer_ = _mask_ = 0;
  low = 0;
  high = Q4;
  value = 0;
 }

 // ---

void lz_exit(void)
 {
 }

 // ---

int getc_buffer(void)
 {
  if (from_pointer == from_max) return(_EOF_);
  return(from_buffer[from_pointer++]);
 }

 // ---

int putc_buffer(int invoer)
 {
  to_buffer[to_pointer++] = (unsigned char)invoer;
  return(1);
 }

 // ---

int GetBit(void)  /* Get one bit (0 or 1) */
 {
  if ((_mask_ >>= 1) == 0)
  {
   _buffer_ = getc_buffer();
   _mask_ = 128;
	}
  return ((_buffer_ & _mask_) != 0);
 }

//
// --- Arithmetic Compression ---
//

 // ---

 // Initialize model
void StartModel(void)
 {
	int ch, sym, i;

	sym_cum[N_CHAR] = 0;
  for (sym=N_CHAR;sym>=1;sym--)
  {
   ch = sym - 1;
   char_to_sym[ch] = sym;
   sym_to_char[sym] = ch;
   sym_freq[sym] = 1;
   sym_cum[sym-1] = sym_cum[sym] + sym_freq[sym];
	}
  sym_freq[0] = 0;      // sentinel (!= sym_freq[1])
	position_cum[N] = 0;
  for (i=N;i>=1;i--)
  {
   position_cum[i-1] = position_cum[i] + 10000 / (i + 200);
   // empirical distribution function (quite tentative)
   // Please devise a better mechanism!
  }
 }

 // ---

void UpdateModel(int sym)
 {
	int i, c, ch_i, ch_sym;

  if (sym_cum[0] >= MAX_CUM)
  {
   c = 0;
   for (i=N_CHAR;i>0;i--)
   {
    sym_cum[i] = c;
    c += (sym_freq[i] = (sym_freq[i] + 1) >> 1);
   }
   sym_cum[0] = c;
	}
  for (i=sym; sym_freq[i] == sym_freq[i - 1]; i--) ;
  if (i < sym)
  {
   ch_i = sym_to_char[i];
   ch_sym = sym_to_char[sym];
   sym_to_char[i] = ch_sym;
   sym_to_char[sym] = ch_i;
   char_to_sym[ch_i] = sym;
   char_to_sym[ch_sym] = i;
	}
	sym_freq[i]++;
	while (--i >= 0) sym_cum[i]++;
 }

 // ---

int BinarySearchSym(unsigned int x)
 // 1      if x >= sym_cum[1],
 // N_CHAR if sym_cum[N_CHAR] > x,
 // i such that sym_cum[i - 1] > x >= sym_cum[i] otherwise
 {
	int i, j, k;

  i = 1;
  j = N_CHAR;
  while (i < j)
  {
   k = (i + j) / 2;
   if (sym_cum[k] > x) i = k + 1;
   else j = k;
	}
	return i;
 }

 // ---

int BinarySearchPos(unsigned int x)
  // 0 if x >= position_cum[1],
  // N - 1 if position_cum[N] > x,
  // i such that position_cum[i] > x >= position_cum[i + 1] otherwise
 {
	int i, j, k;

  i = 1;
  j = N;
  while (i<j)
  {
   k = (i+j)/2;
   if (position_cum[k]>x) i=k+1;
   else j = k;
	}
	return i - 1;
 }

 // -------------------------------------------------------------------------

void StartDecode(void)
 {
  int i;

  for (i = 0; i < M + 2; i++) value = 2 * value + GetBit();
 }

 // -------------------------------------------------------------------------

int DecodeChar(void)
 {
  int sym, ch;
  unsigned long int range;

	range = high - low;
  sym = BinarySearchSym((unsigned int)(((value - low + 1) * sym_cum[0] - 1)
                        / range));
	high = low + (range * sym_cum[sym - 1]) / sym_cum[0];
	low +=       (range * sym_cum[sym    ]) / sym_cum[0];
  for (;;)
  {
   if (low >= Q2)
   {
    value -= Q2;
    low -= Q2;
    high -= Q2;
   }
   else if (low >= Q1 && high <= Q3)
   {
    value -= Q1;
    low -= Q1;
    high -= Q1;
   }
   else if (high > Q2) break;
   low += low;
   high += high;
   value = 2 * value + GetBit();
	}
	ch = sym_to_char[sym];
	UpdateModel(sym);
	return ch;
 }

 // ---

int DecodePosition(void)
 {
	int position;
	unsigned long int  range;

	range = high - low;
  position = BinarySearchPos((unsigned int)(((value - low + 1)
                             * position_cum[0] - 1) / range));
	high = low + (range * position_cum[position    ]) / position_cum[0];
	low +=       (range * position_cum[position + 1]) / position_cum[0];
  for (;;)
  {
   if (low >= Q2)
   {
    value -= Q2;
    low -= Q2;
    high -= Q2;
    }
    else if (low >= Q1 && high <= Q3)
    {
     value -= Q1;
     low -= Q1;
     high -= Q1;
    }
    else if (high > Q2) break;
    low += low;
    high += high;
		value = 2 * value + GetBit();
	}
	return position;
 }

//
// --- Decode ---
//

void lzari_Decode(void)
 {
	int  i, j, k, r, c;
	unsigned long int  count;

	textsize = 0;
	textsize |= getc_buffer() << 24;
	textsize |= getc_buffer() << 16;
	textsize |= getc_buffer() << 8;
	textsize |= getc_buffer();

	if (textsize == 0) return;
  StartDecode();
  StartModel();
  for (i = 0;i<N-F_ARI;i++) text_buf[i] = ' ';
	r = N - F_ARI;
  for (count = 0; count < textsize; )
  {
   c = DecodeChar();
   if (c < 256)
   {
    putc_buffer(c);
    text_buf[r++] = c;
    r &= (N - 1);
    count++;
   }
   else
   {
    i = (r - DecodePosition() - 1) & (N - 1);
    j = c - 255 + THRESHOLD;
    for (k = 0; k < j; k++)
    {
     c = text_buf[(i + k) & (N - 1)];
     putc_buffer(c);
     text_buf[r++] = c;
     r &= (N - 1);
     count++;
    }
   }
  }
 }

//
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
//

 // fbuffer = compressed data
 // tbuffer = uncompressed data
 // comp_size = compressed size
void lzari_decompress(unsigned char* fbuffer, unsigned char* tbuffer, long comp_size)
 {
  lz_init();
  from_buffer = fbuffer;
  from_max = comp_size;
  to_buffer = tbuffer;
  lzari_Decode();
  lz_exit();
 }

