#ifndef _GLIST_H_

typedef unsigned char node;

#define MAXLIST  16

typedef struct {
                 node PrevNode;
                 node NextNode;
               } GLIST;

typedef struct {
                 GLIST ListArray[(MAXLIST)];
                 unsigned char ListContent[(MAXLIST*4);
                 unsigned char CurrentNode;                 
               } LISTMANAGER;

// if node value equal zero, is terminal

#define ImplementList(x)    LISTMANGER x##m; GLIST x
#define InitialList(x)      x.PrevNode = x.NextNode = 0
#define CurrentNodeSize(x)  (x.NextNode - x.PrevNode + 1)
#define AddList(x,s)        (x##m.CurrentNode

#endif
