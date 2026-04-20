#ifndef ERR_H
#define ERR_H

#include <stdbool.h>

typedef enum Err{
	ERR_OK = 0,
	ERR_MEM = -1,
	ERR_EMPTY = -2,
	ERR_FORMAT = -3,
	ERR_EOF = -4,
	ERR_DOUBLE = -5,
	ERR_ELEMENT = -6,
	ERR_PAR = -7,
	ERR_FILE = -8,
}Err;

void printErr(Err code);

#endif
