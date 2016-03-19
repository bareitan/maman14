/***********************************************************************************************
File:				phase_two.h
Developers:			Bar Eitan				                   
					Daniel Ashual
Description:		Parsing assistance header contains fundamental protoypes 				   
Creation Date:		March 18,2016
* *																							   *
 **********************************************************************************************/

#ifndef _SECOND
#define _SECOND	1

#include <stdio.h>
#include "toolkit.h"

/*******************************************	MACROS */

/* Processing constants */
#define IMMEDIATE		0		/* immediate addressing mode           */
#define DIRECT			1		/* Direct addressing mode              */
#define INDEX			2  		/* indexed addressing mode             */
#define DIRECT_REG		3  		/* Direct register addressing mode     */
#define RLT_DELIM		"{}"		/* Command parsing delimiter           */

/* parsing constants */
#define CMB_SHIFT		1		/* Comb                                */
#define DST_REG_SHIFT	2 			/* destination register                */
#define DST_OPR_SHIFT	5 			/* destination operand                 */
#define SRC_REG_SHIFT	7 			/* source register                     */
#define SRC_OPR_SHIFT	10 			/* source operand                      */
#define CMD_SHIFT		12		/* Command                             */           
#define TYP_SHIFT		16		/* type flag          		       */
#define DBL_SHIFT		17		/* dbl flag     		       */


/*******************************************	PROTOTYPES */

/*	
 * entry_table_change : 
 * Encodes the operands to machine code
 * Secondary processes after the first pass.
 */
void entry_table_change(IN_DATA_FILE* data_file, char* error_b);


/*
 * symbol_table_change :
 * Updates the symbol table defined by a file.
 */
void symbol_table_change(IN_DATA_FILE* data_file);


/*
 * parse_for_token :
 * Finds a token in the symbol table.
 */
void parse_for_token(char* val_symbol, IN_DATA_FILE* data_file, int is_relative, char* error_b, int cur_address, int line_number);


/*
 * create_command_code :
 * Encodes a command. 
 */
void create_command_code(MEM_CELL* mem_cell, COMMAND_STRUCT* command_struct);


/*
 * create_dbl_command_code :
 * Encod the dbl of the command.
 */
void create_dbl_command_code(MEM_CELL* mem_cell, int dbl);


/*
 * create_type_command_code :
 * Encodes the comb and type of the command.
 */
void create_type_command_code(MEM_CELL* mem_cell, int* typ);


/*
 * create_address_command_code : 
 * Writes an address to a certain memory cell.
 */
void create_address_command_code(MEM_CELL* mem_cell, int bit_to_shift, int adrs_mode_code);


/*
 * check_addressmode :  
 * Checks whether given address is valid.
 */
int check_addressmode(char* valid_adrs_modes, int adrs_mode);


/*
 * register_check : 
 * Checks whether given register is valid.
 */
int register_check(char* str, int* reg_num);


/*
 * oper_manage : 
 * Validates given addressing modes and process the commands, if no errors occur, attempts writes given data to a memory cell.
 */
void oper_manage(char* opernd, char* adrs_modes, IN_DATA_FILE* data_file, MEM_CELL* mem_cell, int operand_shift, int reg_shift, int line_number, char* command_name);


/*
 * phase_two :
 * interprets and assigns validated data to its destination
 */
void phase_two(IN_DATA_FILE* data_file);

#endif
