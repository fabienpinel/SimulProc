/*!
 * \file exec.c
 * \brief Exécution d'une instruction.
 */
 
 #include "exec.h"
 #include "error.h"
 #include <stdio.h>

//! Vérifie si l'instruction peut avoir un opérande immédiat
/*!
 * Si l'opérande est immédiat et que l'instruction ne le supporte pas,
 * alors un message d'erreur est affiché (arrêt de l'exécution du programme)
 * \param instr l'instruction à exécuter 
 * \param addr l'adresse de l'instruction à exécuter
 * 
 */
void check_immediate(Instruction instr, unsigned addr) {
	if(instr.instr_generic._immediate) {
		error(ERR_IMMEDIATE, addr);
	}
}

//! Teste une condition par rapport au code condition CC
/*! 
 * \param cond la condition à tester
 * \param cc le code condition par rapport auquel la condition doit être testée
 * \param addr l'adresse de l'instruction comportant cette condition
 * \return vrai si la condition est satisfaite, faux sinon
 * 
 */
bool check_condition(Machine *pmach, Instruction instr) {
	switch(instr.instr_generic._regcond){
		case NC : // Pas de condition, donc toujours vrai
			return true;
		case EQ : // Le résultat précédent doit être nul
			return (pmach->_cc == CC_Z);
		case NE : // Le résultat précédent doit être non nul
			return (pmach->_cc != CC_Z);
		case GT : // Le résultat précédent doit être strictement positif
			return (pmach->_cc == CC_P);
		case GE : // Le résultat précédent doit être positif ou nul
			return (pmach->_cc == CC_P || pmach->_cc == CC_Z);
		case LT : // Le résultat précédent doit être strictement négatif
			return (pmach->_cc == CC_N);
		case LE : // Le résultat précédent doit être négatif ou nul
			return (pmach->_cc == CC_N || pmach->_cc == CC_Z);
		default: // Valeur impossible de la condition
			error(ERR_CONDITION, pmach->_pc-1);
			break;
	}
}

//! Mise à jour du code condition CC
/*!
 * Met à jour le code condition en fonction du signe du résultat
 * obtenu par l'instruction de calcul ou de transfert
 * \param pmach la machine/programme en cours d'exécution
 * \param reg le numéro du registre dont le signe du contenu nous intéresse
 */
void update_CC(Machine *pmach, Instruction instr) {
	if(pmach->_registers[instr.instr_generic._regcond] < 0) {
		pmach->_cc = CC_N;
	} else if (pmach->_registers[instr.instr_generic._regcond] > 0) {
		pmach->_cc = CC_P;
	} else { //pmach->_registers[instr.instr_generic._regcond] == 0
		pmach->_cc = CC_Z;
	}
}

//! Vérifie que l'adresse appartient bien au segment de Données
/*!
 * Si on est en-dehors du segment, on affiche une erreur (arrêt programme)
 * \param pmach la machine/programme en cours d'exécution
 * \param addr_mem l'adresse mémoire à laquelle on veut accéder
 */
void check_seg_data(Machine *pmach, unsigned addr_mem) {
	if(addr_mem < 0 || addr_mem > pmach->_datasize-1) {
		error(ERR_SEGDATA, pmach->_pc-1);
	} 
}

//! Vérifie que sp pointe bien le segment de Données
/*!
 * Si on est en-dehors du segment, on affiche une erreur (arrêt programme)
 * \param pmach la machine/programme en cours d'exécution
 */
void check_seg_stack(Machine *pmach) {
	if (pmach->_sp < pmach->_dataend || pmach->_sp >= pmach->_datasize) {
		error(ERR_SEGSTACK, pmach->_pc-1);
	}
}

//! Vérifie que le registre est un des 16 registres
/*!
 * Si on est en-dehors du tableau de registres, on affiche une erreur (arrêt programme)
 * \param pmach la machine/programme en cours d'exécution
 * \param reg le numéro du registre auquel on veut accéder
 */
void check_seg_registers(Machine *pmach, unsigned reg) {
	if(reg < 0 || reg > NREGISTERS - 1) {
		error(ERR_ILLEGAL, pmach->_pc-1);
	}
}

