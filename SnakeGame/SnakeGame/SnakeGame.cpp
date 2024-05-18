#include <conio.h>
#include <iostream>
#include <windows.h>
using namespace std;

bool gameOver;

//Variáveis para dimensão do mapa
const int width = 20;
const int height = 20;

//Variáveis para posição da cabeça, da fruta e da pontuação
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int dificuldade;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void ImprimirLogo() {
	system("cls");
	cout << "                                    " << endl;
	cout << "                                             " << endl;
	cout << "                              $$     " << endl;
	cout << "                              $$      " << endl;
	cout << " $$$$$$$  $$$$$$$    $$$$$$   $$    $$   $$$$$$   " << endl;
	cout << "$$        $$    $$        $$  $$   $$   $$    $$  " << endl;
	cout << " $$$$$$   $$    $$   $$$$$$$  $$$$$$    $$$$$$$$  " << endl;
	cout << "      $$  $$    $$  $$    $$  $$   $$   $$        " << endl;
	cout << "$$$$$$$   $$    $$   $$$$$$$  $$    $$   $$$$$$$ " << endl << endl;
	cout << "                                                                            " << endl;
	cout << "                       $$$$$$    $$$$$$   $$$$$$ $$$$    $$$$$$" << endl;
	cout << "                      $$    $$        $$  $$   $$   $$  $$    $$" << endl;
	cout << "                      $$    $$   $$$$$$$  $$   $$   $$  $$$$$$$$" << endl;
	cout << "                      $$    $$  $$    $$  $$   $$   $$  $$      " << endl;
	cout << "                       $$$$$$$   $$$$$$   $$   $$   $$   $$$$$$$" << endl;
	cout << "                            $$" << endl;
	cout << "                       $$$$$$                                 Por Lucas Miguel Leite" << endl << endl;
}

void Setup() {
	gameOver = false;
	dir = STOP;//Só haverá movimento se for pressionado uma tecla

	//Posicionando a cabeça da cobra no meio do mapa
	x = width / 2;
	y = height / 2;

	//Gerar frutas
	fruitX = rand() % width;
	fruitY = rand() % height;

	score = 0;
}

void Draw() {
	//Limpando a tela
	system("cls"); // Usar no Windows
	//system("clear"); // Usar no Linux

	//Desenhando a matriz de ´#´
	for (int i = 0; i < width + 2; i++) //Preenchendo a parte de cima do mapa (+2 para preencher os espaços em vazio que ficam)
		cout << '#';
	cout << endl;

	for (int i = 0; i < height; i++) { //Preenchendo as laterais do mapa
		for (int j = 0; j < height; j++) {
			if (j == 0)
				cout << "#";

			if (i == y && j == x)// imprime a cabeça da cobra
				cout << "O";
			else if (i == fruitY && j == fruitX) // imprime a fruta
				cout << "F";
			else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}

			if (j == width - 1)
				cout << "#";
			
		}
		cout << endl; //Verifica a próxima linha
	}

	for (int i = 0; i < width + 2; i++) //Preenchendo a parte de baixo do mapa (+2 para preencher os espaços em vaziio que ficam)
		cout << "#";
	cout << endl;
	cout << "Score: " << score << endl << endl << "Aperte 'x' para sair" << endl;
}

void Input() {
	if (_kbhit()) { //função que verifica no console se alguma tecla foi pressionada
		switch (_getch()) { //pega a tecla pressionada
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir) {
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		default:
			break;
	}

	//Fácil
	if (dificuldade == 1) {
		if (x >= width) 
			x = 0; 
		else if (x < 0) 
			x = width - 1; //atravessar as paredes (sem colisão)
		if (y >= height) 
			y = 0; 
		else if (y < 0) 
			y = height - 1;
	}

	//Normal
	if (dificuldade == 2) {
		if (x >= width) 
			x = 0; 
		else if (x < 0) 
			x = width - 1; //atravessar as paredes (sem colisão)
		if (y >= height) 
			y = 0; 
		else if (y < 0) 
			y = height - 1;

		for (int i = 0; i < nTail; i++) { //colisão com a calda
			if (tailX[i] == x && tailY[i] == y)
				gameOver = true;
		}
	}

	//Difícil
	if (dificuldade == 3) {
		if (x > width || x < 0 || y > height || y < 0) //colisão com as paredes
			gameOver = true;

		for (int i = 0; i < nTail; i++) { //colisão com a calda
			if (tailX[i] == x && tailY[i] == y)
				gameOver = true;
		}
	}

	if (x == fruitX && y == fruitY) {
		score++; //pontuação
		//Gerar frutas
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++; //tamanho da calda
	}
}

void Jogo() {
	Setup();
	while (!gameOver) {
		Draw();
		Input();
		Logic();
		Sleep(50); //Usar no Windows
		//sleep(50) //Usar no 
	}
	system("cls"); // Usar no Windows
	//system("clear"); // Usar no Linux
	cout << "Game Over" << endl << endl << "Score final: " << score << endl;
}

void Opcao() {
	int opcao;
	system("cls");
	ImprimirLogo();

	cout << "1 - FACIL (RECOMENDADO PARA INICIANTES): A COBRINHA PODE PASSAR PELA PAREDE E PODE SE ENCOSTAR" << endl << "2 - NORMAL (RECOMENDADO PARA PESSOAS ACOSTUMADAS COM O JOGO): A COBRINHA PODE PASSAR PELA PAREDE, MAS ELA NAO PODE SE ENCOSTAR" << endl << "3 - DIFICIL (RECOMENDADO PARA JOGADORES EXPERIENTES): A COBRINHA NAO PODE PASSAR PELA PAREDE E NEM PODE SE ENCOSTAR" << endl << endl << "Escolha: ";
	cin >> opcao;
	switch (opcao) {
	case 1:
		dificuldade = opcao;
		break;
	case 2:
		dificuldade = opcao;
		break;
	case 3:
		dificuldade = opcao;
		break;
	default:
		break;
	}
	Jogo();
}

void Menu() {
	int opcao;
	ImprimirLogo();

	cout << "1 - JOGAR" << endl << "2 - SAIR" << endl << endl << "Escolha: ";
	cin >> opcao;
	switch (opcao) {
	case 1:
		Opcao();
		break;
	default:
		break;
	}
}

int main() {
	Menu();
	return 0;
}