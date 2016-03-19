/**********************************************************************************************																							   *
File:				phase_one.h
Developers:			Bar Eitan
					Daniel Ashual
Description:		Definitions for first phase
Date created   : March 18, 2016
**********************************************************************************************/

#ifndef _FIRST
#define _FIRST	1

#include <stdio.h>
#include "structs_config.h"
#include "toolkit.h"
#include "structs_config.h"


/*******************************************	MACROS */

#define LINE_SIZE		80				/* Maximum amount of characters in a line */
#define COMMENT			';'				/* Comment sign                           */
#define NO_ADDRESS		-1				/* No address flag                        */
#define LINE_BASE		100				/* Base counter				  */
#define DELIM			":\57\t\40\n"			/* General delimiters			  */
#define SYM_DELIM		':'				/* Token delimiter			  */
#define DATA_DELIM		",\t\40\n"			/* Data delimiter			  */ 
#define SPACE_DELIM		"\t\40\n"			/* White space delimiter		  */
#define STR_DELIM		"\42\t\40\n"			/* String literal delimiter	     	  */
#define OPR_DELIM		",\t\40\n\57"			/* Operands delimiter			  */
#define TYPDBL_DELIM		"\57\54"			/* Command, dbl and type delimiter        */


/*******************************************	PROTOTYPES */


/*
 * phase_one : Encodes the the given file to the .as file's commands and data into the data struct. 
 * Also, Assigns to memory cells.
 */
void phase_one(IN_DATA_FILE* data_file);


/*
 * symb_transfer : Copies from the source token to the destination.
 * assumes the first character is a letter.
 */
int symb_transfer(char* destination_symbol, char* source_symbol);


/*
 *	command_manage : Adds a token to the symbol table, processes the command and copy possible symbols.
 */
void command_manage(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol);


/*
 * ent_ext_management : Inspects a given entry command line by parsing it, adds possible tokens to entry table.
 */
void ent_ext_management(IN_DATA_FILE* data_file, char *temp_token, char* error_buf, int line_number, int ent_ext_sym);


/*
 *	string_management : Adds given string literal to the data list by adding. Adds a null terminator at the string's end.
 */
void string_management(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol);


/*
 * num_management : adds each number to the data list.
 */
void num_management(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol);

#endif


