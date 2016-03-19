/**********************************************************************************************																							   *
File:				main.c
Developers:			Bar Eitan
					Daniel Ashual
Description:		The program parses .as files and coverts them to maching code
					The parsing process is achieved in two stages:
					Phase 1 - Reading and validating the input file, commands and the syntax.
					Phase 2 - Parsing and translation of the data to memory cells + creation of ob, ext and ent files.
Creation date:		March 3, 2016
**********************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "structs_config.h"
#include "toolkit.h"
#include "phase_one.h"
#include "phase_two.h"
#include "pragma_structure.h"

extern GUIDENCE_STRUCT guidences[];
extern COMMAND_STRUCT commands[];
extern REGISTER_STRUCT registers[];
extern MODEADD adrs_modes[];

int main(int argc, char* argv[])
{

	char file_name[NAME_SIZE];
	int i;
	IN_DATA_FILE* data_file;

	for (i = 1; i < argc; ++i) {
		printf("Processing <%s.as>\n", argv[i]);
		strncpy(file_name, argv[i], NAME_SIZE - 3);
		strcat(file_name, AS_SUFFIX);

		if ((data_file = (IN_DATA_FILE*) malloc(sizeof(IN_DATA_FILE))) == NULL) {
			printf("Error Occurred: File creation failed. Could not allocate memory.\n");
			exit(1);
		}
		in_data_file_initialize(data_file);
		if ((data_file->input_file = fopen(file_name, "r")) == NULL) {
			printf("Error: could not read the requested file <%s>\n\n", argv[i]);
			continue;
		}

		phase_one(data_file);
		fclose(data_file->input_file);
		phase_two(data_file);
		data_change(data_file);
		code_change(data_file);

		if (data_file->error_list == NULL)
		{
			create_output(argv[i], data_file);
			printf("***********************\n");
			printf("***********************\n");
			printf("***********************\n");
			printf("Assembly completed successfully on file <%s>\n\n", argv[i]);
		}
		else	
			error_prnt_file(data_file->error_list, argv[i]);
        
		//Free all memory methods
		memory_free(data_file->content_memory);
		memory_free(data_file->key_memory);
		extern_free(data_file->extern_table);
		extern_free(data_file->extern_list);
		entry_table_free(data_file->entry_table);
		symbol_table_free(data_file->symbol_table);
		error_free(data_file->error_list);
		command_list_free(data_file->command_hold_list);
		free(data_file);
	}
	return 0;
}

