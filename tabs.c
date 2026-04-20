#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabs.h"

Table *createTab()
{
	Table *table = (Table *)malloc(sizeof(Table));
	if (table == NULL)
	{
		return NULL;
	}	
	table->ks = NULL;
	table->csize = 0;
	return table;
}

bool isEmpty(Table *table)
{
	if (table == NULL || table->ks == NULL || table->csize == 0)
	{
		return true;
	}
	return false;
}

void freeTab(Table *table)
{
	if (table != NULL)
	{
		KeySpace *cur = table->ks;
		KeySpace *next;
		while (cur != NULL)
		{
			next = cur->next;
			free(cur->info);
			free(cur);
			cur = next;
		}
		table->ks = NULL;
		table->csize = 0;
		free(table);
	}
}

KeySpace *findElem(Table *table, const KeyType key)
{	
	if (isEmpty(table))
	{
		return NULL;
	}
	KeySpace *cur = table->ks;
	while (cur != NULL)
	{
		if (cur->key == key)
		{
			KeySpace *cpy = (KeySpace *)malloc(sizeof(KeySpace));
			if (cpy == NULL)
			{
				return NULL;
			}
			cpy->key = cur->key;
			cpy->par = cur->par;
			cpy->info = strdup(cur->info);
			cpy->next = NULL;
			return cpy;
		}
		cur = cur->next;
	}
	return NULL;
}

bool checkPar(Table *table, KeyType par)
{
	if (par == 0)
	{
		return true;
	}

	KeySpace *cur = table->ks;
	while (cur != NULL)
	{
		if (cur->key == par)
		{
			return true;
		}
		cur = cur->next;
	}

	return false;
}

Err put(Table *table, const KeyType key, const KeyType par, const InfoType *info, const int ch_flag)
{		
	KeySpace *element = findElem(table, key);
	if (element)
	{
		free(element->info);
		free(element);
		return ERR_DOUBLE;
	}

	if ((!checkPar(table, par)) && (ch_flag == 1))
	{
		return ERR_PAR;
	}

	KeySpace *ptr = (KeySpace *)calloc(1, sizeof(KeySpace));
	if (ptr == NULL)
	{
		return ERR_MEM;
	}

	ptr->key = key;
	ptr->par = par;
	ptr->info = strdup(info);
	ptr->next = NULL;

	if (table->ks == NULL)
	{
		table->ks = ptr;
	}
	else
	{
		KeySpace *cur = table->ks;
		while (cur->next != NULL)
		{
			cur = cur->next;
		}
		cur->next = ptr;
	}

	table->csize++;
	return ERR_OK;
}

Err printTab(Table *table)
{
	if (!isEmpty(table))
	{
		KeySpace *cur = table->ks;
		while (cur != NULL)
		{
			printf("%d %d %s\n", cur->key, cur->par, cur->info);
			cur = cur->next;
		}
		return ERR_OK;
	}
	else
	{
		return ERR_EMPTY;
	}
}

char *freadline(FILE *fp)
{
	char buf[81] = {0};
	char *res = NULL;
	int len = 0, n = 0;

	do
	{
		n = fscanf(fp, "%80[^\n]", buf);
		if (n < 0)
		{
			if (!res)
			{
				return NULL;
			}
		}
		else if (n > 0)
		{
			int new_len = strlen(buf);
			int str_len = len + new_len;
			char *new_res = realloc(res, (str_len + 1) * sizeof(char));
			if (!new_res)
			{
				free(res);
				return NULL;
			}
			res = new_res;
			memcpy(res + len, buf, new_len);
			len = str_len;
		}
		else
		{
			fscanf(fp, "%*c");
		}
	} while (n > 0);

	if (len > 0)
	{
		*(res + len) = '\0';
	}
	else
	{
		free(res);
		return NULL;
	}

	return res;
}

Err readFile(const char *fname, Table **table)
{	
	Err code = ERR_OK;
		
	FILE *fp = fopen(fname, "r");	
	if (fp == NULL)
	{	
		return ERR_FILE;
	}
	
	int cnt = 0;
	if (fscanf(fp, "%d\n", &cnt) <= 0)
	{
		fclose(fp);
		return ERR_FORMAT;
	}

	if (table != NULL)
	{
		freeTab(*table);
		*table = createTab();	
	}

	fscanf(fp, "%*[\n]%*c");

	for (int i = 0; i < cnt; i++)
	{
		KeyType key = 0, par = 0;
		InfoType *inf = NULL;


		int a = fscanf(fp, "%d\n", &key);
		int b = fscanf(fp, "%d\n", &par);
		inf = freadline(fp);
		if (a == 0 || b == 0)
		{
			fclose(fp);
			free(inf);
			return ERR_FORMAT;
		}
		
		code = put(*table, key, par, inf, 1);
		if (code != ERR_OK)
		{
			printErr(code);
			fclose(fp);
			free(inf);
			return code;
		}
		free(inf);
	}

	fclose(fp);
	return ERR_OK;
}

Err getRec(Table *table, const KeyType key)
{
	KeySpace *element = findElem(table, key);
	if (isEmpty(table))
	{
		return ERR_EMPTY;
	}
	
	if (!element)
	{
		return ERR_ELEMENT;
	}
	free(element->info);
	free(element);

	KeySpace *cur = table->ks;
	KeySpace *prev = NULL;

	while (cur != NULL)
	{
		int del_flag = 0;
		if (cur->key == key)
		{
			del_flag = 1;
		}
		else
		{
			KeySpace *parent = findElem(table, cur->par);
			if (parent == NULL && cur->par != 0)
			{
				del_flag = 1;
			}
		}

		if (del_flag)
		{
			KeySpace *del = cur;
			cur = cur->next;

			if (prev == NULL)
			{
				table->ks = cur;
			}
			else
			{
				prev->next = cur;
			}

			free(del->info);
			free(del);
			table->csize--;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}

	return ERR_OK;
}

Table *findPar(Table *table, const KeyType key)
{	
	Err code = ERR_OK;
	Table *res = createTab();

	if (res == NULL)
	{
		freeTab(table);
		return NULL;
	}
	
	KeySpace *cur = findElem(table, key);
	if (cur == NULL)
	{
		freeTab(res);
		return NULL;
	}
	
	code = put(res, cur->key, cur->par, cur->info, 0);
	if (code != ERR_OK)
	{
		printErr(code);
		freeTab(res);
		return NULL;
	}
	
	KeyType par = cur->par;
	while (par != 0)
	{
		KeySpace *parent = findElem(table, par);
		if (parent == NULL)
		{
			freeTab(res);
			return NULL;
		}

		code = put(res, parent->key, parent->par, parent->info, 0);
		if (code != ERR_OK)
		{
			printErr(code);
			freeTab(res);
			return NULL;
		}

		par = parent->par;
		free(parent->info);
		free(parent);
	}
	free(cur->info);
	free(cur);
	return res;
}
