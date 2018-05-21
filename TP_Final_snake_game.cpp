#include <iostream>
#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h" // pour la gestion de l'écran
#include <windows.h> // pour la fonction Beep pour generer du son 
using namespace std;

// Partie I
// --------
void afficherTerrain(int nbLignes, int nbColonnes);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void deplacerSerpentI(int direction, int &posX, int &posY);

// Partie II
// ---------
int saisirNiveau();
void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris);
void afficherSouris(int sourisX[], int sourisY[], int nbSouris);
void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris);
bool finDeVie(int MurX[], int MurY[], int nbMurs, int serpentX[],
	int serpentY[], int tailleSerpent, int sourisX[], int sourisY[], int nbSouris);
bool finDeVieNiveauDifficile(int MurX[], int MurY[], int nbMurs, int serpentX[],
	int serpentY[], int tailleSerpent, int sourisX[], int sourisY[], int nbSouris);
/*	ou	*/
void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &indiceTete, int &indiceQueue,
	int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int x, int y, int sourisX[], int sourisY[], int &nbSouris);


// Partie III (BONUS)
// ------------------
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);
void optimisationDeplacementPlacement(int MurX[], int MurY[], int nbMurs, int ancienSourisX[], int ancienSourisY[],
	int sourisX[], int sourisY[], int nbSouris);
void placerMursAvance(int MurX[], int MurY[], int nbLignes, int nbColonnes, int nbMurs);
void placerMurs(int nbLignes, int nbColonnes, int nbMurs);
//validation de saisie 
int validationNumerique();
int validationLogique();



