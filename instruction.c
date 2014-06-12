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

 	printf("%s, ", condition_names[instr.instr_generic._regcond] );
 } 
 
 
 //! Impression du code operande .
/*!
 * \param instr l'instruction corespondante 
 */
  void print_code_op(Instruction instr){

	printf("%s ",cop_names[instr.instr_generic._cop]);
}
 
 

 //! Impression d'un registre  .
/*!
 * \param instr l'instruction corespondante 
 */

 void print_registre(Instruction instr){
	if(instr.instr_generic._regcond>10)
 	printf("R%d ",instr.instr_generic._regcond );
 else 
 	printf("R0%d ",instr.instr_generic._regcond );
 }

 //! Impression de l'operande   .
/*!
 * \param instr l'instruction corespondante 
 */

 void print_operande(Instruction instr){
 	if(instr.instr_generic._immediate){
 		printf("#%d",instr.instr_immediate._value); // I = 1 => operande = val ; 
 	} else {
 		 if(instr.instr_generic._indexed){
 		 	printf("%d[R%d]",instr.instr_indexed._offset,instr.instr_indexed._rindex ); // I=0 & X=1 => adr = (RX)+Offset 
 		 } else {
			
 		 	printf("@0x%.4x", instr.instr_absolute._address ); // I=0 & X=1 => adr = abs ;

 		 }
 	}
 }
 
void print_instruction(Instruction instr, unsigned addr){
	switch (instr.instr_generic._cop) {
		case LOAD:
		case STORE:
		case ADD:
		case SUB:
			print_code_op(instr) ;
			print_registre(instr); 
			print_operande(instr);
			break ; 
			
		case BRANCH : 
		case CALL:
			print_code_op(instr) ;
			print_condition(instr); 
			print_operande(instr);
			break ; 
	    case PUSH:
		case POP:
			print_code_op(instr) ; 
			print_operande(instr);
			break;
		case ILLOP:
		case NOP:
		case RET:
		case HALT:
		print_code_op(instr) ; 
			break;		
		}
}
