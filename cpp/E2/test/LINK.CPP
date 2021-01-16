#include <stdlib.h>

 // ---

class LList
 {
 // -------
    public:
 // -------

  LList()
   {
    Head = NULL;
    Tail = NULL;
    Current = NULL;
   };

  // -

  ~LList() {};

  Add(

 // --------
    private:
 // --------

  struct Node
  {
   LList *object;
   Node *Next;
  };

  // -

  Node *Head;
  Node *Tail;
  Node *Current;

 };
