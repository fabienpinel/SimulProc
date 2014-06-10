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
	

}


//! Affichage d'un avertissement
/*!
 * \param warn code de l'avertissement
 * \param addr adresse de l'erreur
 */
void warning(Warning warn, unsigned addr){

}