
//
// ÄÄÄÄÄ Variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 CENTERLIST **centers; // cube center hash table
 CORNERLIST **corners; // corner value hash table
 EDGELIST   **edges;   // edge and vertex id hash table



// -------------------------------------------------------------------------
// sphere: an inverse square function (always positive)
// -------------------------------------------------------------------------

double sphere (double x, double y, double z)
 {
  double rsq;
  rsq = x*x + y*y + z*z;
  return 1.0/(  rsq < 0.00001 ? 0.00001 : rsq  );
 }

// -------------------------------------------------------------------------
// blob: a three-pole blend function, try size = .1
// -------------------------------------------------------------------------

double blob (double x, double y, double z)
 {
  return 4.0-sphere(x+1.0,y,z)-sphere(x,y+1.0,z)-sphere(x,y,z+1.0);
 }

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------

char   *polygonize (float size, int bounds, float x, float y, float z)
 {
  float delta;

  delta = size/(double)(RES*RES);

  // find point on surface, beginning search at (x, y, z):
  // search for one point inside & one point outside, interpolate between
  // to find a point ON surface
  // DENNE SUGER... HER M VI FINNE P NOE ANNET!!!
  //srand(1);
  //in = find(1, x, y, z);
  //out = find(0, x, y, z);
  //if (!in.ok || !out.ok) return "can't find starting point";
  //converge(&in.p, &out.p, in.value, p.function, &p.start);
  // ---
  // push initial cube on stack:
  // p.cubes = (CUBES *) mycalloc(1, sizeof(CUBES));           // list of 1

  p.cubes = &(global_CUBES[current_CUBES++]);
  if (current_CUBES >= max_CUBES) error ("global_CUBES full\n");

  p.cubes->cube.i = p.cubes->cube.j = p.cubes->cube.k = 0;
  p.cubes->next = NULL;

  // set corners of initial cube:
  for (n = 0; n < 8; n++)
  {
   p.cubes->cube.corners[n] = setcorner(&p, BIT(n,2), BIT(n,1), BIT(n,0));
  }

  p.vertices.count = p.vertices.max = 0;  // no vertices yet
  p.vertices.ptr = NULL;
  setcenter(p.centers, 0, 0, 0);

  // process active cubes till none left
  while (p.cubes != NULL)
  {
   CUBE c;
   CUBES *temp = p.cubes;
   c = p.cubes->cube;

   noabort =  // mode == TET?
    // either decompose into tetrahedra and polygonize:
              //dotet(&c, LBN, LTN, RBN, LBF, &p) &&
              //dotet(&c, RTN, LTN, LBF, RBN, &p) &&
              //dotet(&c, RTN, LTN, LTF, LBF, &p) &&
              //dotet(&c, RTN, RBN, LBF, RBF, &p) &&
              //dotet(&c, RTN, LBF, LTF, RBF, &p) &&
              //dotet(&c, RTN, LTF, RTF, RBF, &p)
              //:
    // or polygonize the cube directly:
    docube(&c, &p);
	if (! noabort) return "aborted";
  // pop current cube from stack
	p.cubes = p.cubes->next;
	free((char *) temp);
  // test six face directions, maybe add to stack:
	testface(c.i-1, c.j, c.k, &c, L, LBN, LBF, LTN, LTF, &p);
	testface(c.i+1, c.j, c.k, &c, R, RBN, RBF, RTN, RTF, &p);
	testface(c.i, c.j-1, c.k, &c, B, LBN, LBF, RBN, RBF, &p);
	testface(c.i, c.j+1, c.k, &c, T, LTN, LTF, RTN, RTF, &p);
	testface(c.i, c.j, c.k-1, &c, N, LBN, LTN, RBN, RTN, &p);
	testface(c.i, c.j, c.k+1, &c, F, LBF, LTF, RBF, RTF, &p);
  }
  return NULL;
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ±±±±± MAIN ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void main(void)
 {
  // allocate hash tables and build cube polygon table:
  centers = (CENTERLIST **)mycalloc(HASHSIZE,sizeof(CENTERLIST *));
  corners = (CORNERLIST **)mycalloc(HASHSIZE,sizeof(CORNERLIST *));
  edges   = (EDGELIST   **)mycalloc(2*HASHSIZE,sizeof(EDGELIST *));
  makecubetable();

