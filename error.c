/*!
 * \file error.h
 * \brief Messages et codes d'erreurs.
 * \author {L. GIN, A. EL-AMRANI, F. PINEL, C. BOINAUD}
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "instruction.h"
#include "exec.h"
#include "debug.h"

//! Affichage d'une erreur et fin du simulateur
/*!
 * \note Toutes les erreurs étant fatales on ne revient jamais de cette
 * fonction. L'attribut \a noreturn est une extension (non standard) de GNU C
 * qui indique ce fait.
 * 
 * \param err code de l'erreur
 * \param addr adresse de l'erreur
 */
void error(Error err, unsigned addr){
	char *err_to_print = "";
	switch(err){
		case ERR_NOERROR: err_to_print="NOERROR";
		case ERR_UNKNOWN: err_to_print="UNKNOWN";
		case ERR_ILLEGAL: err_to_print="ILLEGAL";
		case ERR_CONDITION: err_to_print="CONDITION";
		case ERR_IMMEDIATE: err_to_print="IMMEDIATE";
		case ERR_SEGTEXT: err_to_print="SEGTEXT";
		case ERR_SEGDATA: err_to_print="SEGDATA";
		case ERR_SEGSTACK: err_to_print="SEGSTACK";
		default: err_to_print="SEGSTACK";
	}
	fprintf(stderr, "Erreur %s à l'adresse 0x%x.\n", err_to_print, addr);
	exit(1);
}

//! Affichage d'un avertissement
/*!
 * \param warn code de l'avertissement
 * \param addr adresse de l'erreur
 */
void warning(Warning warn, unsigned addr){
	char *warn_to_print = "";
	switch(warn){
		case WARN_HALT: warn_to_print="HALT";
		default: warn_to_print="HALT";
	}
	fprintf(stderr, "Warning %s à l'adresse 0x%x.\n", warn_to_print, addr);
}