//Projet


//Bibliothèques

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

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
void ajout_mot(Mot* *l1,Mot* *l2);

//Main

int main(void)
{

return EXIT_SUCCESS;
}

//Fonctions
//------------------------------------------------------------

void ajout_mot(Mot* *l1,Mot* *l2)
{
	char ajout[30];
	int pres;
	int indice;
	int choix;

	printf("\nVous avez choisi l'ajout de mot ou acronyme au dictionnaire de prédiction. Que souhaitez-vous faire ?\n");

	while (choix!=3)
	{

		printf("\n- Pour ajouter un mot (sensible à l'orthographe), tapez 1.\n- Pour un acronyme (insensible à l'orthographe), tapez 2.\n- Si vous souhaitez revenir au menu principal, tapez 3.\n\n Votre choix : ");
		scanf("%d",&choix);
	
		switch(choix)
		{
			case 1:
				printf("\nRentrez le mot que vous souhaitez ajouter au dictionnaire de prédiction : ");
				scanf("%s",ajout);

				pres = is_in(ajout,l1);
				indice = hachage(ajout);
	
				if (pres == 0)
				{
					printf("\nCe mot n'est pas dans le dictionnaire français. Veuillez réessayez.\n");
				}
	
				else
				{
					if (is_in(ajout,l2) == 1)
					{
						printf("\nLe mot est déjà dans le dictionnaire de prédiction.\n");
					}

					else
					{
						printf("\nLe mot a été ajouté au dictionnaire de prédiction avec succès.\n");
						Mot *maillon = malloc(sizeof(Mot));
						strcpy(maillon->word,ajout);
						maillon->occurences = 1;
						maillon->suiv = l2[indice];
						l2[indice] = maillon;
					}
				}
				break;

			case 2:
				printf("\nRentrez l'acronyme que vous souhaitez ajouter au dictionnaire de prédiction : ");
				scanf("%s",ajout);
				indice = hachage(ajout);
	
				if (is_in(ajout,l2) == 1)
				{
					printf("\nL'acronyme est déjà dans le dictionnaire de prédiction.\n");
				}

				else
				{
					printf("\nL'acronyme a été ajouté au dictionnaire de prédiction avec succès.\n");
					Mot *maillon = malloc(sizeof(Mot));
					strcpy(maillon->word,ajout);
					maillon->occurences = 1;
					maillon->suiv = l2[indice];
					l2[indice] = maillon;
				}
				break;

			case 3:
				break;
		
		}
	}
}

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
		indice = hachage(mot);

		if (is_in(mot,l) == 1)
		{
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
