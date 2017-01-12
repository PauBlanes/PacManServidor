// PacmanThreads.cpp : Defines the entry point for the console application.
//


#include <windows.h>
#include <iostream>
#include <thread>   // std::thread
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
using namespace std;

mutex bloq1;

#define ARRIBA     72      // CONSTANTS AMB LES FLETXES DEL TECLAT
#define IZQUIERDA  75
#define DERECHA    77
#define ABAJO      80
#define ESC        27
#define ENTER      13

int backcolor = 0;
int dir = 0;
int x = 39, y = 22;
int anteriorpx, anteriorpy, ghostLastX, ghostLastY;

long int punts = 0;
int puntsCont = 0;
int vides = 3;

void setCColor(int color)
{
	static HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color | (backcolor * 0x10 + 0x100));
}

int color[7] = {
	0x009,//[]blau
	0x00E,//[]groc
	0x00C,//[]vermell
	0x002,//[]verd
	0x00B,//[]blau clar
	0x005,//[]lila
	0x00F//[]blanc
};

struct fantasma {
	int fdir; //direcció del fantasma que pot prendre valors del 0-3 i pot ser inicialitzat rand() % 4
	int _x, _y; // posicíó del fantasma
	int col; //color del fantasma
};

void gotoxy(int x, int y)  // funcio que posiciona el cursor a la coordenada (x,y)
{
	HANDLE hCon;
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
}

char mapa[50][100] = {
	"                                                      ",
	"                  AXXXXXXXXXXXXXXXXXXXB AXXXXXXXXXXXXXXXXXXXB",
	"                  Y___________________Y Y___________________Y",
	"                  Y_AXXXXXB_AXXXXXXXB_Y Y_AXXXXXXXB_AXXXXXB_Y",
	"                  Y_Y     Y_Y       Y_Y Y_Y       Y_Y     Y_Y",
	"                  Y_DXXXXXC_DXXXXXXXC_DXC_DXXXXXXXC_DXXXXXC_Y",
	"                  Y________|_________|___|_________|________Y",
	"                  Y_AXXXXXB_AXB_AXXXXXXXXXXXXXB_AXB_AXXXXXB_Y",
	"                  Y_DXXXXXC_Y Y_DXXXXB   AXXXXC_Y Y_DXXXXXC_Y",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  DXXXXXXXB_Y DXXXXB_Y   Y_AXXXXC Y_AXXXXXXXC",
	"                          Y_Y AXXXXC_DXXXC_DXXXXB Y_Y        ",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                  XXXXXXXXC_DXC_AXXXXXX XXXXXXB_DXC_DXXXXXXXX",
	"                  _________|____Y      *      Y____|_________",
	"                  XXXXXXXXB_AXB_DXXXXXXXXXXXXXC_AXB_AXXXXXXXX",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                          Y_Y Y_AXXXXXXXXXXXXXB_Y Y_Y        ",
	"                  AXXXXXXXC_DXC_DXXXXB   AXXXXC_DXC_DXXXXXXXB",
	"                  Y________|_________Y   Y_________|________Y",
	"                  Y_AXXXXXB_AXXXXXXB_Y   Y_AXXXXXXB_AXXXXXB_Y",
	"                  Y_DXXXB Y_DXXXXXXC_DXXXC_DXXXXXXC_Y AXXXC_Y",
	"                  Y_____Y Y_________|_____|_________Y Y_____Y",
	"                  DXXXB_Y Y_AXB_AXXXXXXXXXXXXXB_AXB_Y Y_AXXXC",
	"                  AXXXC_DXC_Y Y_DXXXXB   AXXXXC_Y Y_DXC_DXXXB",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  Y_AXXXXXXXC DXXXXB_Y   Y_AXXXXC DXXXXXXXB_Y",
	"                  Y_DXXXXXXXXXXXXXXC_DXXXC_DXXXXXXXXXXXXXXC_Y",
	"                  Y_________________|_____|_________________Y",
	"                  DXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXC",
};


void pintar_mapa()// Funcio que imprimeix el mapa basant-se en el mapa codificat
{

	for (int i = 0; i < 78; i++) {
		for (int j = 0; j < 30; j++) {
			gotoxy(i, j);

			if (mapa[j][i] == 'X') {
				setCColor(color[0]);
				printf("%c", 205);
			}
			if (mapa[j][i] == '_') {
				setCColor(color[0]);
				printf("%c", 250);
			}
			else if (mapa[j][i] == 'Y') {
				setCColor(color[0]);
				printf("%c", 186);
			}
			else if (mapa[j][i] == 'A') {
				setCColor(color[0]);
				printf("%c", 201);
			}
			else if (mapa[j][i] == 'B') {
				setCColor(color[0]);
				printf("%c", 187);
			}
			else if (mapa[j][i] == 'C') {
				setCColor(color[0]);
				printf("%c", 188);
			}
			else if (mapa[j][i] == 'D') {
				setCColor(color[0]);
				printf("%c", 200);
			}
		}
	}
}

