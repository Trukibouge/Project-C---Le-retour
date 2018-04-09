//Projet


//Bibliothèques

#include <stdio.h>
#include <stdlib.h>
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

int is_in(char* mot,Mot** l);
void table_hachage_dicofr(Mot** l);
void table_hachage_predictions(Mot** l);
int hachage(char* m);
void creation_tete(Mot** l);

//Main

int main(void)
{

return 0;
}

//Fonctions
//------------------------------------------------------------

void table_hachage_predictions(Mot** l)
{
	FILE* fp;
	fp = fopen("dico_predictions.txt","r");
	char mot[25];
	int indice;
	Mot* p;
	
	while (fscanf(fp,"%s",mot) !=0)
	{
		if (is_in(mot,l) == 1)
		{
			indice = hachage(mot);
			p = l[indice];

			while(strcmp(p->word,mot)!=0)
			{
				p = p->suiv;
			}
			p->occurences++;
		}

		else
		{
			Mot* maillon = malloc(sizeof(Mot));	
			strcpy(maillon->word,mot);
			maillon->occurences = 1;		
			indice = hachage(mot);			
			maillon->suiv = l[indice];
			l[indice] = maillon;
		}
	}
	fclose(fp);
}

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

void table_hachage_dicofr(Mot** l)
{
	FILE* fp;
	fp = fopen("dictionnaire.txt","r");
	char mot[25];
	int indice;
	
	while (fscanf(fp,"%s",mot) !=0)
	{
		Mot* maillon = malloc(sizeof(Mot));
		strcpy(maillon->word,mot);
		maillon->occurences = 1;
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
	indice = hachage(mot);
	p = l[indice];

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
