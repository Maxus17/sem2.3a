#ifndef TABS_H
#define TABS_H

#include "key.h"

typedef struct Table{
	KeySpace *ks;// указательна начало списка - head
	IndexType csize;
}Table;

Table *createTab();
void freeTab(Table *);
bool isEmpty(Table *);
KeySpace *findElem(Table *, const KeyType );
bool checkPar(Table *, KeyType );
Err put(Table *, const KeyType, const KeyType, const InfoType *, const int);
Err printTab(Table *);
Err readFile(const char *, Table **);
Err getRec(Table *, const KeyType );
Table *findPar(Table *, const KeyType);

#endif