void tecleig() {
	if (_kbhit()) {
		char tecla = _getch();
		switch (tecla) {
		case ARRIBA:
			dir = 0;
			break;

		case ABAJO:
			dir = 1;
			break;

		case DERECHA:
			dir = 2;
			break;

		case IZQUIERDA:
			dir = 3;
			break;
		}
	}

}

void pintarPacman(int x, int y) {
	setCColor(color[1]);
	gotoxy(x, y);
	printf("%c", 6);
}

void borrarPacman(int x, int y) {
	gotoxy(x, y); printf(" ");
	
}
void mourePacman()
{
	//TODO guardar la posició anterior del pacman
	anteriorpx = x;	anteriorpy = y;
	//TODO detectar cap a on moure pacman

	tecleig();

	//TODO comprovar si es pot moure en aquesta direcció
	if (x < 18) {
		borrarPacman(x, y);
		x = 60;
	}
	if (x > 60) {
		borrarPacman(x, y);
		x = 18;
	}
	switch (dir)
	{
	case 0:

		if (mapa[y - 1][x] != 'A' && mapa[y - 1][x] != 'B' && mapa[y - 1][x] != 'C' && mapa[y - 1][x] != 'D' && mapa[y - 1][x] != 'X' && mapa[y - 1][x] != 'Y') {
			y--;
		}

		break;
	case 1:

		if (mapa[y + 1][x] != 'A' && mapa[y + 1][x] != 'B' && mapa[y + 1][x] != 'C' && mapa[y + 1][x] != 'D' && mapa[y + 1][x] != 'X' && mapa[y + 1][x] != 'Y') {
			y++;
		}

		break;
	case 2:

		if (mapa[y][x + 1] != 'A' && mapa[y][x + 1] != 'B' && mapa[y][x + 1] != 'C' && mapa[y][x + 1] != 'D' && mapa[y][x + 1] != 'X' && mapa[y][x + 1] != 'Y') {
			x++;
		}

		break;
	case 3:

		if (mapa[y][x - 1] != 'A' && mapa[y][x - 1] != 'B' && mapa[y][x - 1] != 'C' && mapa[y][x - 1] != 'D' && mapa[y][x - 1] != 'X' && mapa[y][x - 1] != 'Y') {

			x--;
		}

		break;

	}

	//PUNTUACIO
	bloq1.lock();
	if (mapa[y][x] == '_') {
		mapa[y][x] = ' ';
		punts += 5;
		puntsCont += 5;
		if (punts % 25 == 0) {
			vides += 1;
			if (vides >= 3) {
				vides = 3;
			}
		}
	}
	bloq1.unlock();

	//TODO esborrar el pacman de l'anterior posició i imprimir-lo en la nova
	bloq1.lock();
	borrarPacman(anteriorpx, anteriorpy);
	bloq1.unlock();
	bloq1.lock();
	pintarPacman(x, y);
	bloq1.unlock();
}

fantasma inicialitzarFantasma(int x, int y, int color) {
	//TODO inicialitzar el fantasma amb les dades donades i la direcció rand() % 4
	fantasma ghost;
	ghost._x = x;
	ghost._y = y;
	ghost.col = color;
	ghost.fdir = rand() % 4;
	
	return ghost;
}

void pintarFantasma(fantasma ghost) {
	
	//TODO seleccionar color (funció)
	setCColor(color[ghost.col]);
	//TODO anar a la posicio indicada (funció)
	gotoxy(ghost._x, ghost._y);
	//TODO imprimir el caracter amb codi ASCII 6
	printf("%c", 6);
	
}
void borrarFantasma(fantasma ghost, int x, int y) {
	//TODO anar a la posicio indicada (funció)
	gotoxy(x, y);
	//TODO imprimir el caracter d'espai
	printf(" ");
}
void xocPacman(fantasma ghost) {
	//TODO detectar si el fantasma colisiona amb l'actual o l'anterior posició del Pacman, de ser aixi posicionar al jugador a x=39, y=22, dir = 4 i disminuir en una les vides
	if (y == ghost._y&&x == ghost._x || (ghost.fdir == 0 && ghost.fdir == 1 || ghost.fdir == 1 && ghost.fdir == 0
		|| ghost.fdir == 2 && ghost.fdir == 3 || ghost.fdir == 3 && ghost.fdir == 2) && (ghost._y == anteriorpy && ghost._x == anteriorpx)) {
		x = 39;
		y = 22;
		dir = 4;
		vides--;
		punts -= 25;
	}
}

