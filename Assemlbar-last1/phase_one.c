/**********************************************************************************************
File:			phase_one.c
Developers:		Bar Eitan
				Daniel Ashual
Description:	Firstly, parses ".as" files validates text and translates to needed data
				Parsing of as files, text validation and data convertion to structures
Creation Date:	March 18, 2016
**********************************************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "phase_one.h"

extern GUIDENCE_STRUCT guidences[];
extern COMMAND_STRUCT commands[];
extern REGISTER_STRUCT registers[];
extern MODEADD adrs_modes[];

enum guidences_index {
	data_index,
	string_index,
	entry_index,
	extern_index
};

void phase_one(IN_DATA_FILE* data_file)
{
	int if_val_symbol;						
	unsigned int line_number = LINE_BASE;   
	int i;
	char  line[LINE_SIZE];			
	char  val_symbol[SYM];		
	char* temp_token = NULL;			
	char  error_buf[256];		
	while ((fgets(line, LINE_SIZE - 1, data_file->input_file)) != NULL)
	{
		++line_number;
		if_val_symbol = FALSE;


		if (line[0] == COMMENT)
			continue;
		for (i = 0; line[i] && isspace(line[i]); ++i)
			;

		if (line[i] == '\0')
			continue;

		if (strchr(line, SYM_DELIM))
		{
			temp_token = strtok(line, DELIM);
			if (temp_token) {
				if_val_symbol = TRUE;
			 }
		}

		if (if_val_symbol == TRUE)
		{
			if (symb_transfer(val_symbol, temp_token) == FALSE)
			{
				sprintf(error_buf, "Unknown symbol \"%s\"", temp_token);
				data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
				continue;
			}
		}

		if (if_val_symbol == TRUE) {
			temp_token = strtok(NULL, DELIM);
		}
		else
			temp_token = strtok(line, DELIM);

		if (strcmp(temp_token, guidences[data_index].val_name) == 0)
		{

			num_management(data_file, if_val_symbol, temp_token, error_buf, line_number, val_symbol);
			++data_file->SC;

		}
		else if (strcmp(temp_token, guidences[string_index].val_name) == 0)
		{
			string_management(data_file, if_val_symbol, temp_token, error_buf, line_number, val_symbol);
			++data_file->SC;
		}
		else if (strcmp(temp_token, guidences[entry_index].val_name) == 0)
		{

			ent_ext_management(data_file, temp_token, error_buf, line_number, 1);
		}
		else if (strcmp(temp_token, guidences[extern_index].val_name) == 0)
		{

			ent_ext_management(data_file, temp_token, error_buf, line_number, 0);
		}

		// Else, process it as it is a command.
		else
		{
			command_manage(data_file, if_val_symbol, temp_token, error_buf, line_number, val_symbol);
		}
	}
	int j = 10000;	
	while (j) {
		j--;
	}
}

int symb_transfer(char* destination_symbol, char* source_symbol) {
	int length;
	length = strlen(source_symbol);
	if (length > 30 || !isalpha(source_symbol[0]))
		return FALSE;
	strcpy(destination_symbol, source_symbol);
	return TRUE;
}

void num_management(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol)
{
	char *endp;
	MEM_CELL* mem_cell;
	int num;
	if (if_val_symbol == TRUE)
		data_file->symbol_table = symb_join(data_file->symbol_table, val_symbol, 0, data_file->DC);
	temp_token = strtok(NULL, DATA_DELIM);
	while (temp_token != NULL)
	{
		num = strtol(temp_token, &endp, BASE);
		if (*endp != '\0')
		{
			sprintf(error_buf, "\"%s\" is used. Number is expected", temp_token);
			data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
			temp_token = strtok(NULL, DATA_DELIM);
			continue;
		}
		mem_cell = (MEM_CELL*) malloc(sizeof(MEM_CELL));
		if (!mem_cell)
		{
			printf("Error Occurred: Memory allocation failed\n");
			exit(1);
		}
		mem_cell->ln_address = data_file->DC;
		num = negative(num);
		mem_cell->code = num;
		mem_cell->type = NONE;
		data_file->content_memory = mem_join(data_file->content_memory, mem_cell);
		data_file->DC++;
		temp_token = strtok(NULL, DATA_DELIM);
	}
}

void string_management(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol) {
	MEM_CELL* mem_cell;
	int str_index;

	if (if_val_symbol == TRUE)
		data_file->symbol_table = symb_join(data_file->symbol_table, val_symbol, 0, data_file->DC);
	temp_token = NULL;
	temp_token = strtok(NULL, STR_DELIM);

	while (temp_token != NULL) {
		str_index = 0;
		while (temp_token[str_index] != '\0') {
			mem_cell = (MEM_CELL*) malloc(sizeof(MEM_CELL));
			if (!mem_cell) {
				printf("Error Occurred: Memory allocation failed at phase_one!\n");
				exit(1);
			}
			mem_cell->ln_address = data_file->DC;
			mem_cell->code = temp_token[str_index];
			mem_cell->type = NONE;
			data_file->content_memory = mem_join(data_file->content_memory, mem_cell);
			str_index++;
			data_file->DC++;
		}
		mem_cell = (MEM_CELL*) malloc(sizeof(MEM_CELL));
		if (!mem_cell) {
			printf("Error Occurred: Memory allocation failed at phase_one!\n");
			exit(1);
		}
		mem_cell->ln_address = data_file->DC;
		mem_cell->code = '\0';
		mem_cell->type = NONE;
		data_file->content_memory = mem_join(data_file->content_memory, mem_cell);
		data_file->DC++;
		temp_token = strtok(NULL, STR_DELIM);
		if (temp_token)
		{
			sprintf(error_buf, "Less String arguments are expected");
			data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
			break;
		}
	}
}

void ent_ext_management(IN_DATA_FILE* data_file, char *temp_token, char* error_buf, int line_number, int ent_ext_sym) {
	if (ent_ext_sym == 1)
	{
		char *entry_symbol = NULL;
		temp_token = strtok(NULL, SPACE_DELIM);
		if (temp_token) {
			entry_symbol = temp_token;
			temp_token = strtok(NULL, SPACE_DELIM);
			if (temp_token)
			{
				sprintf(error_buf, "Less tokens are expected: %s", temp_token);
				data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
			}
			else {
				data_file->entry_table = entry_join(data_file->entry_table, entry_symbol, NO_ADDRESS);
			}
		}
	}
	else
	{
		char* ext_symbol = NULL;
		temp_token = strtok(NULL, SPACE_DELIM);
		if (temp_token != NULL)
		{
			ext_symbol = temp_token;
			temp_token = strtok(NULL, SPACE_DELIM);
			if (temp_token)
			{
				sprintf(error_buf, "Less tokens are expected: %s", temp_token);
				data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
			}
			else
			{
				data_file->extern_table = external_join(data_file->extern_table, ext_symbol, NO_ADDRESS);
			}
		}
	}
}

void command_manage(IN_DATA_FILE* data_file, int if_val_symbol, char *temp_token, char* error_buf, int line_number, char* val_symbol)
{
	COMMAND_CONTAINER* cmd_obj;
	int operands_index = 0;
	int typedbl_index = 0;
	COMMAND_STRUCT* command_struct;
	COMMAND_STRUCT* contained_command;
	int reg_index = 0;

	if (if_val_symbol == TRUE)
		data_file->symbol_table = symb_join(data_file->symbol_table, val_symbol, 1, data_file->IC);

	cmd_obj = (COMMAND_CONTAINER*) malloc(sizeof(COMMAND_CONTAINER));
	if (!cmd_obj) {
		printf("Error Occurred: Memory allocation failed at phase_one !\n");
		exit(1);
	}

	contained_command = commands;
	while (contained_command->val_opcode != NOT_VALID)
	{
		if (strcmp(temp_token, contained_command->val_name) == 0)
			break;
		++contained_command;
	}
	if (contained_command->val_opcode == NOT_VALID)
		command_struct =  NULL;
	command_struct = contained_command;

	if (command_struct == NULL)
	{
		sprintf(error_buf, "Unknown command: %s", temp_token);
		data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
	}
	else
	{
		symb_transfer(cmd_obj->val_symbol, val_symbol);
		cmd_obj->if_val_symbol = TRUE;
		temp_token = strtok(NULL, OPR_DELIM);
		if (temp_token)
		{
			cmd_obj->dbl = 0;
			if (atoi(temp_token) == 1)
			{
				cmd_obj->type_arr[FLAG] = 1;
				typedbl_index = 1;
				while (temp_token && typedbl_index < ARGS)
				{

					temp_token = strtok(NULL, OPR_DELIM);
					cmd_obj->type_arr[typedbl_index] = atoi(temp_token);
					typedbl_index++;
				}
				temp_token = strtok(NULL, OPR_DELIM);
				if (temp_token)
				{
					cmd_obj->dbl = atoi(temp_token);
				}
			}
			else if (atoi(temp_token) == 0)
			{
				cmd_obj->type_arr[FLAG] = 0;
				cmd_obj->type_arr[CMB_LEFT] = 0;
				cmd_obj->type_arr[CMB_RIGHT] = 0;
				temp_token = strtok(NULL, OPR_DELIM);
				if (temp_token)
				{
					cmd_obj->dbl = atoi(temp_token);
				}


			}

		}

		if (temp_token)
		{

			while (temp_token && operands_index < command_struct->val_operands)
			{
				temp_token = strtok(NULL, OPR_DELIM);
				if (command_struct->val_operands == 1)
					strcpy(cmd_obj->val_operands[1], temp_token);
				else
					strcpy(cmd_obj->val_operands[operands_index], temp_token);


				++operands_index;

				while (registers[reg_index].num != NOT_VALID)
				{
					if (strcmp(temp_token, registers[reg_index].val_name) == 0
							|| (temp_token[0] == '@' && strcmp(temp_token + 1,
						registers[reg_index].val_name) == 0))
					{
						break;
					}
					reg_index++;
				}
				if (registers[reg_index].num == NOT_VALID)
					data_file->IC++;

				reg_index = 0;
			}

		}
		if (operands_index > command_struct->val_operands)
		{
			sprintf(error_buf, "Different number of operands is required for the command \"%s", command_struct->val_name);
			data_file->error_list = error_join(data_file->error_list, error_buf, line_number - LINE_BASE);
			free(cmd_obj);
		}
		else
		{
			cmd_obj->contained_command = command_struct;
			cmd_obj->line_number = line_number;
			data_file->command_hold_list = command_join(data_file->command_hold_list, cmd_obj);
			data_file->IC++;
		}
	}
}