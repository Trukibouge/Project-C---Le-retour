//Projet


//Bibliothèques

#include <stdio.h>
#include <stdlib.>
#include <string.h>

//Structures

typedef struct _Mot
{
	char word[25];
	int occurences;
	struct _Mot *suiv;

}Mot;

//Variables globales

#define TAILLE_MAX 29

//Prototypes

int is_in(char* mot);
void table_hachage(Mot** l);
int hachage(char* m);
void creation_tete(Mot** l);

//Main

int main(void)
{

return 0;
}

//Fonctions
//------------------------------------------------------------

int hachage(char* m)
{
	int hach;
	char lettre;
	lettre = m[0];
	hach = lettre;
return hach%TAILLE_MAX;
}

//------------------------------------------------------------

void creation_tete(Mot** l)
{
	int i;
	int compteur=97;
	Mot* tete = NULL;
	for (i=0;i<26;i++)
	{
		l[compteur%TAILLE_MAX] = tete;
		compteur++;
	}
}


//------------------------------------------------------------

void table_hachage(Mot** l)
{
	FILE* fp;
	fp = fopen("dictionnaire.txt","r");
	char mot[25];
	int indice;
	
	while ((fscanf(fp,"%s",mot) !=0)
	{
		Mot* maillon = malloc(sizeof(Mot));
		strcpy(maillon->word,mot);
		indice = hachage(mot);
		maillon->suiv = l[indice];
		l[indice] = maillon;
	}
	fclose(fp);
}

//------------------------------------------------------------


int is_in(char* mot,Mot** l)
{
	int indice;
	int test = 0;
	Mot *p = malloc(sizeof(Mot));
	p = l[indice];
	indice = hachage(mot);

	while (p!=NULL)
	{
		if(strcmp(p->word,mot)==0)
		{
			test = 1;
		}
		p = p->suiv;
	}

return test;
}

//------------------------------------------------------------
