/*!
 * \file debug.c
 * \brief Fonctions de mise au point interactive.
 * \author {L. GIN, A. EL-AMRANI, F. PINEL, C. BOINAUD}
 */
#include <stdbool.h>

#include "machine.h"

#include "debug.h"


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
	while(true){
		printf("DEBUG? ");
        c = getchar();

        switch(c){
        	case '\n': return true; break;
        	case 'c': return false; break;



        }




	}
	printf("\n");
	return false;

}