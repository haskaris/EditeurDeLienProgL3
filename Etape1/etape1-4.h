#ifndef __ETAPE1_4_H__
#define __ETAPE1_4_H__
//TODO: nettoyer les includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../librairie/utils.h"
#include "../librairie/lib-bigEndian.h"

/* Créer la table des symboles, la lit dans le fichier ELF
 * puis écrit son contenu dans la console
 * arguments :
 * 	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
 *	- table des entêtes du fichier ELF (section)
 *	- l'indice de la bonne section dans la table des entêtes (indice)
*/
void print_symbol_table32(FILE* elfFile, Elf32_Ehdr header, Elf32_Shdr section, uint32_t indice);

/* Sert à accéder à la bonne section dans la table des entêtes
 * puis appel la fonction print_symbol_table64 pour écrire son contenu
 * arguments :
 *	- le fichier ELF (elfFile) ouvert
 *	- entête du fichier ELF (header)
*/
void affichage_Table_Des_Symbole(FILE *elfFile, Elf32_Ehdr header);
#endif  /* __ETAPE1_4_H__ */