//! Génère une adresse valide selon si elle est indexée ou absolu
/*!
 * Si l'adresse n'est pas valide, on affiche une erreur (arrêt de l'exécution)
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 */
unsigned generate_address(Machine *pmach, Instruction instr){
	unsigned address;
	// L'adresse est indexée
	if(instr.instr_generic._indexed){
		address = pmach->_registers[instr.instr_indexed._rindex] + instr.instr_indexed._offset;
	// L'adresse est en absolue
	} else {
		address = instr.instr_absolute._address;
	}
	// Vérifie que l'adresse est valide
	check_seg_data(pmach,address);
	return address;
}

//! Chargement d'un registre 
/*!
 * si I = 0 : R ← Data[Addr]
 * si I = 1 : R ← Val
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction load à exécuter
 */
void load(Machine *pmach, Instruction instr){
	//Vérifie que le registre est cohérent
	check_seg_registers(pmach, instr.instr_generic._regcond);
	// Cas I = 1 : R ← Val
	if(instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_immediate._regcond] = instr.instr_immediate._value;
	// Cas I = 0 : R ← Data[Addr]
	} else {
		pmach->_registers[instr.instr_generic._regcond] = pmach->_data[generate_address(pmach,instr)];
	}
	//Met à jour le code condition
	update_CC(pmach, instr);
}

//! Rangement du contenu d'un registre 
/*!
 * L'instruction store n'accepte pas l'adresse immédiat et ne modifie
 * pas le code condition.
 * Data[Addr] ← R 
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction load à exécuter
 */
void store(Machine *pmach, Instruction instr) {
	// Vérifie que le numéro de registre est cohérent
	check_seg_registers(pmach, instr.instr_generic._regcond);
	// Vérifie qu'on est pas en adressage immédiat
	check_immediate(instr, pmach->_pc-1);
	// Data[Addr] ← R 
	pmach->_data[generate_address(pmach, instr)] = pmach->_registers[instr.instr_generic._regcond];
}

//! Addition à un registre 
/*!
 * si I = 0 : R ← (R) + Data[Addr]
 * si I = 1 : R ← (R) + Val
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction load à exécuter
 */
void add(Machine *pmach, Instruction instr) {
	// Vérifie que le numéro du registre est cohérent
	check_seg_registers(pmach, instr.instr_generic._regcond);
	// Cas I = 1 : R ← (R) + Val
	if(instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_immediate._regcond] += instr.instr_immediate._value;
	// Cas I = 0 : R ← (R) + Data[Addr]
	} else {
		pmach->_registers[instr.instr_generic._regcond] += pmach->_data[generate_address(pmach, instr)];
	}
	// Met à jour le code condition CC
	update_CC(pmach, instr);
}

//! Soustraction à un registre 
/*!
 * si I = 0 : R ← (R) - Data[Addr]
 * si I = 1 : R ← (R) - Val
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction load à exécuter
 */
void sub(Machine *pmach, Instruction instr) {
	// Vérifie que le numéro du registre est cohérent
	check_seg_registers(pmach, instr.instr_generic._regcond);
	// Cas I = 1 : R ← (R) - Val
	if(instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_immediate._regcond] -= instr.instr_immediate._value;
	// Cas I = 0 : R ← (R) - Data[Addr]
	} else {
		pmach->_registers[instr.instr_generic._regcond] -= pmach->_data[generate_address(pmach, instr)];
	}
	// Met à jour le code condition CC
	update_CC(pmach, instr);
}

//! Branchement conditionnel ou non à une adresse
/*!
 * Si la condition est vraie, PC ← Addr, sinon on ne fait rien.
 * L'instruction Branch n'accepte pas l'adressage immédiat et ne change 
 * pas la valeur du code condition.
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction branch à exécuter
 */
void branch(Machine *pmach, Instruction instr) {
	// Vérifie qu'on est pas en adressage immédiat 
	check_immediate(instr, pmach->_pc-1);
	// Vérifie que la condition est satisfaite
	if(check_condition(pmach, instr)){
		pmach->_pc = generate_address(pmach, instr);
	}
}

//! Appel d'un sous-programme
/*!
 * Si la condition est vraie on exécute call, sinon on ne fait rien.
 * L'instruction Call n'accepte pas l'adressage immédiat et ne modifie
 * pas le code condition.
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction call à exécuter
 */
