#include "machine.h"

/*
 * Test BRANCH en immediat
 */

//! Instruction inconnue
Instruction text[] = {
    {.instr_absolute =  {ADD, true, true,	1, 	0	}},  
    {.instr_absolute =  {SUB, false, false,	1, 	0	}},  
    {.instr_absolute =  {BRANCH, false, false,	GE,	4	}},  
    {.instr_absolute =  {NOP,                    	}},  
    {.instr_absolute =  {STORE, false, false,	1, 	10	}}, 
    {.instr_absolute =  {HALT, 						}},  
};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Segment de données initial
Word data[20] = {
    5,  
    0,  
    0, 
    0,  
};

//! Fin de la zone de données utile
const unsigned dataend = 10;

//! Taille utile du segment de données
const unsigned datasize = sizeof(data) / sizeof(Word);  