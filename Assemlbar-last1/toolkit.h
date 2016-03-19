/***********************************************************************************************
File:				toolkit.h
Developers:			Bar Eitan
					Daniel Ashual
Description:		Defines the program's data structure                    				 
Creation Date:		March 18, 2016
**********************************************************************************************/

#ifndef _UTILITY
#define _UTILITY	1

#include <stdio.h>
#include "structs_config.h"

/*MACROS*/

/*Valid file extensions */
#define AS_SUFFIX		".as"
#define OB_SUFFIX		".ob"
#define ENT_SUFFIX		".ent"
#define EXT_SUFFIX		".EXTERN"

/* Boolean indicators    */
#define TRUE			1
#define FALSE			0

#define BASE			10         /* strtol aid base							 */
#define MIN_NUM			-524288	   /* Minimum value would fit 20 bit -(2^19)				 */
#define NGTV_FIX		1048576	   /* Fixer for a negative given value for a 32 bit integer vale (2 ^ 20)*/

/*******************************************	PROTOTYPES */

/*
 * code_change : Recalcualtes and updates code list accroding to SC counter.
 */
void code_change(IN_DATA_FILE* data_file);


/*
 * data_change : Recalculates and updates the data list according to the address of the list.
 */
void data_change(IN_DATA_FILE* data_file);


/*
 * create_output : Creates .ob / .ext / .ent files.
 */
void create_output(char* file_name, IN_DATA_FILE* data_file);


/*
 * is_number : Checks whether given value is a number.
 */
int is_number(char *str);


/*
 * negative : Attempts to convert less than 0 numbers to 20 bit format from 32 bit parameter.
 */
int negative(int num);

/*
 * symb_join : Adds a token to the token table.
 * The head of the token list is returned.
 */
SYMBOL_LIST_NODE* symb_join(SYMBOL_LIST_NODE* head, char* val_label, int symbol_kind, int symbol_addr);


/*
 * mem_join : Adds a new node to the memory list. 
 * The head of the memory list is returned.
 */
MEMORY_LIST_NODE* mem_join(MEMORY_LIST_NODE* head, MEM_CELL* node);


/*
 * entry_join : Adds a new Entry node to the Entry list.
 * the head of the list is returned.
 */
ENTRY_LIST_NODE* entry_join(ENTRY_LIST_NODE* head, char* val_label, int symbol_addr);


/*
 * external_join : Adds a new external token node to the external token list.
 * The head of the list is returned. 
 */
EXTERN_LIST_NODE* external_join(EXTERN_LIST_NODE* head, char* val_label, int symbol_addr);

/*
 * error_join : Adds a new error node to the error list.
 * The head of the list is returned.
 */
ERROR_LIST_NODE* error_join(ERROR_LIST_NODE* head, char* error_log, unsigned int line_number);


/*
 * command_join : Adds a new command node to the list. 
 * The head of the list is returned.
 */
COMMAND_LIST_NODE* command_join(COMMAND_LIST_NODE* head, COMMAND_CONTAINER* cmd_obj);


/*
 * mem_prnt_file : Prints the memory list into given file.
 */
void mem_prnt_file(MEMORY_LIST_NODE* list, FILE* file);


/*
 * error_prnt_file : Prints the error list from given file to the screen.
 */
void error_prnt_file(ERROR_LIST_NODE* list, char* file_name);


/*
 * entry_prnt_file : Prints the entry list into a given file.
 */
void entry_prnt_file(ENTRY_LIST_NODE* list, FILE* file);


/*
 * extern_prnt_file : Prints the external list into a given file.
 */
void extern_prnt_file(EXTERN_LIST_NODE* list, FILE* file);


/*
 * memory_free : Frees the memory of the given memory list.
 */
void memory_free(MEMORY_LIST_NODE* list);


/*
 * extern_free : Frees the memory of the given external list.
 */
void extern_free(EXTERN_LIST_NODE* extern_node);


/*
 * entry_table_free : Frees the memory of the given entry table.
 */
void entry_table_free(ENTRY_LIST_NODE* entry_table);


/*
 * symbol_table_free : Frees the memory of the given symbol table.
 */
void symbol_table_free(SYMBOL_LIST_NODE* sym_table);


/*
 * error_free : Frees the memory of the given errors list.
 */
void error_free(ERROR_LIST_NODE* err_list);


/*
 * command_list_free : Frees the memory of the command holders list.
 */
void command_list_free(COMMAND_LIST_NODE* cmd_holder_list);


/*
 *	in_data_file_initialize : Creates a new ASMF_OBJ Struct using a given file.
 */
void in_data_file_initialize(IN_DATA_FILE* data_file);

#endif

