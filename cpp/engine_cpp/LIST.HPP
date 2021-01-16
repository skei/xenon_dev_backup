 #ifndef list_hpp_included
 #define list_hpp_included

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 // #include "misc.h"
 #include <iostream.h>

//
// --- Class definitions ---
//

 // list node
template <class T> struct ListNode
 {
  T *object;
  ListNode<T> *next;
  ListNode<T> *prev;
 };

 // ---

 /*

 // list iterator
 template <class T> class ListIterator
 {
  public:
   // default constructor
   inline ListIterator();
   // navigation
   inline T* next();
   inline T* prev();
   // current object
   inline T* current() const;
   // operations on current object
   inline int replace(T const &object);
   inline int replace(T *object);
   inline int insert(T const & object);
   inline int insert(T *object);
   inline int remove();
   inline int free();
  public:
   // current node
   ListNode<T> *CurrentNode;
  private:
   // null node
   ListNode<T> NullNode;
 };

 */

 // ---

 // linked list
template <class T> class List
 {
  public:
   // list iterator
    // typedef ListIterator<T> Iterator;
   // setup
   inline List();
   inline ~List();
   // management
   inline int  add(T const &object);
   inline int  add(T *object);
   inline int  replace(T *object,T *replacement);
   inline int  remove(T *object);
   inline void remove();
   inline int  free(T *object);
   inline void free();
   // iterator
    // inline ListIterator<T> first();
    // inline ListIterator<T> last();
   // list merger
    // inline void merge(List<T> &other);
   // node access
   inline T*   head() const;
   inline T*   tail() const;
   inline T*   num(int i);      // axon
   // information
   inline int  size() const;
   // operators
    // inline void operator =(List<T> &other);
  //private:
  public:
   // root node
   ListNode<T> Root;
 };

//
// --- mehods ---
//

 /*

 template <class T> inline ListIterator<T>::ListIterator()
 {
  // setup null node
  NullNode.next=&NullNode;
  NullNode.prev=&NullNode;
  NullNode.object=NULL;
  // setup defaults
  CurrentNode=&NullNode;
 }

 template <class T> inline T* ListIterator<T>::next()
 {
  // next node
  CurrentNode=CurrentNode->next;
  return CurrentNode->object;
 }

 template <class T> inline T* ListIterator<T>::prev()
 {
  // previous node
  CurrentNode=CurrentNode->prev;
  return CurrentNode->object;
 }

 template <class T> inline T* ListIterator<T>::current() const
 {
  // return current object
  return CurrentNode->object;
 }

 template <class T> inline int ListIterator<T>::replace(T const &object)
 {
  // allocate store
  T *store=new T;
  if (!store) return 0;
  // replace current with store
  *store=object;
  return replace(store);
 }

 template <class T> inline int ListIterator<T>::replace(T *object)
 {
  // replace current object
  if (CurrentNode)
  {
   delete CurrentNode->object;
   CurrentNode->object=object;
   return 1;
  }
  else return 0;
 }

 template <class T> inline int ListIterator<T>::insert(T const &object)
 {
  // allocate store
  T *store=new T;
  if (!store) return 0;
  // replace current with store
  *store=object;
  return insert(store);
 }

 template <class T> inline int ListIterator<T>::insert(T *object)
 {
  // fail on null object
  if (!object) return 0;
  // create new node
  ListNode<T> *node=new ListNode<T>;
  if (!node) return 0;
  // setup node
  node->prev=CurrentNode->prev;
  node->next=CurrentNode;
  // insert node
  CurrentNode->prev->next=node;
  CurrentNode->prev=node;
  // update current node
  CurrentNode=node;
  return 1;
 }

 template <class T> inline int ListIterator<T>::remove()
 {
  // remove current node from list
  if (CurrentNode->object)
  {
   ListNode<T> *next=CurrentNode->next;
   ListNode<T> *prev=CurrentNode->prev;
   delete CurrentNode;
   next->prev=prev;
   prev->next=next;
   CurrentNode=prev;
   return 1;
  }
  else return 0;
 }

 template <class T> inline int ListIterator<T>::free()
 {
  // remove current node from list
  if (CurrentNode->object)
  {
   ListNode<T> *next=CurrentNode->next;
   ListNode<T> *prev=CurrentNode->prev;
   delete CurrentNode->object;
   delete CurrentNode;
   next->prev=prev;
   prev->next=next;
   CurrentNode=prev;
   return 1;
  }
  else return 0;
 }

 */

 // ---

template <class T> inline List<T>::List()
 {
  // setup root node
  Root.next=&Root;
  Root.prev=&Root;
  Root.object=NULL;
 }

template <class T> inline List<T>::~List()
 {
  // free list
  free();
 }

