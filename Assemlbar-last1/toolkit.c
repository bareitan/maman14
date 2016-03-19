/**********************************************************************************************
File:				toolkit.c	
Developers:			Bar Eitan
					Daniel Ashual
Description:		Assistance functions for the runtime enviorment of the program.			   
Creation Date:		March 18, 2016
**********************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "toolkit.h"
#include "structs_config.h"

int is_number(char *str)
{
	if (*str == '-')
		str++;
	while (*str)
	{
		if (!isdigit(*str))
			return FALSE;
		str++;
	}
	return TRUE;
}


void data_change(IN_DATA_FILE* data_file)
{
	MEMORY_LIST_NODE* list = data_file->content_memory;
	unsigned int code_of_last_adrs = data_file->IC;
	while (list)
	{
		list->cell->ln_address += code_of_last_adrs;
		list = list->next;
	}

}


void code_change(IN_DATA_FILE* data_file)
{
	MEMORY_LIST_NODE* list = data_file->key_memory;
	while (list)
	{
		if (list->cell->type == RELOCATEABLE)
			list->cell->code += data_file->SC;
		list = list->next;
	}

}


void create_output(char* file_name, IN_DATA_FILE* data_file)
{
	FILE* file = NULL;
	char asm_file_name[NAME_SIZE];
	strcpy(asm_file_name, file_name);
	strcat(asm_file_name, EXT_SUFFIX);
	if ((file = fopen(asm_file_name, "w")) == NULL)
	{
		printf("Error Occurred: Could not create the file <%s>\n", asm_file_name);
		exit(1);
	}
	extern_prnt_file(data_file->extern_list, file);
	strcpy(asm_file_name, file_name);
	strcat(asm_file_name, ENT_SUFFIX);
	if ((file = fopen(asm_file_name, "w")) == NULL)
	{
		printf("Error Occurred: Could not create the file <%s>\n", asm_file_name);
		exit(1);
	}
	entry_prnt_file(data_file->entry_table, file);
	strcpy(asm_file_name, file_name);
	strcat(asm_file_name, OB_SUFFIX);
	if ((file = fopen(asm_file_name, "w")) == NULL)
	{
		printf("Error Occurred: Could not create the file <%s>\n", asm_file_name);
		exit(1);
	}
	fprintf(file, "                Base8\n");
	fprintf(file, "     Base 8   Machine Code    A,R,E \n");
	fprintf(file, "    %14o   %o\n", data_file->IC - 100, data_file->DC);
	mem_prnt_file(data_file->key_memory, file);
	mem_prnt_file(data_file->content_memory, file);
	fclose(file);
}

int negative(int num)
{
	if ((num < 0) && (num >= MIN_NUM))
	{
		num += NGTV_FIX;
		return num;
	}
	else
		return num;
}

SYMBOL_LIST_NODE* symb_join(SYMBOL_LIST_NODE* head, char* val_label, int symbol_kind, int symbol_addr)
{
	SYMBOL_LIST_NODE* new_node = NULL;
	SYMBOL_LIST_NODE* cur_node = NULL;

	if ((new_node = (SYMBOL_LIST_NODE*) malloc(sizeof(SYMBOL_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at symb_join\n");
		exit(1);
	}
	if (head == NULL)
	{
		head = new_node;
		strncpy(head->val_label, val_label, SYM);
		head->symbol_addr = symbol_addr;
		head->symbol_kind = symbol_kind;
		head->next = NULL;
	}
	else
	{
		strncpy(new_node->val_label, val_label, SYM);
		new_node->symbol_addr = symbol_addr;
		new_node->symbol_kind = symbol_kind;
		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}

MEMORY_LIST_NODE* mem_join(MEMORY_LIST_NODE* head, MEM_CELL* new_cell) {
	MEMORY_LIST_NODE* new_node = NULL;
	MEMORY_LIST_NODE* cur_node = NULL;

	if ((new_node = (MEMORY_LIST_NODE*) malloc(sizeof(MEMORY_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at mem_join\n");
		exit(1);
	}
	if (head == NULL)
	{
		head = new_node;
		head->cell = new_cell;
		head->next = NULL;
	}
	else
	{
		new_node->cell = new_cell;
		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}

void mem_prnt_file(MEMORY_LIST_NODE* list, FILE* file) {
	while (list)
	{
		fprintf(file, "%10.4o\t%.7o%7c\n", list->cell->ln_address, list->cell->code, list->cell->type);
		list = list->next;
	}
}

EXTERN_LIST_NODE* external_join(EXTERN_LIST_NODE* head, char* val_label, int symbol_addr)
{
	EXTERN_LIST_NODE* new_node = NULL;
	EXTERN_LIST_NODE* cur_node = NULL;

	if ((new_node = (EXTERN_LIST_NODE*) malloc(sizeof(EXTERN_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at external_join\n");
		exit(1);
	}

	if (head == NULL)
	{
		head = new_node;
		strncpy(head->val_label, val_label, SYM);
		head->symbol_addr = symbol_addr;
		head->next = NULL;
	}
	else
	{
		strncpy(new_node->val_label, val_label, SYM);
		new_node->symbol_addr = symbol_addr;
		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}

ENTRY_LIST_NODE* entry_join(ENTRY_LIST_NODE* head, char* val_label, int symbol_addr)
{
	ENTRY_LIST_NODE* new_node = NULL;
	ENTRY_LIST_NODE* cur_node = NULL;

	if ((new_node = (ENTRY_LIST_NODE*) malloc(sizeof(ENTRY_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at entry_join\n");
		exit(1);
	}

	if (head == NULL)
	{
		head = new_node;
		strncpy(head->val_label, val_label, SYM);
		head->symbol_addr = symbol_addr;
		head->next = NULL;
	}
	else
	{
		strncpy(new_node->val_label, val_label, SYM);
		new_node->symbol_addr = symbol_addr;
		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}

ERROR_LIST_NODE* error_join(ERROR_LIST_NODE* head, char* error_log, unsigned int line_number)
{
	ERROR_LIST_NODE* new_node = NULL;
	ERROR_LIST_NODE* cur_node = NULL;

	if ((new_node = (ERROR_LIST_NODE*) malloc(sizeof(ERROR_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at error_join\n");
		exit(1);
	}

	if (head == NULL)
	{
		head = new_node;
		strncpy(head->error_log, error_log, 256);
		head->line_number = line_number;
		head->next = NULL;
	}
	else
	{
		strncpy(new_node->error_log, error_log, 256);
		new_node->line_number = line_number;

		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}

void entry_prnt_file(ENTRY_LIST_NODE* list, FILE* file)
{
	while (list)
	{
		fprintf(file, "%-32s%o\n", list->val_label, list->symbol_addr);
		list = list->next;
	}
}

void extern_prnt_file(EXTERN_LIST_NODE* list, FILE* file)
{
	while (list)
	{
		fprintf(file, "%-32s%o\n", list->val_label, list->symbol_addr);
		list = list->next;
	}
}

void error_prnt_file(ERROR_LIST_NODE* list, char* file_name) {
	printf("Printing errors for the file %s.as:\n", file_name);
	while (list)
	{
		printf("- Error found at line %u: %s \n", list->line_number, list->error_log);
		list = list->next;
	}
	printf("\n");
}

COMMAND_LIST_NODE* command_join(COMMAND_LIST_NODE* head, COMMAND_CONTAINER* cmd_obj)
{
	COMMAND_LIST_NODE* cur_node = NULL;
	COMMAND_LIST_NODE* new_node = NULL;

	if ((new_node = (COMMAND_LIST_NODE*) malloc(sizeof(COMMAND_LIST_NODE))) == NULL)
	{
		printf("Error Occurred: Memory allocation failed at command_join\n");
		exit(1);
	}

	if (head == NULL)
	{
		head = new_node;
		head->cmd_obj = cmd_obj;
		head->next = NULL;
	}
	else
	{
		new_node->cmd_obj = cmd_obj;
		new_node->next = NULL;
		cur_node = head;
		while (cur_node->next)
			cur_node = cur_node->next;
		cur_node->next = new_node;
	}
	return head;
}


/* Free functions */


