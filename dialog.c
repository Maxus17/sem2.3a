#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "tabs.h"

Err dialogInput(Table *table)
{
	KeyType key;
	KeyType par;
	InfoType *info;
		
	int inp = 2;
	Err code = ERR_OK;
	
	while (inp == 2)
	{
		printf("введите ключ нового элемента(он должен быть уникальным)\n> ");
		inp = input(&key);
		if (inp == EOF)
		{
			return ERR_EOF;
		}
	}
	inp = 2;

	while (inp == 2)
	{
		printf("введите родительский ключ нового элемента(0 или один из ранее введеных)\n> ");
		inp = input(&par);
		if (inp == EOF)
		{
			return ERR_EOF;
		}	
	}
	inp = 2;

	printf("Введите информацию элемента\n");
	info = readline("> ");
	if (info == NULL)
	{
		return ERR_EOF;
	}

	code = put(table, key, par, info, 1);
	if (code != ERR_OK)
	{	
		free(info);
		return code;
	}
	
	free(info);
	return ERR_OK;
}

Err dialogFind(Table *table)
{
	int inp = 2;
	
	KeyType key;

	while (inp == 2)
	{
		printf("Введите ключ для поиска\n> ");
		inp = input(&key);
		if (inp == EOF)
		{
			return ERR_EOF;
		}
	}
	inp = 2;

	KeySpace *element = findElem(table, key);
	if (element == NULL)
	{
		return ERR_ELEMENT;
	}
	printf("%d %d %s", element->key, element->par, element->info);
	free(element->info);
	free(element);
	return ERR_OK;
}

Err dialogRead(Table **table)
{
	printf("введите название файла\n");
	char *file = readline("> ");
	if (file == NULL)
	{
		return ERR_EOF;
	}
	Err code = readFile(file, table);
	if (code != ERR_OK)
	{
		free(file);
		return code;
	}
	free(file);
	return ERR_OK;
}

Err dialogGetAll(Table *table)
{
	KeyType key;

	int inp = 2;

	while (inp == 2)
	{
		printf("введите ключ для удаления\n> ");
		inp = input(&key);
		if (inp == EOF)
		{
			return ERR_EOF;
		}
	}
	inp = 2;

	Err code = getRec(table, key);
	if (code != ERR_OK)
	{
		return code;
	}
	return ERR_OK;
}

Err dialogFindAll(Table *table)
{
	int inp = 2;

	KeyType par;
	
	while (inp == 2)
	{
		printf("введите ключ для поиска\n> ");
		inp = input(&par);
		if (inp == EOF)
		{
			return ERR_EOF;
		}
	}
	inp = 2;

	Table *res = findPar(table, par);
	if (res == NULL)
	{
		return ERR_EMPTY;
	}
	Err code = printTab(res);
	if (code != ERR_OK)
	{
		freeTab(res);
		return code;
	}
	freeTab(res);
}