template <class T> inline int List<T>::add(T const &object)
 {
  // alloc object
  T *store=new T;
  if (!store) return 0;
  // assign
  *store=object;
  // add to list
  if (!add(store))
  {
   delete store;
   return 0 ;
  }
  else return 1;
 }

template <class T> inline int List<T>::add(T *object)
 {
  // fail on null object
  if (object==NULL) return 0;
  // create node
  ListNode<T> *node=new ListNode<T>;
  if (!node) return 0;
  node->object=object;
  // add at tail
  node->prev=Root.prev;
  node->next=&Root;
  node->prev->next=node;
  node->next->prev=node;
  return 1;
 }

template <class T> inline int List<T>::replace(T *object,T *replacement)
 {
  // seach through list for matching object
  ListNode<T> *node=Root.next;
  while (node->object)
  {
   // check for match
   if (node->object==object)
   {
    // delete object
    delete node->object;
    // replace it
    node->object=replacement;
    return 1;
   }
   // move to next node
   node=node->next;
  }
  // no match
  return 0;
 }

template <class T> inline int List<T>::remove(T *object)
 {
  // seach through list for matching object
  ListNode<T> *node=Root.next;
  while (node->object)
  {
   // check for match
   if (node->object==object)
   {
    // next/prev
    ListNode<T> *next=node->next;
    ListNode<T> *prev=node->prev;
    // remove from list
    prev->next=next;
    next->prev=prev;
    // remove from list
    //node->prev->next=node->next;
    //node->next->prev=node->prev;
    // delete node
    delete node;
    return 1;
   }
   // move to next node
   node=node->next;
  }
  // no match
  return 0;
 }

template <class T> inline void List<T>::remove()
 {
  // remove all list nodes
  ListNode<T> *node=Root.next;
  while (node->object)
  {
   // remove node from list
   node->prev->next=node->next;
   node->next->prev=node->prev;
   // delete node
   delete node;
   node=Root.next;
  }
  // reset list
  Root.next=&Root;
  Root.prev=&Root;
  Root.object=NULL;
 }

template <class T> inline int List<T>::free(T *object)
 {
  // remove the object from list
  if (!remove(object)) return 0;
  // delete the object
  delete object;
  return 1;
 }

template <class T> inline void List<T>::free()
 {
  // free all list nodes
  ListNode<T> *node=Root.next;
  while (node->object)
  {
   // next/prev
   ListNode<T> *prev=node->prev;
   ListNode<T> *next=node->next;
   // remove from list
   prev->next=next;
   next->prev=prev;
   // remove node from list
   //node->prev->next=node->next;
   //node->next->prev=node->prev;
   // delete object
   delete node->object;
   delete node;
   node=Root.next;
  }
  // clear list nodes
  Root.next=&Root;
  Root.prev=&Root;
 }

 /*

 template <class T> inline ListIterator<T> List<T>::first()
 {
  // return iterator at first node in list
  ListIterator<T> i;
  i.CurrentNode=Root.next;
  return i;
 }

 template <class T> inline ListIterator<T> List<T>::last()
 {
  // return iterator at last node in list
  ListIterator<T> i;
  i.CurrentNode=Root.prev;
  return i;
 }

 */

 /*

template <class T> inline void List<T>::merge(List<T> &other)
 {
  // merge items another list with this list
  ListIterator<T> iterator=other.first();
  T *object=iterator.current();
  while (object)
  {
   add(object);
   object=iterator.next();
  }
  // remove all items from other list
  other.remove();
 }

 */

template <class T> inline T* List<T>::head() const
 {
  // return head object
  return Root.next->object;
 }

template <class T> inline T* List<T>::tail() const
 {
  // return tail object
  return Root.prev->object;
 }

 // --- axon ---

 // does absolutely NO checking whatsoever!!!! Callers responsibility...
 // first node is 0..
template <class T> inline T* List<T>::num(int i)
 {
  ListNode<T> *node;

  node = Root.head;
  while (i!=0)
  {
   node=node->next;
   i--;
  }
  return node->object;
 }

 // ---

template <class T> inline int List<T>::size() const
 {
  // return the number of objects stored in the list
  int count=0;
  ListNode<T> *node=Root.next;
  while (node->object)
  {
   node=node->next;
   count++;
  }
  return count;
 }

 /*

template <class T> inline void List<T>::operator =(List<T> &other)
 {
  // free list
  free();
  // add items from other list to this list
  ListIterator<T> iterator=other.first();
  T *current=iterator.current();
  while (current)
  {
   // copy to this list
   T *copy=new T;
   if (copy)
   {
    *copy=*current;
    add(copy);
   }
   // move to next item
   current=iterator.next();
  }
 }

 */

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