void memory_free(MEMORY_LIST_NODE* node)
{
	MEMORY_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node->cell);
		free(node);
		node = cur_node;
	}
}

void extern_free(EXTERN_LIST_NODE* node)
{
	EXTERN_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node);
		node = cur_node;
	}
}

void entry_table_free(ENTRY_LIST_NODE* node)
{
	ENTRY_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node);
		node = cur_node;
	}
}

void symbol_table_free(SYMBOL_LIST_NODE* node)
{
	SYMBOL_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node);
		node = cur_node;
	}
}

void error_free(ERROR_LIST_NODE* node)
{
	ERROR_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node);
		node = cur_node;
	}
}

void command_list_free(COMMAND_LIST_NODE* node)
{
	COMMAND_LIST_NODE* cur_node = node;
	while (cur_node)
	{
		cur_node = cur_node->next;
		free(node);
		node = cur_node;
	}
}

void in_data_file_initialize(IN_DATA_FILE* data_file)
{
	data_file->content_memory = NULL;
	data_file->key_memory = NULL;
	data_file->entry_table = NULL;
	data_file->extern_table = NULL;
	data_file->extern_list = NULL;
	data_file->symbol_table = NULL;
	data_file->error_list = NULL;
	data_file->command_hold_list = NULL;
	data_file->IC = LINE_BASE;
	data_file->DC = 0;
	data_file->SC = 0;
}
