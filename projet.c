//Projet n°2 d'algorithmique et programmation : Développement d'une application de saisie intuitive.
//------------------------------------------------------------

//Bibliothèques
//------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

//Structures

typedef struct _Mot
{
	char word[25];
	int occurences;
	struct _Mot *suiv;
}Mot;

//Variables globales

#define TAILLE_MAX 29
Mot* dico_fr[TAILLE_MAX];
Mot* dico_predic[TAILLE_MAX];

//Prototypes
//------------------------------------------------------------

//Pour la bibliothèque GTK
void Destroy(GtkWidget *pWidget,gpointer pData);
void on_activate_entry(GtkWidget *Entry,gpointer *data);
void on_clicked_button_entry(GtkWidget *Button,gpointer data);
void on_clicked_button_buttons(GtkWidget *Button,gpointer *data);
void on_clicked_activation(void);
void on_clicked_acquisition(GtkWidget *Button, gpointer data);
void on_change_prediction(void);

//Pour la bibliothèque standard
int is_in(char* mot,Mot** l);
void table_hachage_dicofr(Mot** l);
void table_hachage_predictions(Mot** l);
int hachage(char* m);
void creation_tete(Mot** l);
void ajout_mot(Mot* *l1,Mot* *l2,const gchar* T);
void sauvegarde(Mot** l);

//Main
//------------------------------------------------------------

int main(int argc,char** argv)
{

/*-------------------------Initialisation des tables-------------------------*/

	creation_tete(dico_fr);
	creation_tete(dico_predic);
	table_hachage_predictions(dico_predic);
	table_hachage_dicofr(dico_fr);
	
/*-------------------------Initialisation de l'interface graphique-------------------------*/

	//Les Widgets GTK nécessaires
	GtkWidget *Window;
	GtkWidget *Label[2];
	GtkWidget *Button[6];
	GtkWidget *Table;
	GtkWidget *Saisie;
	GtkWidget *Image;
	gchar* intitule;
	gchar* basdepage;

	//Initialisation de GTK
	gtk_init(&argc,&argv);

	//Initialisation de la fenêtre
	Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(Window),GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(Window),"PredWords - La prédiction de mots");
	gtk_window_set_default_size(GTK_WINDOW(Window),800,600);
	gtk_window_set_resizable(GTK_WINDOW(Window),TRUE);	
	g_signal_connect(G_OBJECT(Window),"destroy",G_CALLBACK(Destroy),NULL);

	//Création des différents labels
	intitule = g_locale_to_utf8("<b><span font_desc=\"Times New Roman italic 12\"> Bienvenue dans PredWords !\n L'application créée par deux élèves ingénieurs de folie. </span></b>",-1,NULL,NULL,NULL);
	Label[0] = gtk_label_new(intitule);
	gtk_label_set_use_markup(GTK_LABEL(Label[0]),TRUE);
	g_free(intitule);

	basdepage = g_locale_to_utf8("<i>Constant Bounoure and Tuan-Anh Bach. All rights reserved.</i>",-1,NULL,NULL,NULL);
	Label[1] = gtk_label_new(basdepage);
	gtk_label_set_use_markup(GTK_LABEL(Label[1]),TRUE);
	g_free(basdepage);

	//Justifications
	gtk_label_set_justify(GTK_LABEL(Label[0]),GTK_JUSTIFY_CENTER);
	gtk_label_set_justify(GTK_LABEL(Label[1]),GTK_JUSTIFY_CENTER);

	//Création des différents boutons
	Button[0] = gtk_button_new_with_label("Proposition 1");
	Button[1] = gtk_button_new_with_label("Proposition 2");
	Button[2] = gtk_button_new_with_label("Proposition 3");
	Button[3] = gtk_button_new_with_label("Activer / Désactiver la prédiction");
	Button[4] = gtk_button_new_from_stock(GTK_STOCK_QUIT);
	Button[5] = gtk_button_new_with_label("Acquisition de mot");

	//Création de la table
	Table = gtk_table_new(5,3,TRUE);
	gtk_container_add(GTK_CONTAINER(Window),Table);

	//Création de la zone de saisie de texte
	Saisie = gtk_entry_new_with_max_length(100);	

	//Remplissage de la table
	gtk_table_attach(GTK_TABLE(Table),Label[0],0,3,0,1,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(Table),Label[1],0,3,4,5,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(Table),Button[0],0,1,1,2,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(Table),Button[1],1,2,1,2,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(Table),Button[2],2,3,1,2,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(Table),Saisie,0,3,2,3,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,10,20);
	gtk_table_attach(GTK_TABLE(Table),Button[3],0,1,3,4,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,10,0);
	gtk_table_attach(GTK_TABLE(Table),Button[4],2,3,3,4,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,10,0);
	gtk_table_attach(GTK_TABLE(Table),Button[5],1,2,3,4,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,10,0);

	//Connexion des différents boutons et de la zone de texte
	g_signal_connect(G_OBJECT(Button[4]),"clicked",G_CALLBACK(Destroy),NULL);
	g_signal_connect(G_OBJECT(Button[0]),"clicked",G_CALLBACK(on_clicked_button_entry),Saisie);
	g_signal_connect(G_OBJECT(Button[1]),"clicked",G_CALLBACK(on_clicked_button_entry),Saisie);
	g_signal_connect(G_OBJECT(Button[2]),"clicked",G_CALLBACK(on_clicked_button_entry),Saisie);
	g_signal_connect(G_OBJECT(Button[0]),"clicked",G_CALLBACK(on_clicked_button_buttons),Button);
	g_signal_connect(G_OBJECT(Button[1]),"clicked",G_CALLBACK(on_clicked_button_buttons),Button);
	g_signal_connect(G_OBJECT(Button[2]),"clicked",G_CALLBACK(on_clicked_button_buttons),Button);
	g_signal_connect(G_OBJECT(Button[3]),"clicked",G_CALLBACK(on_clicked_activation),NULL);
	g_signal_connect(G_OBJECT(Button[5]),"clicked",G_CALLBACK(on_clicked_acquisition),Saisie);
	g_signal_connect(G_OBJECT(Saisie),"activate",G_CALLBACK(on_activate_entry),Button);
	g_signal_connect(G_OBJECT(Saisie),"changed",G_CALLBACK(on_change_prediction),NULL);
	
	//Affichage de la fenêtre
	gtk_widget_show_all(Window);
	gtk_main();

