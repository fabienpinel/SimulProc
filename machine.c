/*!
 * \file machine.c 
 * \brief {Implémentation de la structure processeur et de sa mémoire.}
 * \author {L. GIN, A. EL-AMRANI, F. PINEL, C. BOINAUD}
 * \date 09.06.14
 *
 * Implémentation du fichier d'entête machine.h - Implémentation de la structure processeur et de sa mémoire.
 *
 */

#include <stdio.h>
#include "machine.h"

/*!
 * La machine est réinitialisée et ses segments de texte et de données sont
 * remplacés par ceux fournis en paramètre.
 *
 * \param pmach la machine en cours d'exécution
 * \param textsize taille utile du segment de texte
 * \param text le contenu du segment de texte
 * \param datasize taille utile du segment de données
 * \param data le contenu initial du segment de texte
 */
void load_program(Machine *pmach,
                  unsigned textsize, Instruction text[textsize],
                  unsigned datasize, Word data[datasize],  unsigned dataend){

	//réinitialisation du segment de texte
	pmach->_text = text;
	pmach ->_textsize = textsize;

	//réinitialisation du segment de donnée
	pmach->_data = data;
	pmach->_datasize = datasize;
	pmach->_dataend = dataend;

	//réinitialisation des registres R0..R14 à 0
	for(int i = 0 ; i<NREGISTERS-1 ; i++){
		pmach->_registers[i] = 0;
	}

	//réinitialisation du code condition à U (unknown)
	pmach->_cc = CC_U;

	//réinitialisation du compteur ordinal à 0
	pmach->_pc = 0;

	//réinitialisation du registre R15
	pmach->_sp = datasize-1;

}

