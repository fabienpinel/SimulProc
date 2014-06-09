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
#include <stdlib.h>
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
	if(!(f = fopen(programfile,"r"))){

		//Si on rencontre un problème, on quitte l'execution
		perror("Erreur lors de l'ouverture du fichier dans <machine.c:read_program>");
		exit(1);

	}

	//on lit les 3 entiers non signés textsize, datasize et dataend. 
	if((fread(&textsize, sizeof(unsigned int), 4, f)<0) 
		|| (fread(&datasize, sizeof(unsigned int), 4, f)<0)
		|| (fread(&dataend, sizeof(unsigned int), 4, f)<0)){

		//Si on rencontre un problème, on quitte l'exécution. 
		perror("Erreur lors de la lecture des entiers non signés dans <machine.c:read_program>");
		exit(1);

	}

	//variable définissant la taille de la pile
	int stack_size;

	//On vérifie que la taille pour la pile est suffisante, sinon on la modifie.
	if((stack_size = datasize - dataend) < MINSTACKSIZE){
		
		stack_size = MINSTACKSIZE;
	
	}

	//variable contenant les instuctions du programme
	Instruction * text;

	//On alloue la mémoire nécessaire à l'accueil des instructions du programme.
	if(!(text = (Instruction *) calloc(textsize, sizeof(Instruction)))){
		
		//Si une erreur se produit, on quitte l'exécution.
		perror("Erreur d'allocation mémoire pour le segment text dans <machine.c:read_program>");
		exit(1);

	} 
	//On lit les instructions du programme
	else if(fread(text, sizeof(Instruction), textsize, f)<0){

		//En cas d'erreur, on quitte l'exécution
		perror("Erreur lors de la lecture des instructions dans <machine.c:read_program>");
		exit(1);

	}

	//variable contenant les données du programme
	Word * data;

	//On alloue la mémoire nécessaire à l'accueil des instructions du programme.
	if(!(data = (Word *) calloc(dataend + stack_size, sizeof(Word)))){
		
		//Si une erreur se produit, on quitte l'exécution.
		perror("Erreur d'allocation mémoire pour le segment data dans <machine.c:read_program>");
		exit(1);

	} 
	//On lit les données du programme
	else if(fread(data, sizeof(Word), datasize, f)<0){

		//En cas d'erreur, on quitte l'exécution
		perror("Erreur lors de la lecture des données dans <machine.c:read_program>");
		exit(1);

	}

	//On appelle la fonction fclose() de stio.h et on ferme proprement le fichier.
	fclose(f);

	//on réinitialise la machine avec les nouvelles données du programme
	load_program(mach, textsize, text, dataend+stack_size, data, dataend);

}

/*!
 * Délégation de l'affichage du programme et des données par dump_memory()
 * 
 * \param pmach la machine en cours d'exécution
 */
void dump_print(Machine *pmach){
	
	//impression des instructions
	printf("Instruction text[] = {\n");

	for(int i = 0 ; i< pmach->_textsize; i++){

		//si on commence une nouvelle ligne
		if(i%4 == 0) printf("	");
		
		//affichage d'une instruction
		printf("Ox%08x, ",(pmach->_text[i])._raw);
		
		//au bout de 4 instructions, on change de ligne
		if((i%4 == 3) || (i > pmach->_textsize -1)) printf("\n");
	}

	printf("};\nint textsize = %d\n\n", pmach->_textsize);

	//imnpression des donnéess
	printf("Word data[] = {\n");

	for(int i = 0 ; i< pmach->_datasize; i++){

		//si on commence une nouvelle ligne
		if(i%4 == 0) printf("	");
		
		//affichage d'une donnée
		printf("Ox%08x, ",pmach->_data[i]);
		
		//au bout de 4 données, on change de ligne
		if((i%4 == 3) || (i > pmach->_datasize -1)) printf("\n");
	}

	printf("};\nint datasize = %d", pmach->_datasize);
}

/*!
 * Délégation de la création du dump binaire par dump_memory()
 * 
 * \param pmach la machine en cours d'exécution
 */
void dump_create(Machine *pmach){
	FILE * f;

	//ouverture en mode ecriture (création si inexistant, remplace sinon)
	if(!(f = fopen("dump.bin","w+"))){

		//si le fichier n'a pas été ouvert, on quitte l'exécution
		perror("Erreur lors de l'ouverture du fichier dump.bin dans <machine.c:dump_create>");
		exit(1);
	}

	//écriture de textsize
	if(fwrite(&(pmach->_textsize),sizeof(unsigned), 1, f)!=1){

		//si on écrit plus ou moins d'un caratère, on quitte l'exécution
		perror("Erreur lors de l'écriture de _textsize dans <machine.c:dump_create>");
		exit(1);
	}

	//écriture de datasize
	if(fwrite(&(pmach->_datasize),sizeof(unsigned), 1, f)!=1){

		//si on écrit plus ou moins d'un caratère, on quitte l'exécution
		perror("Erreur lors de l'écriture de _datasize dans <machine.c:dump_create>");
		exit(1);
	}

	//écriture de dataend
	if(fwrite(&(pmach->_dataend),sizeof(unsigned), 1, f)!=1){

		//si on écrit plus ou moins d'un caratère, on quitte l'exécution
		perror("Erreur lors de l'écriture de _dataend dans <machine.c:dump_create>");
		exit(1);
	}

	//écriture des instructions
	if((fwrite(&(pmach->_text),sizeof(Word), pmach->_textsize, f)) != pmach->_textsize){

		//si l'on écrit moins de pmach->_textsize mots de 32 bits, alors on quitte l'exécutions
		perror("Erreur lors de l'écriture des instructions dans <machine.c:dump_create>");
		exit(1);
	}

	//écriture des données
	if((fwrite(&(pmach->_data),sizeof(Word), pmach->_datasize, f)) != pmach->_datasize){

		//si l'on écrit moins de pmach->_datasize mots de 32 bits, alors on quitte l'exécutions
		perror("Erreur lors de l'écriture des données dans <machine.c:dump_create>");
		exit(1);
	}

	//on ferme proprement le fichier ouvert
	fclose(f);
}


/*! 
 * Affichage du programme et des données
 *
 * On affiche les instruction et les données en format hexadécimal, sous une
 * forme prête à être coupée-collée dans le simulateur.
 *
 * Pendant qu'on y est, on produit aussi un dump binaire dans le fichier
 * dump.prog. Le format de ce fichier est compatible avec l'option -b de
 * test_simul.
 *
 * \param pmach la machine en cours d'exécution
 */
void dump_memory(Machine *pmach){

	//Délégation de l'affichage	
	dump_print(pmach);
	
	//Délégation de la production du dump binaire
	dump_create(pmach);

}