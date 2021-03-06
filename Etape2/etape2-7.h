#ifndef __ETAPE2_7_H__
#define __ETAPE2_7_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/arbreBinaire.c"


int fusion_2_7(FILE * elfFile1, FILE * elfFile2, FILE * outputFile);

int fusion_section_2_7(FILE* elfFile1, FILE* elfFile2, FILE* outputFile, Elf32_Ehdr* headerOutput);

/* Initialise l'arbre des symboles globaux
*/
void ArbreVariableGlobalInitialisation(struct Noeud *ArbreVariableGlobal);

void ArbreVariableLocalInitialisation(struct NoeudLocal *ArbreVariableLocal);
/* Vérifie les conditions sur les symboles globaux, si
 * le symbole n'y est pas il est ajouté.
 * arguments :
 * 	- le symbole global à ajouter (symbole)
*/
void verificationSymboleGlobal(Elf32_Sym symbole, struct Noeud *ArbreVariableGlobal);

/* Ecrit les symboles locaux dans le fichier ELF
 * arguments :
 * 	- le fichier elf destination (elfFileDest)
 *	- le symbole à écrire dans le fichier (symbole)
*/
void ecritureSymbolLocalFichierElf(FILE* elfFileDest, struct Liste *listeLocal);

/* PROBLEME
 * Variable Global MAIN présente dans toutes
 * les tables de symbôle, comment faire ???
 * Parcours la table des symboles d'un fichier
 * et donne les variables à écrire dans le fichier destination
 * arguments :
 * 	- le fichier elf de la table des symbole à copier (elfFile)
 *	- l'entête du fichier ELF à copier (header)
 *	- la table des sections du fichier ELF à copier (section)
 *	- l'indice de la section contenant la table des symboles (indice)
 *	- le fichier ELF destination (elfFileDest)
*/
void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice, struct Noeud *ArbreVariableGlobal, struct Liste *listeLocal);



/* Fonction qui affiche toutes les informations du symbole donné
 * arguments :
 *	- symbole qu'on souhaite afficher (table_symbole)
*/
void afficheSymbole(Elf32_Sym table_symbole);

/* Fonction qui écrit tout les symboles globaux dans le fichier final
 * arguments :
 *	- fichier elf final (elfFileDest)
 *	- l'arbre contenant les variables globals (noeud)
*/
void ecritureSymbolGlobalFichierElf(FILE* elfFileDest, Noeud *noeud);

/* PROBLEME
 * Variable Global MAIN présente dans toutes
 * les tables de symbôle, comment faire ???
*/




#endif  /* __ETAPE2_7_H__ */
