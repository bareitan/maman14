/*********************************************************************************************
File:				pragma_structure.h
Developers:			Bar Eitan
					Daniel Ashual
Description:		Structures definitions
Creation Date:		March 18, 2016
**********************************************************************************************/

#include <stdio.h>
#include "toolkit.h"

/* 
 * Guiding structure : defines a type of guide structure.
 */
GUIDENCE_STRUCT guidences[] =
{
	{ ".data" },
	{ ".string" },
	{ ".entry" },
	{ ".extern" }
};

/*
 * ADRS_MOD : defines a type of address structure.
 */
MODEADD adrs_modes[] =
{
	{ "Immediate", 0, '#' },
	{ "Direct", 1, '\0' },
	{ "Index", 2, '*' },
	{ "Direct Register", 3, '\0' },
	{ "not_valid", NOT_VALID, '\0' }
};

/* 
 * CMD : defines a type of command strucutre.
 */
COMMAND_STRUCT commands[] = {
	{ "mov", 0, 2, "0123", "123" },
	{ "cmp", 1, 2, "0123", "0123" },
	{ "add", 2, 2, "0123", "123" },
	{ "sub", 3, 2, "0123", "123" },
	{ "not", 4, 1, NULL, "123" },
	{ "clr", 5, 1, NULL, "123" },
	{ "lea", 6, 2, "123", "123" },
	{ "inc", 7, 1, NULL, "123" },
	{ "dec", 8, 1, NULL, "123" },
	{ "jmp", 9, 1, NULL, "123" },
	{ "bne", 10, 1, NULL, "123" },
	{ "red", 11, 1, NULL, "123" },
	{ "prn", 12, 1, NULL, "0123" },
	{ "jsr", 13, 1, NULL, "1" },
	{ "rts", 14, 0, NULL, NULL },
	{ "stop", 15, 0, NULL, NULL },
	{ "inv", NOT_VALID, NOT_VALID, NULL, NULL }
};

/* 
 * REG structure : defines a type of register structure.
 */
REGISTER_STRUCT registers[] = {
	{ "r0", 0 },
	{ "r1", 1 },
	{ "r2", 2 },
	{ "r3", 3 },
	{ "r4", 4 },
	{ "r5", 5 },
	{ "r6", 6 },
	{ "r7", 7 },
	{ "not_valid", NOT_VALID }
};

