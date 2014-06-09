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


/*!
 * Lecture d'un fichier binaire.
 *
 * Le fichier binaire a le format suivant :
 * 
 *    - 3 entiers non signés, la taille du segment de texte (\c textsize),
 *    celle du segment de données (\c datasize) et la première adresse libre de
 *    données (\c dataend) ;
 *
 *    - une suite de \c textsize entiers non signés représentant le contenu du
 *    segment de texte (les instructions) ;
 *
 *    - une suite de \c datasize entiers non signés représentant le contenu initial du
 *    segment de données.
 *
 * Tous les entiers font 32 bits et les adresses de chaque segment commencent à
 * 0. La fonction initialise complétement la machine.
 *
 * \param pmach la machine à simuler
 * \param programfile le nom du fichier binaire
 */
void read_program(Machine *mach, const char *programfile){
	
	FILE * f;
	unsigned int textsize, datasize, dataend;
	
	//on ouvre le programme en lecture seule. 
	if(!(f = fopen(programfile,'r'))){

		//Si on rencontre un problème, on quitte l'execution
		perror("Erreur lors de l'ouverture du fichier dans read_program");
		exit(1);

	}

	//on lit les 3 entiers non signés textsize, datasize et dataend. 
	if((fread(&textsize, sizeof(unsigned int), 4, f)<0) 
		|| (fread(&datasize, sizeof(unsigned int), 4, f)<0)
		|| (fread(&dataend, sizeof(unsigned int), 4, f)<0)){

		//Si on rencontre un problème, on quitte l'exécution. 
		perror("Erreur lors de la lecture des entiers non signés dans read_program");
		exit(1);

	}

	//On vérifie que la taille pour la pile est suffisante, sinon on la modifie.
	if((int stack_size = datasize - dataend) < MINSTACKSIZE){
		
		stack_size = MINSTACKSIZE;
	
	}

	//On alloue la mémoire nécessaire à l'accueil des instructions du programme.
	if(!(Instruction * text = (Instruction *) calloc(textsize, sizeof(Instruction)))){
		
		//Si une erreur se produit, on quitte l'exécution.
		perror("Erreur d'allocation mémoire pour le segment text dans read_program");
		exit(1);

	} 

	//On alloue la mémoire nécessaire à l'accueil des instructions du programme.
	if(!(Word * data = (Word *) calloc(dataend + stacksize, sizeof(Word)))){
		
		//Si une erreur se produit, on quitte l'exécution.
		perror("Erreur d'allocation mémoire pour le segment data dans read_program");
		exit(1);

	}
	//On lit les instructions du programme
	if(fread(text, sizeof(Instruction), textsize, f)<0){

		//En cas d'erreur, on quitte l'exécution
		perror("Erreur lors de la lecture des instructions dans read_program");
		exit(1);

	}

	//On lit les données du programme
	if(fread(data, sizeof(Word), datasize, f)<0){

		//En cas d'erreur, on quitte l'exécution
		perror("Erreur lors de la lecture des données dans read_program");
		exit(1);

	}

	//On appelle la fonction fclose() de stio.h et on ferme proprement le fichier.
	fclose(f);

	//on réinitialise la machine avec les nouvelles données du programme
	load_program(mach, textsize, text, dataend+stack_size, data, dataend);

}