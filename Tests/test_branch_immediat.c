#include "machine.h"

/*
 * Test BRANCH en immediat
 */

//! Instruction inconnue
Instruction text[] = {
    {.instr_absolute =  {BRANCH, true, false,	0, 	0	}},  
};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Segment de données initial
Word data[20] = {
    0,  
    0,  
    0, 
    0,  
};

//! Fin de la zone de données utile
const unsigned dataend = 10;

//! Taille utile du segment de données
const unsigned datasize = sizeof(data) / sizeof(Word);  