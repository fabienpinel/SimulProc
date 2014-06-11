#include "machine.h"

/*!
 * Test segmentation error data
 */

Instruction text[] = {
    {.instr_absolute =   {LOAD,      false, false,  0,  0   }}, 
    {.instr_absolute =   {LOAD,      false, false,  1,  1   }}, 
    {.instr_immediate = {SUB,    true,  false,  1,  1   }},  
    {.instr_absolute =  {BRANCH, false, false,  LE,     7   }},
    {.instr_indexed =   {ADD,    false, false,  0,  0   }}, 
    {.instr_immediate = {SUB,    true,  false,  1,  1   }},  
    {.instr_absolute =  {BRANCH, false, false,  NC,     3   }},   
    {.instr_absolute =  {STORE,  false, false,  0,  2   }}, 
    {.instr_generic =   {HALT,                  }},  
    {.instr_generic =   {ILLOP,                 }},  
    {.instr_generic =   {ILLOP,                 }},  
    {.instr_generic =   {ILLOP,                 }},  
};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Premier exemple de segment de données initial
Word data[20] = {
    10, 
    5,  
    20, 
    0,  
};

//! Fin de la zone de données utiles
const unsigned dataend = 5;

//! Taille utile du segment de données
const unsigned datasize = 2;  
