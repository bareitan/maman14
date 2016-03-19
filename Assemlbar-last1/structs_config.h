/***********************************************************************************************
File:				structs_config.h
Developers:			Bar Eitan
					Daniel Ashual
Description:		Struct definitions                   				   
Creation Date:		March 18,2016
**********************************************************************************************/


#ifndef STRUCTS_CONFIG
#define STRUCTS_CONFIG 1
#include <stdio.h>

/*******************************************	MACROS */

#define LINE_BASE		100			/* Memory encoding address		*/
#define ABSOLOTE		'a'			/* Absolute memory			*/
#define RELOCATEABLE	'r'				/* Relocatable memory			*/
#define EXTERN			'e'			/* External memory			*/
#define	NONE			' '			/* No type assigned			*/
#define SYM				32		/* Max symbol size			*/
#define NAME_SIZE		32			/* Max file name			*/
#define OPERAND_SIZE	16				/* Operand size				*/
#define COMMAND_SIZE   	5				/* Command size				*/
#define NOT_VALID		-1			/* Invalid flag				*/
#define FLAG			0			/* Type flag parameter			*/
#define CMB_LEFT		1			/* comb left  bit parameter		*/
#define CMB_RIGHT		2			/* comb right bit parameter		*/
#define ARGS			3			/* Allowed number of command parameters */


/*******************************************	PROTOTYPES */


/* 
 * COMMAND_STRUCT : defines a command structure.
 */
typedef struct COMMAND_STRUCT {
	char val_name[COMMAND_SIZE];			/* Command's name			       */
	int val_opcode; 				/* Command's op-code			       */
	int val_operands; 				/* Command's max valid parameter number        */
	char* source_addr; 				/* Command's valid source  addressing mode     */
	char* dest_addr; 				/* Command's valid destination addressing mode */
} COMMAND_STRUCT;


/* 
 * GUIDENCE_STRUCT : defines a type of instruction structure.
 */
typedef struct GUIDENCE_STRUCT {
	char* val_name; 				/* Instruction guide name */
} GUIDENCE_STRUCT;


/* 
 * REGISTER_STRUCT : defines a type of register structure.
 */
typedef struct REGISTER_STRUCT {
	char* val_name; 				/* reg name  */
	int num; 					/* reg number */
} REGISTER_STRUCT;


/* 
 *COMMAND_CONTAINER : defines a type of command assistance holder structure. 
 * Command is being hold once it is parsed.
 */
typedef struct COMMAND_CONTAINER {			
	char val_symbol[SYM];				/* Command's tokens				     */
	COMMAND_STRUCT* contained_command; 		/* Command's instructions	           	     */
	char val_operands[2][OPERAND_SIZE];		/* Command's operators		                     */
	int if_val_symbol; 				/* Command's symbol flag  : determines if has symbol */
	int line_number;				/* Command's line number location in the .as file    */
	int dbl;					/* Command's dbl			             */
	int type_arr[3];				/* Command's type				     */
} COMMAND_CONTAINER;


/* 
 * MODEADD : defines a type of address mode structure.
 */
typedef struct MODEADD {
	char* val_name; 				/* Address mode name		*/
	int val_opcode;					/* Address mode operation code */
	char val_symbol; 				/* Address mode symbol         */
} MODEADD;


/* 
 * _mem_cell : defines a memroy cell for machine code values structure.
 */
typedef struct _mem_cell {			
	int ln_address; 				/* Cell's line address in the .ob file  */
	int code;					/* Cell's machine code		        */
	char type; 					/* Cell's type: 'a', 'r' or 'e'		*/
} MEM_CELL;


/* 
 * COMMAND_LIST_NODE : defines a linked list to hold all commands structure.
 */
typedef struct COMMAND_LIST_NODE COMMAND_LIST_NODE;
struct COMMAND_LIST_NODE {
	COMMAND_LIST_NODE* next;		/* Next command in the list */
	COMMAND_CONTAINER* cmd_obj;		/* Contained command object */
};


/* 
 * SYMBOL_LIST_NODE : defines a linked list to hold all tokens structure.
 */
typedef struct SYMBOL_LIST_NODE SYMBOL_LIST_NODE;	// symbol linked list

struct SYMBOL_LIST_NODE {
	SYMBOL_LIST_NODE* next;			/* Next token      */
	char val_label[SYM]; 			/* Token name      */
	int symbol_kind; 			/* Token's data    */
	int symbol_addr; 			/* Token's address */
};


/* 
 * MEMORY_LIST_NODE : defines a linked list to merge all memory cells structure.
 */
typedef struct MEMORY_LIST_NODE MEMORY_LIST_NODE;	// list of memory cells

struct MEMORY_LIST_NODE {
	MEMORY_LIST_NODE* next;				/* Next memory cell	 */
	MEM_CELL* cell; 				/* Contained memory cell */
};


/* 
 *ENTRY_LIST_NODE : defines a linked list to hold all entry lines structure.
 */
typedef struct ENTRY_LIST_NODE ENTRY_LIST_NODE;	// entries table

struct ENTRY_LIST_NODE {
	ENTRY_LIST_NODE* next;			/* Next entry line      */
	char val_label[SYM];			/* Contained entry line */
	int symbol_kind;			/* Entry type           */
	int symbol_addr;			/* Entry dec address    */
};


/* 
 * EXTERN_LIST_NODE : defines a linked list to hold all external typed lines structure.
 */
typedef struct EXTERN_LIST_NODE EXTERN_LIST_NODE;	// external table

struct EXTERN_LIST_NODE {			
	EXTERN_LIST_NODE* next;				/* Next external line	   */
	char val_label[SYM];				/* Contained external line */	
	int symbol_addr;				/* external line's address */
};


/* 
 * ERROR_LIST_NODE : defines a linked list to hold all errors found durring assembling process structure.
 */
typedef struct ERROR_LIST_NODE ERROR_LIST_NODE;	// errors list

struct ERROR_LIST_NODE {
	ERROR_LIST_NODE* next;			/* Next assembling process error */
	unsigned int line_number; 		/* Contained error message       */
	char error_log[256]; 			/* Error number			 */
};


/* 
 * IN_DATA_FILE : defines a type of assembler parsing structure. 
 * assembls all values in a file, contains all needed data to build a two step parses assembler.
 */
typedef struct IN_DATA_FILE {			
	MEMORY_LIST_NODE* content_memory;			/* Data list			  */
	MEMORY_LIST_NODE* key_memory;				/* Code list			  */
	EXTERN_LIST_NODE* extern_table;				/* External token table		  */
	EXTERN_LIST_NODE* extern_list;				/* External creator file aider    */
	ENTRY_LIST_NODE* entry_table;				/* Entries token table		  */
	SYMBOL_LIST_NODE* symbol_table;				/* Tokens table			  */
	ERROR_LIST_NODE* error_list;				/* Error list			  */
	FILE* input_file;					/* ".as" input file		  */ 
	COMMAND_LIST_NODE* command_hold_list; 			/* Command holder list            */
	unsigned int IC;					/* Instruction counter            */
	unsigned int DC;					/* Data counter			  */
	unsigned int SC;					/* Symbol counter                 */
} IN_DATA_FILE;

#endif