//  fonction principale
//  -------------------
const int NB_LIGNE_INITIAL = 5, NB_COLONNE_INITIAL = 6;
const int NB_DEPLACEMENT = 4;
const int nbLignes = 25, nbColonnes = 50;
int score = 0;
const int point = 10;
int main()
{
	//declaration des variables 
	int ancienSourisX[25]; int ancienSourisY[25];
	int touche, direction, nbSouris;
	int MurX[35], MurY[35]; //maximum du murs c'est 10 murs
	int posX, posY;
	int sourisX[25], sourisY[25];
	bool gameOver = false;
	int tailleSerpent;
	int serpentX[25] = {};
	int serpentY[25] = {};
	int nouvelleDirection;

	//initialisation des nombres pseudo-aleatoire
	srand((unsigned int)time(NULL));

	//	on cache le curseur
	curseurVisible(false);

	//saisie de niveau
	gotoXY(0, 50);
	cout << "SNAKE GAME !!" << endl;
	nbSouris = saisirNiveau();

	//déterminer la dimension de la fenetre
	setDimensionFenetre(80, 0, 80, 35);

	// affichge du terrain 25 sur 50
	afficherTerrain(nbLignes, nbColonnes);

	//calculer une position aléatoire sur le terrain
	positionAleatoire(nbLignes, nbColonnes, posX, posY);
	serpentX[0] = posX;
	serpentY[0] = posY;
	color(FOREGROUND_RED);
	cout << char(153);
	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED);

	//calculer une position aleatoire des nbsouris sur le terrain  
	//creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);

	//Affichage des souris sur le terrain 
	//afficherSouris(sourisX, sourisY, nbSouris);

	//palacement des murs 
	int nbMurs = nbSouris / 2;
	//placerMursAvance(MurX, MurY, nbLignes, nbColonnes, nbMurs);

	direction = -1; tailleSerpent = 1;
	clock_t horlageDeplacementSerpent = clock();
	clock_t horlageDeplacementSouris = clock();
	clock_t  dureDeplacementSerpent = 100;
	clock_t dureDeplacementSouris = 200;
	// lvl 1-9
	if (nbSouris < 10)
		while (nbSouris < 10)
		{
			if (nbSouris < 10) {
				//calculer une position aleatoire des nbsouris sur le terrain  
				creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);

				//Affichage des souris sur le terrain 
				afficherSouris(sourisX, sourisY, nbSouris);

			}
			while (!gameOver && nbSouris != 0)
			{

				touche = recupererTouche();    //tester si le joueur a appuyé sur une touche

				nouvelleDirection = calculerDirectionTouche(touche);   //calculer la direction correspondant à une touche ou - 1

				gameOver = finDeVie(MurX, MurY, nbMurs, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);

				if (nouvelleDirection != -1)
					direction = nouvelleDirection;
				//deplacer Serpent
				if (clock() > horlageDeplacementSerpent + dureDeplacementSerpent) {
					deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
					horlageDeplacementSerpent = clock();
				}
			}
			//saisie de niveau
			nbSouris = saisirNiveau();
		}
	//partie bonus 10
	clock_t horlageJeu = clock();
	clock_t durePartie = 30000;
	while (nbSouris == 10) {
		if (nbSouris == 10) {
			gotoXY(0, 0);
			cout << "Vous avez 30 (s) pour cette partie " << endl;
			//calculer une position aleatoire des nbsouris sur le terrain  
			creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);

			//Affichage des souris sur le terrain 
			afficherSouris(sourisX, sourisY, nbSouris);
			//palacement des murs 
			int nbMurs = nbSouris / 2;
			placerMursAvance(MurX, MurY, nbLignes, nbColonnes, nbMurs);
			while (!gameOver && nbSouris != 0 && (clock() <= (horlageJeu + durePartie)))
			{

				touche = recupererTouche();    //tester si le joueur a appuyé sur une touche

				nouvelleDirection = calculerDirectionTouche(touche);   //calculer la direction correspondant à une touche ou - 1

				gameOver = finDeVie(MurX, MurY, nbMurs, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);

				if (nouvelleDirection != -1)
					direction = nouvelleDirection;
				//deplacer Serpent
				if (clock() > horlageDeplacementSerpent + dureDeplacementSerpent) {
					deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
					horlageDeplacementSerpent = clock();
				}
				if (clock() > horlageDeplacementSouris + dureDeplacementSouris)
				{
					deplacerSouris(sourisX, sourisY, nbSouris);
					optimisationDeplacementPlacement(MurX, MurY, nbMurs, ancienSourisX, ancienSourisY, sourisX, sourisY, nbSouris);
					horlageDeplacementSouris = clock();
				}
			}
			if (clock() > (horlageJeu + durePartie)) {
				gotoXY(0, 2);
				cout << "Temps ecoule !!! " << endl;
			}
		}
		//saisie de niveau
		nbSouris = saisirNiveau();
	}
	// lvl 11-19
	if (nbSouris < 20)
		while (nbSouris < 20)
		{
			if (nbSouris < 20) {
				//calculer une position aleatoire des nbsouris sur le terrain  
				creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);

				//Affichage des souris sur le terrain 
				afficherSouris(sourisX, sourisY, nbSouris);

				//palacement des murs 
				int nbMurs = nbSouris/2;
				placerMursAvance(MurX, MurY, nbLignes, nbColonnes, nbMurs);

			}
			while (!gameOver && nbSouris != 0)
			{

				touche = recupererTouche();    //tester si le joueur a appuyé sur une touche

				nouvelleDirection = calculerDirectionTouche(touche);   //calculer la direction correspondant à une touche ou - 1

				gameOver = finDeVieNiveauDifficile(MurX, MurY, nbMurs, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);

				if (nouvelleDirection != -1)
					direction = nouvelleDirection;
				//deplacer Serpent
				if (clock() > horlageDeplacementSerpent + dureDeplacementSerpent) {
					deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
					horlageDeplacementSerpent = clock();
				}
			}
			//saisie de niveau
			nbSouris = saisirNiveau();
		}
	//partie bonus 20
	while (nbSouris == 20) {
		if (nbSouris == 20) {
			gotoXY(0, 0);
			cout << "Vous avez 30 (s) pour cette partie " << endl;
			//calculer une position aleatoire des nbsouris sur le terrain  
			creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);

			//Affichage des souris sur le terrain 
			afficherSouris(sourisX, sourisY, nbSouris);
			//palacement des murs 
			int nbMurs = nbSouris / 2;
			placerMursAvance(MurX, MurY, nbLignes, nbColonnes, nbMurs);
			while (!gameOver && nbSouris != 0 && (clock() <= (horlageJeu + durePartie)))
			{

				touche = recupererTouche();    //tester si le joueur a appuyé sur une touche

				nouvelleDirection = calculerDirectionTouche(touche);   //calculer la direction correspondant à une touche ou - 1

				gameOver = finDeVieNiveauDifficile(MurX, MurY, nbMurs, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);

				if (nouvelleDirection != -1)
					direction = nouvelleDirection;
				//deplacer Serpent
				if (clock() > horlageDeplacementSerpent + dureDeplacementSerpent) {
					deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
					horlageDeplacementSerpent = clock();
				}
				if (clock() > horlageDeplacementSouris + dureDeplacementSouris)
				{
					deplacerSouris(sourisX, sourisY, nbSouris);
					optimisationDeplacementPlacement(MurX, MurY, nbMurs, ancienSourisX, ancienSourisY, sourisX, sourisY, nbSouris);
					horlageDeplacementSouris = clock();
				}
			}
			if (clock() > (horlageJeu + durePartie)) {
				gotoXY(0, 2);
				cout << "Temps ecoule !!! " << endl;
			}
		}
		//saisie de niveau
		nbSouris = saisirNiveau();
	}

	//deplacer les souris 
	if (clock() > horlageDeplacementSouris + dureDeplacementSouris)
	{
		//deplacerSouris(sourisX, sourisY, nbSouris);
		//optimisationDeplacementPlacement(MurX, MurY, nbMurs,ancienSourisX,ancienSourisY,sourisX,sourisY, nbSouris);
		//optimisationDeplacement(ancienserpentX,ancienserpentY, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
		horlageDeplacementSouris = clock();
	}
	return 0;
}


