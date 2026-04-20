#ifndef DIALOG_H
#define DIALOG_H

#include "tabs.h"

Err dialogInput(Table *);
Err dialogGet(Table *);
Err dialogFind(Table *);
Err dialogRead(Table **);
Err dialogGetAll(Table *);
Err dialogFindAll(Table *);

#endif
