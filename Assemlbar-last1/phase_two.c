/***********************************************************************************************
File:				phase_two.c
Developers:			Bar Eitan
					Daniel Ashual
Description:		Operations on the data that was collected on phase 1 + Conversion to machine code.
Creation Date:		March 18, 2016
**********************************************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "phase_two.h"


extern GUIDENCE_STRUCT guidences[];
extern COMMAND_STRUCT commands[];
extern REGISTER_STRUCT registers[];
extern MODEADD adrs_modes[];

void add_num(IN_DATA_FILE* data_file, int num) {
	MEM_CELL* word = NULL;
	word = (MEM_CELL*) malloc(sizeof(MEM_CELL));
	if (!word) {
		printf("Error Occurred: Memory allocation failed at add_num\n");
		exit(1);
	}
	num = negative(num);
	word->code = num;
	++data_file->IC;
	word->ln_address = data_file->IC;
	word->type = ABSOLOTE;
	data_file->key_memory = mem_join(data_file->key_memory, word);
}

void entry_table_change(IN_DATA_FILE* data_file, char* error_b) {
	SYMBOL_LIST_NODE* cur_sym;
	ENTRY_LIST_NODE* cur_ent;
	cur_ent = data_file->entry_table;

	while (cur_ent) {
		cur_sym = data_file->symbol_table;
		while (cur_sym) {
			if (strcmp(cur_sym->val_label, cur_ent->val_label) == 0)
				break;
			cur_sym = cur_sym->next;
		}
		if (cur_sym)
			cur_ent->symbol_addr = cur_sym->symbol_addr;
		else {
			sprintf(error_b, "Unknown token \"%s\". No matching symbol found ", cur_ent->val_label);
			data_file->error_list = error_join(data_file->error_list, error_b, 1);
		}
		cur_ent = cur_ent->next;
	}
}

void symbol_table_change(IN_DATA_FILE* data_file) {
	SYMBOL_LIST_NODE* cur_sym;
	cur_sym = data_file->symbol_table;
	while (cur_sym) {
		if (cur_sym->symbol_kind == 0)
			cur_sym->symbol_addr += data_file->IC;
		cur_sym = cur_sym->next;
	}
}

void parse_for_token(char* val_symbol, IN_DATA_FILE* data_file, int is_relative, char* error_b, int cur_address, int line_number)
{
	SYMBOL_LIST_NODE* cur_sym = data_file->symbol_table;
	EXTERN_LIST_NODE* cur_ext = NULL;
	int is_found_sym = FALSE;
	MEM_CELL* word = NULL;
	char type = NONE;
	int code = 0;
	int found_addr = 0;
	int symbol_machine_code = -1;
	while (cur_sym)
	{
		if (strcmp(cur_sym->val_label, val_symbol) == 0)
		{
			is_found_sym = TRUE;
			symbol_machine_code = cur_sym->symbol_addr + data_file->SC;
			break;
		}
		cur_sym = cur_sym->next;
	}
	if (is_found_sym == FALSE) {
		cur_ext = data_file->extern_table;
		while (cur_ext) {
			if (strcmp(cur_ext->val_label, val_symbol) == 0) {
				is_found_sym = TRUE;
				break;
			}
			cur_ext = cur_ext->next;
		}
	}
	if (cur_sym) {
		type = RELOCATEABLE;
		found_addr = negative(cur_sym->symbol_addr);
		code = found_addr;
	}
	else if (cur_ext) {
		type = EXTERN;
		code = 0;
	}
	if (is_found_sym == TRUE) {
		word = (MEM_CELL*) malloc(sizeof(MEM_CELL));
		if (!word) {
			printf("Error Occurred: Memory allocation failed at parse_for_token!\n");
			exit(1);
		}
		if (is_relative == TRUE) {
			word->type = ABSOLOTE;
			found_addr = negative(symbol_machine_code - cur_address);
			code = found_addr;
		}
		else
			word->type = type;
		word->code = code;
		++data_file->IC;
		word->ln_address = data_file->IC;
		if (type == EXTERN)
			data_file->extern_list = external_join(data_file->extern_list, val_symbol, data_file->IC);
		data_file->key_memory = mem_join(data_file->key_memory, word);
	}
	else
	{
		sprintf(error_b, "Unknown token \"%s\". No matching symbol found ", val_symbol);
		data_file->error_list = error_join(data_file->error_list, error_b, line_number - LINE_BASE);
	}
}

void create_command_code(MEM_CELL* mem_cell, COMMAND_STRUCT* command_struct) {
	int op = command_struct->val_opcode;
	op <<= CMD_SHIFT;
	mem_cell->code = op;
}

void create_dbl_command_code(MEM_CELL* mem_cell, int dbl)
{
	int dbl_shift = dbl;
	dbl_shift <<= DBL_SHIFT;
	mem_cell->code |= dbl_shift;
}

void create_type_command_code(MEM_CELL* mem_cell, int* typ)
{
	int typ_shift = typ[FLAG];
	typ_shift <<= TYP_SHIFT;
	mem_cell->code |= typ_shift;
	if (typ[FLAG] == TRUE)
	{
		int cmb_left = typ[CMB_LEFT]; 
		cmb_left <<= CMB_SHIFT;
		mem_cell->code |= cmb_left;
		int cmb_right = typ[CMB_RIGHT];
		mem_cell->code |= cmb_right;
	}
}


void create_address_command_code(MEM_CELL* mem_cell, int bit_to_shift, int adrs_mode_code)
{
	adrs_mode_code <<= bit_to_shift;
	mem_cell->code |= adrs_mode_code;
}


int check_addressmode(char* valid_adrs_modes, int adrs_mode) {
	while (*valid_adrs_modes != '\0')
	{
		if ((*valid_adrs_modes) - '0' == adrs_mode)
			break;
		valid_adrs_modes++;
	}
	if (*valid_adrs_modes != '\0')
		return TRUE;
	return FALSE;
}

int register_check(char* str, int* reg_num) {
	REGISTER_STRUCT* cur_reg = registers;
	while (cur_reg->num != NOT_VALID) {
		if (strcmp(cur_reg->val_name, str) == 0)
		{
			*reg_num = cur_reg->num;
			break;
		}
		++cur_reg;
	}
	if (cur_reg->num == NOT_VALID)
		return FALSE;
	return TRUE;
}

void oper_manage(char* opernd, char* valid_adrs_modes, IN_DATA_FILE* data_file, MEM_CELL* mem_cell, int operand_shift, int reg_shift, int line_number, char* command_name)
{
	char* endp;
	char* temp_token = NULL;
	int num;
	int reg_num;
	int is_error = TRUE;
	char error_b[256];
	MODEADD* cur_adrs_mode = NULL;
	char opernd_tok[strlen(opernd) + 1];

	if ((strchr(opernd, '{') != NULL) && (strchr(opernd, '}') != NULL))
	{
		if (check_addressmode(valid_adrs_modes, INDEX) == TRUE)
		{
			strcpy(opernd_tok, opernd);
			temp_token = strtok(opernd_tok, RLT_DELIM);
			parse_for_token(temp_token, data_file, FALSE, error_b, mem_cell->ln_address, line_number);
			create_address_command_code(mem_cell, operand_shift, adrs_modes[DIRECT].val_opcode);
			is_error = FALSE;
			temp_token = strtok(NULL, RLT_DELIM);
			if (temp_token[0] == '*')
			{
				parse_for_token(temp_token + 1, data_file, TRUE, error_b, mem_cell->ln_address, line_number);
				create_address_command_code(mem_cell, operand_shift, adrs_modes[INDEX].val_opcode);
				is_error = FALSE;
			}
			else if (is_number(temp_token) == TRUE)
			{
				num = strtol(temp_token, &endp, BASE);
				create_address_command_code(mem_cell, operand_shift, adrs_modes[IMMEDIATE].val_opcode);
				add_num(data_file, num);
				is_error = FALSE;
			}
			else if (register_check(temp_token, &reg_num) == TRUE)
			{
				create_address_command_code(mem_cell, operand_shift, adrs_modes[DIRECT_REG].val_opcode);
				reg_num <<= reg_shift;
				mem_cell->code |= reg_num;
				is_error = FALSE;
			}
		}
	}
	else if (opernd[0] == '#')
	{
		if (check_addressmode(valid_adrs_modes, IMMEDIATE) == TRUE)
		{
			num = strtol(opernd + 1, &endp, BASE);
			if (*endp != '\0')
			{
				sprintf(error_b, "\"%s\" is used. Number is expected", opernd);
				data_file->error_list = error_join(data_file->error_list, error_b, line_number - LINE_BASE);
			}
			else
			{
				create_address_command_code(mem_cell, operand_shift, adrs_modes[IMMEDIATE].val_opcode);
				add_num(data_file, num);
				is_error = FALSE;
			}
		}
		else
		{
			cur_adrs_mode = &adrs_modes[IMMEDIATE];
		}
	}
	else
	{
		if (register_check(opernd, &reg_num) == TRUE)
		{
			if (check_addressmode(valid_adrs_modes, DIRECT_REG) == TRUE)
			{
				create_address_command_code(mem_cell, operand_shift, adrs_modes[DIRECT_REG].val_opcode);
				reg_num <<= reg_shift;
				mem_cell->code |= reg_num;
				is_error = FALSE;
			}
			else
			{
				cur_adrs_mode = &adrs_modes[DIRECT_REG];
			}
		}
		else
		{
			if (check_addressmode(valid_adrs_modes, DIRECT) == TRUE)
			{
				parse_for_token(opernd, data_file, FALSE, error_b, mem_cell->ln_address, line_number);
				create_address_command_code(mem_cell, operand_shift, adrs_modes[DIRECT].val_opcode);
				is_error = FALSE;
			}
			else
			{
				cur_adrs_mode = &adrs_modes[DIRECT];
			}
		}
	}
	if (is_error == TRUE)
	{
		sprintf(error_b, "%s invalid addressing on %s", cur_adrs_mode->val_name, command_name);
		data_file->error_list = error_join(data_file->error_list, error_b, line_number - LINE_BASE);
	}
}

void phase_two(IN_DATA_FILE* data_file) {
	char error_b[256];
	MEM_CELL* mem_cell;
	COMMAND_LIST_NODE* COMMAND_LIST_NODE;
	char* source_oper;
	char* dest_oper;
	char* source_addr;
	char* dest_addr;
	int line_number = LINE_BASE;
	char* command_name = NULL;
	symbol_table_change(data_file);
	entry_table_change(data_file, error_b);
	data_file->IC = LINE_BASE;
	COMMAND_LIST_NODE = data_file->command_hold_list;
	while (COMMAND_LIST_NODE)
	{
		mem_cell = (MEM_CELL*) malloc(sizeof(MEM_CELL));
		if (!mem_cell) {
			printf("Error Occurred: Memory allocation failed at phase_two!\n");
			exit(1);
		}
		mem_cell->ln_address = data_file->IC;
		mem_cell->type = ABSOLOTE;

		create_command_code(mem_cell, COMMAND_LIST_NODE->cmd_obj->contained_command);
		create_dbl_command_code(mem_cell, COMMAND_LIST_NODE->cmd_obj->dbl);
		create_type_command_code(mem_cell, COMMAND_LIST_NODE->cmd_obj->type_arr);
		data_file->key_memory = mem_join(data_file->key_memory, mem_cell);
		source_oper = COMMAND_LIST_NODE->cmd_obj->val_operands[0];
		dest_oper = COMMAND_LIST_NODE->cmd_obj->val_operands[1];

		if (COMMAND_LIST_NODE->cmd_obj->contained_command->val_operands == 1)
		{
			line_number = COMMAND_LIST_NODE->cmd_obj->line_number;
			dest_addr = COMMAND_LIST_NODE->cmd_obj->contained_command->dest_addr;
			command_name = COMMAND_LIST_NODE->cmd_obj->contained_command->val_name;
			oper_manage(dest_oper, dest_addr, data_file, mem_cell, DST_OPR_SHIFT, DST_REG_SHIFT, line_number, command_name);
		}
		else if (COMMAND_LIST_NODE->cmd_obj->contained_command->val_operands == 2)
		{
			source_addr = COMMAND_LIST_NODE->cmd_obj->contained_command->source_addr;
			dest_addr = COMMAND_LIST_NODE->cmd_obj->contained_command->dest_addr;
			line_number = COMMAND_LIST_NODE->cmd_obj->line_number;
			command_name = COMMAND_LIST_NODE->cmd_obj->contained_command->val_name;
			oper_manage(source_oper,
				source_addr,
				data_file,
				mem_cell,
				SRC_OPR_SHIFT,
				SRC_REG_SHIFT,
				line_number,
				command_name);
			oper_manage(dest_oper,
				dest_addr,
				data_file,
				mem_cell,
				DST_OPR_SHIFT,
				DST_REG_SHIFT,
				line_number,
				command_name);
		}
		COMMAND_LIST_NODE = COMMAND_LIST_NODE->next;
		++data_file->IC;
	}
}