// ************
// * PARTIE I *
// ************
void afficherTerrain(int nbLignes, int nbColonnes)
/*
Tâche: afficher le contour du terrain
Paramètres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
{
	cadre(NB_COLONNE_INITIAL, NB_LIGNE_INITIAL, nbColonnes + NB_COLONNE_INITIAL, nbLignes + NB_LIGNE_INITIAL, FOREGROUND_BLUE);
}

int recupererTouche()
/*
Tâche: tester si le joueur a appuyé sur une touche
Retour: retourner le code ASCII de la touche pressée, -1 sinon
*/
{
	// 	utiliser la fonction _kbhit() de conio.h qui retourne une valeur non 
	//	nulle si une touche a été pressée
	int appuyerOuNon;
	if (_kbhit())
	{
		appuyerOuNon = _getch();
	}
	else appuyerOuNon = -1;
	return appuyerOuNon;
}

int calculerDirectionTouche(int touche)
/*
Tâche: calculer la direction correspondant à une touche ou -1
Paramètre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond à la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
{
	int direction;
	const int directionHaut = 2, directionBas = 3, directionDroite = 0, directionGauche = 1;
	switch (touche)
	{
	case 'a': direction = directionDroite; break;
	case 'd': direction = directionGauche; break;
	case 'w': direction = directionHaut; break;
	case 's': direction = directionBas; break;
	default:  direction = -1;
	}
	return direction;
}

void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
/*
Tâche: calculer une position aléatoire sur le terrain
Paramètres: les dimensions du terrain en entrée et les coordonnées de la position aléatoire en sortie
*/
{
	posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 1);
	posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 1);
	gotoXY(posX, posY);
}

void deplacerSerpentI(int direction, int &posX, int &posY)
/*
Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le serpent est à l'écran avant l'appel et au retour de la fonction
Paramètres: la direction du serpent en entrée, et la position du serpent en	entrée / sortie
*/
{
	//effacer la position precedente de serpent 
	gotoXY(posX, posY);
	cout << " ";
	//deplacer le serpent 
	switch (direction)
	{
	case 0: posX--; break;
	case 1: posX++; break;
	case 2: posY--; break;
	case 3: posY++; break;
	}
	gotoXY(posX, posY);
}




