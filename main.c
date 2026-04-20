#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "dialog.h"

int main()
{
	Table *table = createTab();
	
	int opt = -1;
	
	char c;

	Err code = ERR_OK;

	while (opt != 0)
	{
		printf("\n");
		printf("****МЕНЮ****\n");
		printf("0) выход\n");
		printf("1) добавление элемента в таблицу по значению ключа\n");
		printf("2) поиск в таблицы по ключу\n");
		printf("3) вывод таблицы\n");	
		printf("4) импорт данных из текстового файла\n");	
		printf("5) Удаление элемента, а так же всех его дочерних элементов\n");
		printf("6) Вывод элемента и его родителей\n");
		int ent = scanf("%d%c", &opt, &c);
		if (ent == EOF)
		{
			freeTab(table);
			return 0;
		}

		switch (opt)
		{
			case 0:
				printf("Выход...\n");
				break;
			case 1:
				if (table == NULL)
				{
					table = createTab();
				}
				code = dialogInput(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 2:
				code = dialogFind(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 3:
				code = printTab(table);
				printErr(code);
				break;
			case 4:
				code = dialogRead(&table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 5:
				code = dialogGetAll(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}	
				break;
			case 6:
				code = dialogFindAll(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			default:
				printf("Такой команды нет\n");
				break;				
		}
	}
	freeTab(table);
	return 0;
}
