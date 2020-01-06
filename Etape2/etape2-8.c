#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "etape2-8.h"

int longueur_nom(char* nom){
	int j = 0;
	while (nom[j]!=0){
		j++;
	}
	return j;
}
void ajouter_nom(char* nom, char* table_nom,int fin_tab){
	int i=fin_tab,j;
	for(j=0;j<longueur_nom(nom);j++){
		table_nom[i]=nom[j];
		i++;
	}
}

void file_copy(FILE* file1, FILE* file2, size_t size){
	char* ptr=malloc(size);
	fread(ptr,size,1,file1);
	fwrite(ptr,size,1,file2);
}

int est_nom_dans_tab(char* nom, char* table_nom){
	int i=0;
	int j=0;
	while(table_nom[i]!=0 || table_nom[i+1]!=0){
		while(table_nom[i+j]==nom[j] && j<longueur_nom(nom)){
			j++;
		}
		if (j==longueur_nom(nom)) return 1;
		else j=0;
		i++;
	}
	return 0;
}

void fusion_reimplementation(FILE* elfFile1, FILE* elfFile2,FILE* outputFile){
	FILE * tempFile;
	uint32_t section_name=0;
	Elf32_Ehdr header1,header2,headerOutput;
	Elf32_Shdr section1,section2,sectionOut;
	int16_t nb_section=0;
	int offset_actuel=0;
	char tabNomSection[10000]="";
	char* nom_section1=NULL;
	char* nom_section2=NULL;
	tempFile = fopen("tempFileSectionTable","w");
	litEtInverse_Header(elfFile1,&header1);
	litEtInverse_Header(elfFile2,&header2);
	// check so its really an elf file
	if (memcmp(header1.e_ident, ELFMAG, SELFMAG) == 0 && memcmp(header2.e_ident, ELFMAG, SELFMAG) == 0 ) {
		fwrite(&header1,sizeof(header1),1,outputFile);
		for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
			fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile1,header1,&section1);
			nom_section1=get_section_name(elfFile1,header1,section1);
			for (int j=0;j<header2.e_shnum;j++){ //parcours des sections du fichier 2
				fseek(elfFile2, header2.e_shoff + j * header2.e_shentsize, SEEK_SET);
				litEtInverse_Section(elfFile2,header2,&section2);
				nom_section2=get_section_name(elfFile2,header2,section2);
				if(!strcmp(nom_section1,nom_section2)){ //si les sections ont le meme nom
					fprintf(tempFile,"%s",nom_section1);
					ajouter_nom(nom_section1,tabNomSection,section_name);
					sectionOut.sh_name=section_name;
					section_name+=longueur_nom(nom_section1)+1;
					sectionOut.sh_type=section1.sh_type;
					sectionOut.sh_flags=section1.sh_flags;
					sectionOut.sh_addr=section1.sh_addr;
					sectionOut.sh_offset=offset_actuel;
					sectionOut.sh_size=section1.sh_size+section2.sh_size;
					offset_actuel+=section1.sh_size+section2.sh_size;
					fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
					fseek(elfFile1,section1.sh_offset,SEEK_SET);
					file_copy(elfFile1,tempFile,sizeof(section1.sh_size));
					fseek(elfFile2,section2.sh_offset,SEEK_SET);
					file_copy(elfFile2,tempFile,sizeof(section2.sh_size));
					nb_section+=1;
				}
			}
		}
		for (int i=0; i<header1.e_shnum;i++){ //parcours des sections du fichier 1
			fseek(elfFile1, header1.e_shoff + i * header1.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile1,header1,&section1);
			if(!est_nom_dans_tab(get_section_name(elfFile1,header1,section1),tabNomSection)){
				fprintf(tempFile,"%s",nom_section1);
				ajouter_nom(nom_section1,tabNomSection,section_name);
				sectionOut.sh_name=section_name;
				section_name+=longueur_nom(nom_section1)+1;
				sectionOut.sh_type=section1.sh_type;
				sectionOut.sh_flags=section1.sh_flags;
				sectionOut.sh_addr=section1.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				offset_actuel+=section1.sh_size;
				sectionOut.sh_size=section1.sh_size;
				fwrite(&sectionOut,sizeof(sectionOut),1,outputFile);
				fseek(elfFile1,section1.sh_offset,SEEK_SET);
				file_copy(elfFile1,tempFile,sizeof(section1.sh_size));
			}
		}
		for (int i=0; i<header2.e_shnum;i++){ //parcours des sections du fichier 1
			fseek(elfFile2, header2.e_shoff + i * header2.e_shentsize, SEEK_SET);
			litEtInverse_Section(elfFile2,header2,&section2);
			if(!est_nom_dans_tab(get_section_name(elfFile2,header2,section2),tabNomSection)){
				fprintf(tempFile,"%s",nom_section2);
				ajouter_nom(nom_section2,tabNomSection,section_name);
				sectionOut.sh_name=section_name;
				section_name+=longueur_nom(nom_section2)+1;
				sectionOut.sh_type=section2.sh_type;
				sectionOut.sh_flags=section2.sh_flags;
				sectionOut.sh_addr=section2.sh_addr;
				sectionOut.sh_offset=offset_actuel;
				offset_actuel+=section2.sh_size;
				sectionOut.sh_size=section2.sh_size;
				fwrite(&sectionOut,sizeof(sectionOut),2,outputFile);
				fseek(elfFile2,section2.sh_offset,SEEK_SET);
				file_copy(elfFile2,tempFile,sizeof(section2.sh_size));
			}
		}
	}
	fclose(tempFile);
}