// *************
// * PARTIE II *
// *************
int saisirNiveau()
/*
Tâche: lire le niveau de difficulté avec tests de validation d'entrée
Retour: le niveau (= le nombre de souris initialement sur le terrain)
*/
{
	int nbSouris;
	//saisie de niveau 
	cout << " Quel niveau (1-20) ? ";
	nbSouris = validationLogique();

	//effacer le niveau precedent 
	gotoXY(0, 0);
	cout << "                                                                         " << endl;
	//les niveaux 
	switch (nbSouris)
	{
	case 1: cout << " le niveau 1 : " << endl; break;
	case 2: cout << " le niveau 2 : " << endl; break;
	case 3: cout << " le niveau 3 : " << endl; break;
	case 4: cout << " le niveau 4 : " << endl; break;
	case 5: cout << " le niveau 5 : " << endl; break;
	case 6: cout << " le niveau 6 : " << endl; break;
	case 7: cout << " le niveau 7 : " << endl; break;
	case 8: cout << " le niveau 8 : " << endl; break;
	case 9: cout << " le niveau 9 : " << endl; break;
	case 10: cout << " le niveau 10 : " << endl; break;
	case 11: cout << " le niveau 11 : " << endl; break;
	case 12: cout << " le niveau 12 : " << endl; break;
	case 13: cout << " le niveau 13 : " << endl; break;
	case 14: cout << " le niveau 14 : " << endl; break;
	case 15: cout << " le niveau 15 : " << endl; break;
	case 16: cout << " le niveau 16 : " << endl; break;
	case 17: cout << " le niveau 17 : " << endl; break;
	case 18: cout << " le niveau 18 : " << endl; break;
	case 19: cout << " le niveau 19 : " << endl; break;
	case 20: cout << " le niveau 20 : " << endl;
	}
	return nbSouris;
}
void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris)
	/*
	Tâche: générer les nbSouris aléatoirement sur le terrain
	Paramètres: les dimensions du terrain, les tableaux de coordonnées et le
	nombre de souris
	*/
{
	int posX, posY;
	// gposYeneration des souris en foctions du  niveau 
	for (int i = 0; i < nbSouris; i++)
	{

		posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 1);
		posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 1);

		sourisX[i] = posX;
		sourisY[i] = posY;
		// regenerartion d'un autre nombre aleatoire si il existe precedement  
		for (int j = 0; j < i; j++)
		{
			if (sourisX[i] == sourisX[j])
				while (sourisX[i] == sourisX[j]) {
					sourisX[i] = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 1);
				}
		}
		gotoXY(posX, posY);
	}
}

void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: afficher les souris
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{
	for (int i = 0; i < nbSouris; i++)
	{
		gotoXY(sourisX[i], sourisY[i]);
		cout << char(2);
		color(FOREGROUND_GREEN);

	}
}
//fonction pour les conditions pour que le serpent reste  vivant  :p :p 
bool finDeVie(int MurX[], int MurY[], int nbMurs, int serpentX[],
	int serpentY[], int tailleSerpent, int sourisX[], int sourisY[], int nbSouris) {
	bool gameOver = false;
	//condition de ne pas enfoncer dans lui meme, sinon mort 
	for (int i = 2; i <= tailleSerpent; i++) {
		if ((serpentX[0] == serpentX[i]) && (serpentY[0] == serpentY[i])) {
			gameOver = true;
			gotoXY(45, 0);
			cout << "Game Over" << endl;
		}
	}
	// condition si il enfonce dans un mur la partie est termine
	for (int i = 0; i < nbMurs * 3; i++)
	{
		if ((serpentX[0] == MurX[i]) && (serpentY[0] == MurY[i])) {
			gameOver = true;
			gotoXY(45, 0);
			cout << "Game Over" << endl;
		}
	}
	//niveau simple
	for (int i = 0; i < tailleSerpent; i++)
	{
		if (serpentX[0] > nbColonnes + NB_COLONNE_INITIAL - 1)
		{
			gotoXY(serpentX[0], serpentY[0]);
			cout << ' ';
			serpentX[i] = 1 + NB_COLONNE_INITIAL;
		}
		else if (serpentY[0] > nbLignes + NB_LIGNE_INITIAL - 1)
		{
			gotoXY(serpentX[0], serpentY[0]);
			cout << ' ';
			serpentY[i] = 1 + NB_LIGNE_INITIAL;
		}
		else if (serpentX[0] < NB_COLONNE_INITIAL)
		{
			gotoXY(serpentX[0], serpentY[0]);
			cout << ' ';
			serpentX[i] = nbColonnes + NB_COLONNE_INITIAL - 1;
		}
		else if (serpentY[0] < 1 + NB_LIGNE_INITIAL)
		{
			gotoXY(serpentX[0], serpentY[0]);
			cout << ' ';
			serpentY[i] = nbLignes + NB_LIGNE_INITIAL - 1;
		}
	}
	return gameOver;
}
// fonction fin de vie niveau difficile 
bool finDeVieNiveauDifficile(int MurX[], int MurY[], int nbMurs, int serpentX[],
	int serpentY[], int tailleSerpent, int sourisX[], int sourisY[], int nbSouris) {
	bool gameOver = false;
	//condition de ne pas enfoncer dans lui meme, sinon mort 
	for (int i = 2; i <= tailleSerpent; i++) {
		if ((serpentX[0] == serpentX[i]) && (serpentY[0] == serpentY[i])) {
			gameOver = true;
			gotoXY(45, 0);
			cout << "Game Over" << endl;
		}
	}
	// condition si il enfonce dans un mur la partie est termine
	for (int i = 0; i < nbMurs * 3; i++)
	{
		if ((serpentX[0] == MurX[i]) && (serpentY[0] == MurY[i])) {
			gameOver = true;
			gotoXY(45, 0);
			cout << "Game Over" << endl;
		}
	}
	//condition de mur qu'on il enfonce dans le mur 

	if (serpentX[0] > nbColonnes + NB_COLONNE_INITIAL - 1 || serpentX[0] < NB_COLONNE_INITIAL + 1
	|| serpentY[0] > nbLignes + NB_LIGNE_INITIAL - 1 || serpentY[0] < NB_LIGNE_INITIAL + 1)
	{
	gameOver = true;
	gotoXY(45, 0);
	cout << "Game Over" << endl;
	}
	return gameOver;
}

