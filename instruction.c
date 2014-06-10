/*!
 * \file machine.c 
 * \brief {description et affichage des instructions .}
 * \author {L. GIN, A. EL-AMRANI, F. PINEL, C. BOINAUD}
 * \date 10.06.2014
 *
 *
 */

    
#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 //! Tous les codes  conditions
 const char* condition_names[] = { "NC", "EQ", "NE", "GT", "GE", "LT", "LE" };

 //! Tous les codes des operations : 
 const char* cop_names[] = { "ILLOP", "NOP", "LOAD", "STORE", "ADD", "SUB", "BRANCH", "CALL", "RET", "PUSH", "POP", "HALT" };



 //! Impression du code condition .
/*!
 * \param instr l'instruction corespondante 
 */

 void print_condition(Instruction instr){

 	printf("%s", condition_names[instr.instr_generic._regcond] );
 } 

 //! Impression d'un registre  .
/*!
 * \param instr l'instruction corespondante 
 */

 void print_registre(Instruction instr){

 	printf("%d",instr.instr_generic._regcond );
 }

 //! Impression de l'operande   .
/*!
 * \param instr l'instruction corespondante 
 */

 void print_operande(Instruction instr){
 	if(instr.instr_generic._immediate){
 		printf("%s",instr.instr_immediate._value); // I = 1 => operande = val ; 
 	} else {
 		 if(instr.instr_generic._indexed){
 		 	printf("%d[R%d"],instr.instr_indexed._offset,instr.instr_indexed._rindex ); // I=0 & X=1 => adr = (RX)+Offset 
 		 } else {
 		 	printf("%s@0x", instr.instr_absolute._address ); // I=0 & X=1 => adr = abs ;

 		 }
 	}
 }
