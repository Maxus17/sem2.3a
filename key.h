#ifndef KEY_H
#define KEY_H

#include "err.h"
#include "inp.h"

typedef int KeyType;
typedef int IndexType;
typedef char InfoType;

typedef struct KeySpace{
	KeyType key;//ключ элемента
	KeyType par;//ключ родителя
	InfoType *info;
	struct KeySpace *next;
}KeySpace;


#endif