return EXIT_SUCCESS;
}

//Fonctions
//------------------------------------------------------------

//Les callbacks pour GTK

void Destroy(GtkWidget *pWidget,gpointer data)
{
	sauvegarde(dico_predic);
	gtk_main_quit();
}

//------------------------------------------------------------

void on_activate_entry(GtkWidget *Entry,gpointer *data)
{
	const gchar *Text;
	Text = gtk_entry_get_text(GTK_ENTRY(Entry));
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[0]),Text);
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[1]),Text);
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[2]),Text);
}

//------------------------------------------------------------

void on_clicked_button_entry(GtkWidget *Button,gpointer data)
{
	const gchar *Text;
	Text = gtk_button_get_label(GTK_BUTTON(Button));
	gtk_entry_set_text(GTK_ENTRY(data),Text);	
}

//------------------------------------------------------------

void on_clicked_button_buttons(GtkWidget *Button,gpointer *data)
{
	const gchar *Text;
	Text = g_locale_to_utf8(gtk_button_get_label(GTK_BUTTON(Button)),-1,NULL,NULL,NULL);
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[0]),Text);
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[1]),Text);
	gtk_button_set_label(GTK_BUTTON((GtkWidget*)data[2]),Text);
}

//------------------------------------------------------------

void on_clicked_activation(void)
{

}

//------------------------------------------------------------

void on_clicked_acquisition(GtkWidget *Button, gpointer data)
{
	const gchar *Text;
	int success;
	Text = gtk_entry_get_text(GTK_ENTRY(data));
	ajout_mot(dico_fr,dico_predic,Text);
}

//------------------------------------------------------------

void on_change_prediction(void)
{

}

//------------------------------------------------------------

//Les fonctions à exécuter

void sauvegarde(Mot** l)
{
	FILE *fp;
	fp = fopen("sauvegarde.txt","w+");
	int compteur = 97;
	Mot *p = malloc(sizeof(Mot));

	while (compteur<=122)
	{
		p = l[compteur%TAILLE_MAX];
		while(p!=NULL)
		{
			fprintf(fp,"%s %d",p->word,p->occurences);
			p = p->suiv;
		}
		compteur++;
	}

}


void ajout_mot(Mot** l1,Mot** l2,const gchar* t)
{
	char* T = (gchar*)t;
	int pres;
	int indice;
	pres = is_in(T,l1);
	
	if (pres == 0)
	{}

	else
	{
		indice = hachage(T);
		if (is_in(T,l2) == 1)
		{
			Mot *p = malloc(sizeof(Mot));
			p = l2[indice];
			
			while (strcmp(T,p->word)!=0)
			{
				p = p->suiv;
			}
			p->occurences++;
		}

		else
		{
			Mot *maillon = malloc(sizeof(Mot));
			strcpy(maillon->word,T);
			maillon->occurences = 1;
			maillon->suiv = l2[indice];
			l2[indice] = maillon;
		}
	}
}

//------------------------------------------------------------

void table_hachage_predictions(Mot** l)
{
	FILE* fp;
	fp = fopen("dicopredictions.txt","r");
	char mot[25];
	int indice;
	
	while (fscanf(fp,"%s",mot) ==1)
	{
		indice = hachage(mot);

		if (is_in(mot,l) == 1)
		{
			Mot* p = malloc(sizeof(Mot));
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

int hachage(char* t)
{
	char lettre;
	lettre = t[0];
	int ascii = lettre;

return ascii%TAILLE_MAX;
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
	fp = fopen("dictionnaire.txt","r+");
	char mot[25];
	int indice;
	
	while (fscanf(fp,"%s",mot)==1)
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
