 #define HASHBIT     (5)
 #define HASHSIZE    (size_t)(1<<(3*HASHBIT)) // hash table size (32768)
 #define MASK        ((1<<HASHBIT)-1)
 #define HASH(i,j,k) ((((((i)&MASK)<<HASHBIT)|((j)&MASK))<<HASHBIT)|((k)&MASK))

 kube = 0 - 256

 p.corners = (CORNERLIST **)mycalloc(HASHSIZE,sizeof(CORNERLIST *));
 // ijk = int's :-/
 int index = HASH(i, j, k);