void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le serpent est à l'écran avant l'appel et au retour de la fonction
	Paramètres: en entrée : la direction du serpent,
	en entrée/sortie :	les tableaux de coordonnées du serpent,
	la taille du serpent,
	les tableaux de coordonnées des souris et
	le nombre de souris
	*/
{
	int teteSerpentX = serpentX[0];
	int teteSerpentY = serpentY[0];
	bool collision = false;
	for (int i = tailleSerpent - 1; i >= 0; i--)
	{
		serpentX[i + 1] = serpentX[i];
		serpentY[i + 1] = serpentY[i];
	}

	switch (direction)
	{
	case 0: teteSerpentX--; break;
	case 1: teteSerpentX++; break;
	case 2: teteSerpentY--; break;
	case 3: teteSerpentY++; break;
	}
	serpentX[0] = teteSerpentX;
	serpentY[0] = teteSerpentY;
	if (direction != -1) {
		gotoXY(serpentX[0], serpentY[0]);
		cout << char(153);
		color(0x0002);
		gotoXY(serpentX[1], serpentY[1]);
		cout << char(207);
		color(0x0004 + 0x0008);
		gotoXY(serpentX[tailleSerpent], serpentY[tailleSerpent]);
		cout << ' ';
		if (tailleSerpent != 1)
		{
			gotoXY(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
			switch (direction)
			{
			case 1:cout << char(174); break;
			case 0:cout << char(175); break;
			case 2:cout << char(190); break;
			case 3:cout << char(94); break;
			}
		}
	}
	collision = testerCollision(serpentX[0], serpentY[0], sourisX, sourisY, nbSouris);
	if (collision) {
		tailleSerpent++;
		score = score + point;
		gotoXY(0, 0);
		cout << "Votre score est : " << score << "                   ";
	}
}

bool testerCollision(int posX, int posY,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: tester si (posX, posY) est la position d'une souris. Si oui, retirer
	la souris de la population de souris
	Paramètres: la position de la tête du serpent, les tableaux de coordonnées
	des souris et le nombre de souris
	Retour: true si collision, false sinon
	*/
{
	bool collision = false;
	for (int i = 0; i < nbSouris; i++)
	{
		if (posX == sourisX[i] && posY == sourisY[i])
		{
			//generer un beep (son)
			Beep(1000, 40);
			// J'écrase la souris bouffée
			for (int j = i; j < nbSouris; j++)
			{
				sourisX[j] = sourisX[j + 1];
				sourisY[j] = sourisY[j + 1];
			}
			//je decrimente le nombre du souris 

			nbSouris--;
			collision = true;
		}
	}
	return collision;
}

// **********************
// * PARTIE III : BONUS *
// **********************

void deplacerSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: déplacer les souris (aléatoirement ou intelligemment)
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{
	int ancienSourisX[25]; int ancienSourisY[25];

	for (int i = 0; i < nbSouris; i++)
	{
		ancienSourisX[i] = sourisX[i];
		ancienSourisY[i] = sourisY[i];
	};

	for (int i = 0; i < nbSouris; i++)
	{
		//deplacement aleatoire de souris 
		int direction = rand() % NB_DEPLACEMENT;
		//effacer la position precedente de souris 
		gotoXY(sourisX[i], sourisY[i]);
		cout << " ";
		//deplacer la souris 
		switch (direction)
		{
		case 0: sourisX[i]--; break;
		case 1: sourisX[i]++; break;
		case 2: sourisY[i]--; break;
		case 3: sourisY[i]++; break;
		}
		if (sourisX[i] > nbColonnes + NB_COLONNE_INITIAL - 1 || sourisX[i]  < NB_COLONNE_INITIAL + 1
			|| sourisY[i] > nbLignes + NB_LIGNE_INITIAL - 1 || sourisY[i] < NB_LIGNE_INITIAL + 1)
		{
			sourisX[i] = ancienSourisX[i];
			sourisY[i] = ancienSourisY[i];
		}
		gotoXY(sourisX[i], sourisY[i]);
		cout << char(2);
		color(FOREGROUND_GREEN);
		
	}
}
// fonction optimisation de deplacement et placement des murs 
void optimisationDeplacementPlacement(int MurX[], int MurY[], int nbMurs, int ancienSourisX[], int ancienSourisY[],
	int sourisX[], int sourisY[], int nbSouris) {

	for (int i = 0; i < nbSouris; i++)
	{
		ancienSourisX[i] = sourisX[i];
		ancienSourisY[i] = sourisY[i];
		for (int j = 0; j < nbMurs * 3; j++)
		{
			if ((sourisX[i] == MurX[j]) && (sourisY[i] == MurY[j])) {
				//effacer la position precedente de souris 
				gotoXY(MurX[j], MurY[j]);
				cout << char(219);
				sourisX[i] = ancienSourisX[i];
				sourisY[i] = ancienSourisY[i];
				gotoXY(sourisX[i], sourisY[i]);
				cout << char(2);
				color(FOREGROUND_GREEN);
			}
		}
	}
}
//fonction placer mur intelligent 
void placerMursAvance(int MurX[], int MurY[], int nbLignes, int nbColonnes, int nbMurs) {

	int posX, posY;
	int murVertical = nbMurs / 2;
	int murHoriz = nbMurs - murVertical;
	const int tailleMurMax = 3;
	for (int i = 0; i < murVertical; i++)
	{
		posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 4);
		posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 4);
		for (int j = 0; j < tailleMurMax; j++)
		{
			gotoXY(posX, posY + j);
			cout << char(219);
			MurX[i*tailleMurMax + j] = posX;
			MurY[i*tailleMurMax + j] = posY + j;
		}
	}
	for (int i = 0; i < murHoriz; i++)
	{
		posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 4);
		posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 4);
		for (int j = 0; j < tailleMurMax; j++)
		{
			gotoXY(posX + j, posY);
			cout << char(219);
			MurX[i*tailleMurMax + (murVertical * 3) + j] = posX + j;
			MurY[i*tailleMurMax + (murVertical * 3) + j] = posY;
		}
	}

}
void placerMurs(int nbLignes, int nbColonnes, int nbMurs)
/*
Tâche: placer des murs aléatoirement sur le terrain de jeu
Paramètres: les dimensions du terrain et le nombre de murs à placer
*/
{
	int posX, posY;
	int murVertical = nbMurs / 2;
	int murHoriz = nbMurs - murVertical;
	const int tailleMurMax = 3;
	for (int i = 0; i < murVertical; i++)
	{
		posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 4);
		posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 4);
		for (int i = 0; i < tailleMurMax; i++)
		{
			gotoXY(posX, posY + i);
			cout << char(219);
		}
	}
	for (int i = 0; i < murHoriz; i++)
	{
		posX = NB_COLONNE_INITIAL + 1 + rand() % (nbColonnes - 4);
		posY = NB_LIGNE_INITIAL + 1 + rand() % (nbLignes - 4);
		for (int i = 0; i < tailleMurMax; i++)
		{
			gotoXY(posX + i, posY);
			cout << char(219);
		}
	}
}
//declaration des fonctions validation de saisie 
int validationNumerique() {
	int n;
	bool validation = false;

	do {
		cin >> n;

		if (cin.fail() || cin.peek() != '\n') {
			cout << "Erreur : Entrez un level Valide ..." << endl;
			cin.clear();
			cin.ignore(512, '\n');	//	
		}
		else {
			validation = true;
		}
	} while (!validation);
	return n;
}

int validationLogique() {
	int n;
	bool validation = false;
	do {
		n = validationNumerique();
		if (n < 1 || n > 20) {
			cout << "Erreur : Dans ce jeu maximun level c'est 20" << endl;
		}
		else {
			validation = true;
		}

	} while (!validation);
	return n;
}