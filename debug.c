/*!
 * \file debug.c
 * \brief Fonctions de mise au point interactive.
 * \author {L. GIN, A. EL-AMRANI, F. PINEL, C. BOINAUD}
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

#include "debug.h"

void options(void){
	printf("Available commands:\nh	help\nc	continue (exit interactive debug mode)\ns	step by step (next instruction)\nRET	step by step (next instruction)\nr	print registers\nd	print data memory\nt	print text (program) memory\np	print text (program) memory\nm	print registers and data memory\n");
}

//! Dialogue de mise au point interactive pour l'instruction courante.
/*!
 * Cette fonction gère le dialogue pour l'option \c -d (debug). Dans ce mode,
 * elle est invoquée après l'exécution de chaque instruction.  Elle affiche le
 * menu de mise au point et on exécute le choix de l'utilisateur. Si cette
 * fonction retourne faux, on abandonne le mode de mise au point interactive
 * pour les instructions suivantes et jusqu'à la fin du programme.
 * 
 * \param mach la machine/programme en cours de simulation
 * \return vrai si l'on doit continuer en mode debug, faux sinon
 */
bool debug_ask(Machine *pmach){
	int c;
	while(true){
		printf("DEBUG? ");
        c = getchar();

        switch(c){
        	/* step by step (next instruction) */
        	case '\n': return true;
        	/* continue (exit interactive debug mode) */
        	case 'c': return false;
        	/* help */
        	case 'h': 
        		options();
        		break;
        	/* step by step (next instruction) */
        	case 's': return true;
        	/* print registers */
        	case 'r':
        		print_cpu(pmach);
        		break;
        	/* print data memory */
        	case 'd':
        		print_data(pmach);
        		break;
        	/* print text (program) memory */
        	case 't':
        	case 'p':
        		print_program(pmach);
        		break;
        	/* print registers and data memory */
        	case 'm':
        		print_cpu(pmach);
        		print_data(pmach);
        		break;
        	default:
        		break;

        }
	}
	printf("\n");
	return false;

}