void call(Machine *pmach, Instruction instr) {
	// Vérifie qu'on est pas en adressage immédiat 
	check_immediate(instr, pmach->_pc-1);
	// Vérifie que la condition est satisfaite
	if(check_condition(pmach, instr)) {
		// Vérifie qu'il y a assez de place pour empiler dans la pile
		check_seg_stack(pmach);
		pmach->_data[pmach->_sp--] = pmach->_pc; // Data[(SP)] ← (PC) et SP ← (SP) - 1
		pmach->_pc = generate_address(pmach, instr); // PC ← Addr
	}
}

//! Retour d'un sous-programme 
/*!
 * L'instruction Ret ne modifie pas le code condition CC
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction ret à exécuter
 */
void ret(Machine *pmach, Instruction instr){
	pmach->_sp += 1; // SP ← (SP) + 1
	//Vérifie qu'on est pas sorti de la pile 
	check_seg_stack(pmach);
	pmach->_pc = pmach->_data[pmach->_sp]; // PC ← Data[(SP)]
}

//! Empilement sur la pile d'exécution
/*!
 * On empile la valeur ou l'adresse (indexée ou non) dans la pile
 * si I = 0 : Data[(SP)] ← Data[Addr]
 * si I = 1 : Data[(SP)] ← Val
 * SP ← (SP) - 1
 * L'instruction Push ne modifie pas le code condition
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction push à exécuter
 */
void push(Machine *pmach, Instruction instr) {
	// Vérifie que l'on est bien dans la pile
	check_seg_stack(pmach);
	// Cas I = 1 : Data[(SP)] = Val  et SP ← (SP) - 1
	if(instr.instr_generic._immediate) {
		pmach->_data[pmach->_sp--] = instr.instr_immediate._value;
	// Cas I = 0 : Data[(SP)] = Data[Addr]  et SP ← (SP) - 1
	} else {
		pmach->_data[pmach->_sp--] = pmach->_data[generate_address(pmach, instr)];
	}
}

//! Dépilement de la pile d'exécution
/*!
 * On dépile le sommet de pile qu'on met à l'adresse Addr dans la mémoire
 * SP ← (SP) + 1
 * Data[Addr] ← Data[(SP)]
 * L'instruction Pop n'accepte pas l'adressage immédiat
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction pop à exécuter
 */
void pop(Machine *pmach, Instruction instr) {
	// Vérifie qu'on est pas en adressage immédiat
	check_immediate(instr, pmach->_pc-1);
	pmach->_sp += 1; // SP ← (SP) + 1
	// Vérifie qu'on est pas sortie de la pile
	check_seg_stack(pmach);
	pmach->_data[generate_address(pmach, instr)] = pmach->_data[pmach->_sp]; // Data[Addr] ← Data[(SP)]
}

//! Décodage et exécution d'une instruction
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \return faux après l'exécution de \c HALT ; vrai sinon
 */
bool decode_execute(Machine *pmach, Instruction instr){
	bool keepgoing = 1;
	// Le code opération nous dit quelle instruction est à exécuter
	switch(instr.instr_generic._cop){
		// ILLOP est une opération illégale qui provoque l'arrêt du programme
		case ILLOP :
			error(ERR_ILLEGAL, pmach->_pc-1);
			break;
		// NOP est une opération vide, elle ne fait rien
		case NOP :
			break;
		case LOAD :
			load(pmach, instr);
			break;
		case STORE :
			store(pmach, instr);
			break;
		case ADD :
			add(pmach, instr);
			break;
		case SUB :
			sub(pmach, instr);
			break;
		case BRANCH :
			branch(pmach, instr);
			break;
		case CALL :
			call(pmach, instr);
			break;
		case RET :
			ret(pmach, instr);
			break;
		case PUSH :
			push(pmach,instr);
			break;
		case POP :
			pop(pmach,instr);
			break;
		// HALT indique la fin du programme donc l'arrêt de l'exécution, on retourne faux
		case HALT :
			warning(WARN_HALT, pmach->_pc-1);
			keepgoing = 0;
			break;
		// Si le code opération ne correspond à aucune instruction, on affiche une erreur
		default: {
			error(ERR_UNKNOWN, pmach->_pc-1);
		}
	}	
	return keepgoing;
}

//! Trace de l'exécution
/*!
 * On écrit l'adresse et l'instruction sous forme lisible.
 *
 * \param msg le message de trace
 * \param pmach la machine en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr son adresse
 */
void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: %s:",msg);
	print_instruction(instr,addr);
	printf("\n");
}