void moureFantasma(fantasma &ghost, int random) {
	ghostLastX = ghost._x, ghostLastY = ghost._y;

	if (mapa[ghost._y][ghost._x] == '|') {
		ghost.fdir = random;
	}

	if (mapa[ghost._y][ghost._x] == '*') {
		ghost.fdir = 0;
	}

	bloq1.lock();
	if (ghost._x <= 17)	ghost._x = 61;
	else if (ghost._x >= 62)	ghost._x = 18;
	bloq1.unlock();
	
	//TODO intentar seguir en la direcció que anava, en cas de trobar-se amb algun obstacle (X,A,B,C,D) intentar-lo redirigir
	
	switch (ghost.fdir)
	{
		
	case 0:
		if (mapa[ghost._y - 1][ghost._x] != 'A' && mapa[ghost._y - 1][ghost._x] != 'B' && mapa[ghost._y - 1][ghost._x] != 'C' && mapa[ghost._y - 1][ghost._x] != 'D' && mapa[ghost._y - 1][ghost._x] != 'X' && mapa[ghost._y - 1][ghost._x] != 'Y') {
			ghost._y--;
		}
		else	ghost.fdir = random;
		break;
	case 1:
		if (mapa[ghost._y + 1][ghost._x] != 'A' && mapa[ghost._y + 1][ghost._x] != 'B' && mapa[ghost._y + 1][ghost._x] != 'C' && mapa[ghost._y + 1][ghost._x] != 'D' && mapa[ghost._y + 1][ghost._x] != 'X' && mapa[ghost._y + 1][ghost._x] != 'Y') {
			ghost._y++;
		}

		else	ghost.fdir = random;
		break;
	case 2:
		if (mapa[ghost._y][ghost._x + 1] != 'A' && mapa[ghost._y][ghost._x + 1] != 'B' && mapa[ghost._y][ghost._x + 1] != 'C' && mapa[ghost._y][ghost._x + 1] != 'D' && mapa[ghost._y][ghost._x + 1] != 'X' && mapa[ghost._y][ghost._x + 1] != 'Y') {
			ghost._x++;
		}			
		else	ghost.fdir = random;
		break;
	case 3:
		if (mapa[ghost._y][ghost._x - 1] != 'A' && mapa[ghost._y][ghost._x - 1] != 'B' && mapa[ghost._y][ghost._x - 1] != 'C' && mapa[ghost._y][ghost._x - 1] != 'D' && mapa[ghost._y][ghost._x - 1] != 'X' && mapa[ghost._y][ghost._x - 1] != 'Y') {
			ghost._x--;
		}
		else	ghost.fdir = random;
		break;
	}		
	
	
	//TODO Esborrar el fantasma
	bloq1.lock();
	borrarFantasma(ghost, ghostLastX, ghostLastY);
	bloq1.unlock();
	

	//TODO Printar el fantasma
	bloq1.lock();
	pintarFantasma(ghost);
	bloq1.unlock();
	bloq1.lock();
	if (mapa[ghostLastX][ghostLastY] == '_') {
		setCColor(color[1]);
		gotoxy(ghostLastX, ghostLastY);
		printf("%c", 250);
	}
	bloq1.unlock();

	
}


void marcador() {
	bloq1.lock();
	setCColor(color[6]);
	gotoxy(5, 6); printf("PUNTS");
	bloq1.unlock();
	
	bloq1.lock();
	setCColor(color[6]);
	gotoxy(5, 7); printf("%ld", punts);
	bloq1.unlock();

	bloq1.lock();
	setCColor(color[1]);
	gotoxy(5, 25); printf("VIDES");
	for (int i = 0; i <= vides; i++) {
		gotoxy(5, i + 27); printf(" ");
	}
	for (int i = 0; i < vides; i++) {
		gotoxy(5, i + 27); printf("%c", 2);
	}
	bloq1.unlock();
	bloq1.lock();
	setCColor(color[2]);
	gotoxy(70, 27); printf("%c", 169);
	bloq1.unlock();
}

int main() {

	fantasma ghostA = inicialitzarFantasma(45, 14, 2);
	fantasma ghostB = inicialitzarFantasma(43, 14, 3);
	fantasma ghostC = inicialitzarFantasma(40, 14, 4);
	fantasma ghostD = inicialitzarFantasma(39, 14, 5);
	pintar_mapa();


	while (vides > 0 && punts < 1950) {



		//TODO Moure el Pacman

		thread moureP(mourePacman);
		//Sleep(55);
		thread marc(marcador);

		//els fantasmes
		//thread moureFA(moureFantasma, ghostA, rand() % 4); //No hem aconseguit que el fantasma es mogui utilitzant threads de manera que ho farem amb funcions normals.	
		//thread moureFB(moureFantasma, ghostB);
		//thread moureFC(moureFantasma, ghostC);
		//thread moureFD(moureFantasma, ghostD);
		moureFantasma(ghostA, rand () % 4);
		moureFantasma(ghostB, rand() % 4);
		moureFantasma(ghostC, rand() % 4);
		moureFantasma(ghostD, rand() % 4);
		
		//i detectar els possibles xocs
		thread xocA(xocPacman, ghostA);
		thread xocB(xocPacman, ghostB);
		thread xocC(xocPacman, ghostC);
		thread xocD(xocPacman, ghostD);

		//els joins
		marc.join();
		moureP.join();

		//moureFA.join();
		//moureFB.join();
		//moureFC.join();
		//moureFD.join();
		xocA.join();
		xocB.join();
		xocC.join();
		xocD.join();

		if (puntsCont == 25 && vides <3) {
			puntsCont = 0;
			vides++;
			
		}

		Sleep(110);
	}




	for (int i = 0; i <= vides; i++) {
		gotoxy(5, i + 27);
		printf(" ");
	}

	system("pause>NULL");
	return 0;

}
