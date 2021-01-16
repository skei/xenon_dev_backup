 // UTIL for † kalkulere de to j‘vlige tabellene i meta.c
 // basert p† en PostScript fil av Jules Bloomenthal
 // (endel modifisert, s† man f†r clockwise polys) etc... Og s† tabellene
 // passer inn til rutinene mine...

 /*
    3---7
   /|  /|
  2---6 |
  | | | |
  | 1-|-5
  |/  |/
  0---4

 */

 // set bit if inside of blob

 #define L   0      // left direction: -x, -i
 #define R   1      // right direction:  +x, +i
 #define B   2      // bottom direction: -y, -j
 #define T   3      // top direction:  +y, +j
 #define N   4      // near direction: -z, -k
 #define F   5      // far direction:  +z, +k

 #define LB  0  // left bottom edge
 #define LT  1  // left top edge
 #define LN  2  // left near edge
 #define LF  3  // left far edge
 #define RB  4  // right bottom edge
 #define RT  5  // right top edge
 #define RN  6  // right near edge
 #define RF  7  // right far edge
 #define BN  8  // bottom near edge
 #define BF  9  // bottom far edge
 #define TN  10 // top near edge
 #define TF  11 // top far edge
 #define LBN 0      // left bottom near corner
 #define LBF 1      // left bottom far corner
 #define LTN 2      // left top near corner
 #define LTF 3      // left top far corner
 #define RBN 4      // right bottom near corner
 #define RBF 5      // right bottom far corner
 #define RTN 6      // right top near corner
 #define RTF 7      // right top far corner

 #define BIT(i, bit) (((i)>>(bit))&1)

typedef struct intlist    // list of integers
 {
  int i;                    // an integer
  struct intlist *next;     // remaining elements
 } INTLIST;

typedef struct intlists   // list of list of integers
 {
  INTLIST *list;            // a list of integers
  struct intlists *next;    // remaining elements
 } INTLISTS;

 INTLISTS *cubetable[256];
 int corner1[12]   = {LBN,LTN,LBN,LBF,RBN,RTN,RBN,RBF,LBN,LBF,LTN,LTF};
 int corner2[12]   = {LBF,LTF,LTN,LTF,RBF,RTF,RTN,RTF,RBN,RBF,RTN,RTF};
 int leftface[12]  = {B,  L,  L,  F,  R,  T,  N,  R,  N,  B,  T,  F};
 int rightface[12] = {L,  T,  N,  L,  B,  R,  R,  F,  B,  F,  N,  T};


// --- Make cube table ------------------------------------------------------

 // nextcwedge: return next clockwise edge from given edge around given face
 // used in: makecubetable
int     nextcwedge (int edge, int face)
 {
  switch (edge)
  {
   case LB: return (face == L)? LF : BN;
   case LT: return (face == L)? LN : TF;
   case LN: return (face == L)? LB : TN;
   case LF: return (face == L)? LT : BF;
   case RB: return (face == R)? RN : BF;
   case RT: return (face == R)? RF : TN;
   case RN: return (face == R)? RT : BN;
   case RF: return (face == R)? RB : TF;
   case BN: return (face == B)? RB : LN;
   case BF: return (face == B)? LB : RF;
   case TN: return (face == T)? LT : RN;
   case TF: return (face == T)? RT : LF;
  }
  return 0;
 }

 // otherface: return face adjoining edge that is not the given face
 // used in: makecubetable
int     otherface (int edge, int face)
 {
  int other = leftface[edge];
  return face == other ? rightface[edge] : other;
 }

 // ---

 // makecubetable: create the 256 entry table for cubical polygonization
void    makecubetable (void)
 {
  int i, e, c, done[12], pos[8];

  for (i = 0; i < 256; i++)
  {
   for (e = 0; e < 12; e++) done[e] = 0;
   for (c = 0; c < 8; c++) pos[c] = BIT(i, c);
   for (e = 0; e < 12; e++)
   {
    if (!done[e] && (pos[corner1[e]] != pos[corner2[e]]))
    {
     INTLIST *ints = 0;
     INTLISTS *lists = (INTLISTS *) malloc(1, sizeof(INTLISTS));
     int start = e, edge = e;
     // get face that is to right of edge from pos to neg corner:
     int face = pos[corner1[e]] ? rightface[e] : leftface[e];
     while (1)
     {
      edge = nextcwedge(edge, face);
      done[edge] = 1;
      if (pos[corner1[edge]] != pos[corner2[edge]])
      {
       INTLIST *tmp = ints;
       ints = (INTLIST *) malloc(1, sizeof(INTLIST));
       ints->i = edge;
       ints->next = tmp;              // add edge to head of list
       if (edge == start) break;
       face = otherface(edge, face);
      }
     }
     lists->list = ints;              // add ints to head of table entry
     lists->next = cubetable[i];
     cubetable[i] = lists;
    }
   }
  }
 }

void main(void)
 {
  int i,j,k;
  INTLISTS *l;
  INTLIST  *list;
  unsigned int number;
  int len;

  makecubetable();

  for (i=0;i<256;i++)
  {
   len = 0;
   printf("  {");
   l = cubetable[i];
   // g† gjennom alle lister i cubetable
   if (l==0) printf(" 0,");
   while (l != 0)
   {
    // tell antall f›rst
    list = l->list;
    number = 0;
    while (list != 0) { number++; list=list->next; }
    len += number;
    len++;
    printf(" %i",number);
    list = l->list;
    while (list != 0)
    {
     if (list->i > 9) printf(",%i",list->i);
     else printf(", %i",list->i);
     list=list->next;
    }
    printf(",");
    l=l->next;
   }
   for (j=len;j<16;j++) printf("-1,");
   printf("},\n");
  }

  printf("---------\n");

  for (k=0;k<32;k++)
  {
   for (j=0;j<8;j++)
   {
    i = (k*8) + j;
    l = cubetable[i];
    number = 0;
    while (l != 0)
    {
     list = l->list;
     while (list != 0)
     {
      number |= (1 << list->i );   // edge number
      list=list->next;
     }
     l=l->next;
    }
    printf("0x%03x,",number);
   }
   printf("\n");
  }
  printf("\n");
 }

