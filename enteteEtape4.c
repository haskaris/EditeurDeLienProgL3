#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "enteteEtape4.h"

int isbigendian(Elf32_Ehdr h){
	switch (h.e_ident[EI_DATA]) {
		case ELFDATA2LSB:
      			return 0;
    		case ELFDATA2MSB:
      			return 1;
    		default://ELFDATANONE
      			printf("isbigEndian problème pas d'encodage\n");
			exit(1);
  	}
}

uint16_t byteshift16(uint16_t n, int bigEndian){
    if (bigEndian){
        return ((n>>8)&0xff) | ((n<<8)&0xff00);
    }
    else{
        return n;
    }
}

uint32_t byteshift32(uint32_t n, int bigEndian) {
    if (bigEndian){
          return ((n>>24)&0xff) | ((n<<8)&0xff0000) | ((n>>8)&0xff00) | ((n<<24)&0xff000000);
    }
    else{
        return n;
    }
}

uint64_t byteshift64(uint64_t n, int bigEndian) {
    if (bigEndian){
          return ((n>>24)&0xff0000) | ((n<<8)&0xff00000000) | ((n>>8)&0xff000000) | ((n<<24)&0xff0000000000) | ((n>>40)&0xff00) | ((n<<40)&0xff000000000000) | ((n<<56)&0xff00000000000000) | ((n>>56)&0xff);
    }
    else{
        return n;
    }
}

char* get_section_names(FILE *elfFile, Elf32_Shdr sh_table, int bigEndian)
{
    //Lire les noms des sections
    char *nom = malloc(byteshift32(sh_table.sh_size, bigEndian));
    fseek(elfFile, byteshift32(sh_table.sh_offset, bigEndian), SEEK_SET);
    fread(nom, 1, byteshift32(sh_table.sh_size, bigEndian), elfFile);
    return nom;
}

void print_symbol_table32(FILE* elfFile, Elf32_Ehdr eh, Elf32_Shdr sh_table, uint32_t indice, int bigEndian){	
	Elf32_Sym sym_tbl;
	uint32_t i, nombre_symbol;
	//accéde à la table des symboles
	fseek(elfFile, eh.e_shoff + indice * sizeof(sh_table), SEEK_SET);
	fread(&sh_table, 1, sizeof(sh_table), elfFile);
  	//printf("Name : %s\n",get_section_names(elfFile, sh_table, bigEndian) + sh_table.sh_name);
	
	// calcul du nombre de symbole   	
	nombre_symbol = byteshift32(sh_table.sh_size, bigEndian) / byteshift32(sh_table.sh_entsize, bigEndian);
  	fseek(elfFile, byteshift32(sh_table.sh_offset, bigEndian), SEEK_SET);

	printf("La table de symboles << .symtab >> contient %d entrées :\n", nombre_symbol);
	printf("  Num:   Valeur     Tail  Type  Lien  Vis     Ndx Nom\n");
        
	//on affiche les informations du symbole
	for(i=0; i< nombre_symbol; i++) {
		fread(&sym_tbl, 1, sizeof(sym_tbl), elfFile);
		printf("   %d:",i);
		printf("  %08x ", byteshift32(sym_tbl.st_value, bigEndian));
		printf(" %u ", byteshift32(sym_tbl.st_size, bigEndian));
		switch (ELF32_ST_TYPE(sym_tbl.st_info)) {
        		case 0:
            			printf("  NOTYPE  ");
            			break;
       			case 1:
            			printf("  OBJT  ");
            			break;
        		case 2:
            			printf("  FUNC  ");
            			break;
        		case 3:
            			printf("  SECTION  ");
            			break;
        		case 4:
            			printf("  FILE  ");
            			break;

       			default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_TYPE ");
            			break;
    		}
		switch(ELF32_ST_BIND(sym_tbl.st_info))
    		{
       	 		case 0: printf(" LOCAL ");
            			break;
        		case 1: printf(" GLOBAL ");
            			break;
        		case 2: printf(" WEAK ");
            			break;
        		case 3: printf("  NUM ");
            			break;
        		default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table32 switch ELF32_ST_BIND ");
        			break;
    		}
		//printf(" %x  ", sym_tbl.st_other);
		switch(ELF32_ST_VISIBILITY(sym_tbl.st_other)){
			case 0: printf(" DEFAULT ");
				break;
			case 1: printf(" INTERNAL ");
				break;
			case 2: printf(" HIDDEN ");
				break;
			case 3: printf(" PROTECTED ");
				break;
			default: printf(" Cas non géré fichier enteteEtape4.c fonction print_symbol_table64 switch ELF64_ST_VISIBILITY ");
				break;
		}
    		printf(" %x  ", byteshift16(sym_tbl.st_shndx, bigEndian));
    		printf(" %02x\n", byteshift32(sym_tbl.st_name, bigEndian));	}
}

void print_table_symbol(FILE* elfFile, Elf32_Ehdr eh, int bigEndian){
	uint32_t i;
	Elf32_Shdr section;
	//parcours la table des entêtes et 
	//cherche la section contenant la table des symboles (SHT_SYMTAB)		
	for(i=0; i<byteshift16(eh.e_shnum, bigEndian); i++) {
		fseek(elfFile, byteshift32(eh.e_shoff, bigEndian) + i * sizeof(section), SEEK_SET);
         	fread(&section, 1, sizeof(section), elfFile);
		if (byteshift32(section.sh_type, bigEndian)==SHT_SYMTAB) {
			print_symbol_table32(elfFile, eh, section, i, bigEndian);
			break;
		}
	}
}



int main(int argc, char *argv[]) {
  FILE * elfFile;

  Elf32_Ehdr header;

  if (argc != 2) {
    printf("Utilisation : %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }
  else {
    elfFile = fopen(argv[1], "r");
    if (elfFile == NULL) {
      printf("Erreur d'ouverture du fichier.\n");
    }
    else {
      // read the header
      	fread(&header, 1, sizeof(header), elfFile);
	if (isbigendian(header)){
		print_table_symbol(elfFile, header, 1);
		//printf("pas encore fait big endian"); 
	} else { 
		//printf("pas encore fait litle endian");
		print_table_symbol(elfFile, header, 0); 
	}  
	fclose(elfFile);
    }
  }
}