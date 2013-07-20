// clear && gcc really.c -o project -Wall -lSDL -lSDL_image && ./project

// arquivo final do projeto

// main3 - quadrado que anda na tela


#include "math.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "stdlib.h"
#include "time.h"

#define SCREEN_W 600
#define SCREEN_H 400

// #define QNT_GREEN 7
// #define QNT_RED 5
// #define QNT_BLUE 5

#define QNT_BALLS 15

#define FILE_PLACE "records.bin"

typedef unsigned short int usint;

int records[5];

typedef struct {
	SDL_Surface *image;
	SDL_Rect place;
	float horizontalVelocity, verticallVelocity;
    int incX, incY; // incrementos (positivos e/ou negativos)
} Cursor; // a bolinha que segue o mouse

Cursor cursor;
SDL_Surface* screen;

enum typesOfBalls{
	Green,
	Blue,
	Red
}; // tipos das bolas, verde (preciso pegar), azul (pega as verdes e me da pontos), púrpura (ferra tudo), a amarela, que segue o mouse, não entra aqui.

typedef enum typesOfBalls typesOfBall;

typedef struct {
    SDL_Surface *image; // onde será a imagem em si. 

    int incX, incY; // incrementos (positivos e/ou negativos)

    int x, y, h, w;

    usint isActive; // para saber se ela ja está na tela, para limitar o número de bolas visíveis

    usint isFree; // se não está grudada. Só server para as Green Balls

    typesOfBall type;
} ball; // as bolas, todas, menos a amarela


int readRecords(){
	FILE *file = fopen(FILE_PLACE, "r");

	if (! file){
		printf("Erro ao abrir (ou gerar) o arquivo de recordes.\n");
		return 0;
	}

	printf("ARQUIVO ABERTO\n");

	fread(records, sizeof(int), 5, file);
	
	int i;

	for (i = 0; i < 5; i++)
	{
		printf("RECORD %d: %d\n", i, records[i]);
	}

	fclose(file);

	return 1;	
}

void sortRecords(){
	int aux = 0;

	int i, j;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			if (records[i] > records[j]){				
				aux = records[i];
				records[i] = records[j];
				records[j] = aux;
			}
		}
	}

}

int writeRecord(){
	FILE *file = fopen(FILE_PLACE, "w");

	if (! file){
		printf("Erro ao abrir (ou gerar) o arquivo de recordes.\n");
		return 0;
	}

	printf("FILE ABERTO\n");

	fwrite(records, sizeof(int), 5, file);

	fclose(file);

	int i;

	for (i = 0; i < 5; i++)
	{
		printf("RECORD %d: %d\n", i, records[i]);
	}


	return 1;	
}

int insertCurrentPoint(int point){
	int i, j, aux;

	for (i = 0; i < 5; i++){
		if (point > records[i]){

			records[4] = point;

			sortRecords();

			return 1;
		}
	}

	return 0; // não é um novo record
}

int SDL_CollideBoundingCircle(int x1, int y1, int r1, int x2, int y2, int r2, int offset){
	int xdiff = x2 - x1; //- diferença no eixo x
	int ydiff = y2 - y1; //- diferença no eixo y

	/* Distância entre os centros dos círculos ao quadrado */
	int dcentre_sq = (ydiff*ydiff) + (xdiff*xdiff);

	/* Calcula a soma dos raios ao quadrado */
	int r_sum_sq = r1 + r2;
	r_sum_sq *= r_sum_sq; //- melhor que (r1+r2)*(r1+r2)

	return (dcentre_sq - r_sum_sq <= (offset*offset));
}

ball balls[QNT_BALLS];

// ball greens[QNT_GREEN];
// ball reds[QNT_RED];
// ball blues[QNT_BLUE];


int points = 0;
int lifes = 2;

void freeAlreadyCounted(){
	int i;

	// for (i = 0; i < QNT_GREEN; i++){
	// 	if (! greens[i].isFree){
	// 		greens[i].isFree = 1;
	// 		greens[i].x = 50 * i;
	// 		greens[i].y = 50;
	// 	}
	// }

	for (i = 0; i < QNT_BALLS; i++){
		if ((balls[i].type == Green) && (!balls[i].isFree)){
			balls[i].isFree = 1;
			balls[i].x = 50 * i;
			balls[i].y = 50;	
		}
	}
}

// void testColisionsReds(){
// 	// entre as bolas vermelhas com as verdes

// 	int i, j;
// 	int colision;

// 	for (i = 0; i < QNT_RED; i++){
// 		for (j = 0; j < QNT_GREEN; j++){
// 			colision = SDL_CollideBoundingCircle(reds[i].x, reds[i].y, reds[i].h/2-1, greens[j].x, greens[j].y, reds[i].h/2-1, 5);

// 			if (colision){
// 				if (! greens[j].isFree){
// 					lifes --;
// 					freeAlreadyCounted();
// 				}
// 			}

// 		}
// 	}

// 	for (i = 0; i < QNT_RED; i++){
// 		colision = SDL_CollideBoundingCircle(reds[i].x, reds[i].y, reds[i].h/2-1, cursor.place.x, cursor.place.y, reds[i].h/2-1, 5);

// 		if (colision){
// 			lifes --;
// 			freeAlreadyCounted();
// 		}
// 	}
// }

// void testColisionsBlues(){
// 	// entre as bolas vermelhas com as verdes

// 	int i, j;
// 	int colision;

// 	for (i = 0; i < QNT_BLUE; i++){
// 		for (j = 0; j < QNT_GREEN; j++){
// 			colision = SDL_CollideBoundingCircle(blues[i].x, blues[i].y, blues[i].h/2-1, greens[j].x, greens[j].y, blues[i].h/2-1, 5);

// 			if (colision){
// 				if (! greens[j].isFree){
// 					points += getPoints();
// 					getPoints();
// 				}
// 			}

// 		}
// 	}

// 	for (i = 0; i < QNT_BLUE; i++){
// 		colision = SDL_CollideBoundingCircle(blues[i].x, blues[i].y, blues[i].h/2-1, cursor.place.x, cursor.place.y, blues[i].h/2-1, 5);

// 		if (colision){
// 			points += getPoints();
// 			getPoints();
// 		}
// 	}
// }

// void testColisionsGreens(){
// 	// entre as bolas verdes
// 	int c, c2, i;

// 	for (c = 0; c < QNT_GREEN; c++){
// 		for (c2 = 0; c2 < QNT_GREEN; c2++){
// 			if (c != c2){
// 				if (! greens[c2].isFree){
// 					i = SDL_CollideBoundingCircle(greens[c2].x, greens[c2].y, greens[c2].h/2-1, greens[c].x, greens[c].y, greens[c2].h/2-1, 5);					
// 					if (i){
// 						if (greens[c].isFree){
// 							greens[c].isFree = 0;
// 						}
// 					}
// 				}
// 			}
// 		}

// 		i = SDL_CollideBoundingCircle(cursor.place.x, cursor.place.y, cursor.place.h/2-1, greens[c].x, greens[c].y, cursor.place.h/2-1, 5);
// 		if (i){
// 			greens[c].isFree = 0;
// 		}else{
// 			if (greens[c].isFree){
// 				greens[c].y += greens[c].incY;
// 				if(((greens[c].x + greens[c].w) > screen->w) || (greens[c].x < 0)) 
// 				{
// 					greens[c].incX = -greens[c].incX;
// 				}
// 				if(((greens[c].y + greens[c].h) > screen->h) || (greens[c].y < 0))
// 				{
// 					greens[c].incY = -greens[c].incY;
// 				}
// 				greens[c].y += greens[c].incY;
// 				greens[c].x += greens[c].incX;
// 			}else{

// 			}
// 		}
// 	}
// }

void testColisions(){
	int i, j;
	int colision;

	for (i = 0; i < QNT_BALLS; i++){
		// aqui eu verei se a bola I está em choque com TODAS as outras bolas.
		// caso ela seja verde e não esteja livre. Se ela for verde, mas estiver livre
		// não importa se bateu ou não.
		if (balls[i].type == Green && (!balls[i].isFree)){
			for (j = i+1; j < QNT_BALLS; j++){
				colision = SDL_CollideBoundingCircle(balls[j].x, balls[j].y, balls[j].h/2-1, balls[i].x, balls[i].y, balls[j].h/2-1, 5);
				
				if (colision){
					if (balls[j].type == Red){
						lifes --;
						freeAlreadyCounted();
					}else{
						if (balls[j].type == Blue){
							points += getPoints();
							getPoints();
							
						}else{
							if (balls[j].type == Green && balls[j].isFree){
								balls[j].isFree = 0;
							}
						}
					}
				}
			}
		}
	}
}


int getDirection(){

	int random = rand() % 2;

	switch (random){
		case 0: return -1;
		case 1: return 1;
	}
}

int getPoints(){
	int i;

	int quantity = 0;

	// verei quantas bolinhas verdes tem presas a branca, para retornar a quantidade de pontos
	for (i = 0; i < QNT_BALLS; i++){
		if ((! balls[i].isFree) && balls[i].type == Green)
			quantity ++;
	}

	switch(quantity){
		case 3:
		freeAlreadyCounted();
		return 1;

		case 4:
		freeAlreadyCounted();
		return 5;

		case 5:
		freeAlreadyCounted();
		return 11;

		case 6:
		freeAlreadyCounted();
		return 17;

		case 7:
		freeAlreadyCounted();
		return 25;

		case 8:
		freeAlreadyCounted();
		return 33;

		case 9:
		freeAlreadyCounted();
		return 41;

		case 10:
		freeAlreadyCounted();
		return 51;

		default:
		freeAlreadyCounted();
		return 0;
	}
}

typesOfBall sortType(){
	usint type = (rand() * 100) % 10;

	if (type < 5)
		return Green;
	else if (type < 8)
		return Blue;
	else
		return Red;
}

int sortPositionX(){
	return (rand() % SCREEN_W);
}

int sortPositionY(){
	return (rand() % (SCREEN_H/2)); // só sai da parte de cima da tela
}

int main(int argc, char** argv)
{

	sortRecords();

	srand(time(NULL));

	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());

	SDL_Event event;

	int quit = 0;
	
	int c;

	for (c = 0; c < QNT_BALLS; c ++ ){
		balls[c].type = sortType();
	
		switch (balls[c].type){
			case Green:
			balls[c].image = IMG_Load("green.png");
			break;
			
			case Red:
			balls[c].image = IMG_Load("red.png");
			break;

			case Blue:
			balls[c].image = IMG_Load("blue.png");
			break;
		}
			balls[c].x = sortPositionX();
			balls[c].x = sortPositionY();

	//	balls[c].IMG_Load()
	}

	cursor.image = IMG_Load("yellow.png");
	cursor.incX = 1;
	cursor.incY = 1;
	cursor.place.x = 50;
	cursor.place.y = 0;
	cursor.place.w = 40;
	cursor.place.h = 40;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);

	readRecords();

	while(!quit)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = 1;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN)
				{
					quit = 1;
				}
			}
			else if (event.type == SDL_MOUSEMOTION){
				cursor.place.x = event.motion.x - cursor.place.h/2;
				cursor.place.y = event.motion.y - cursor.place.h/2;

				for (c = 0; c < QNT_BALLS; c++){
					if (balls[c].type == Green && (!balls[c].isFree)){
						// greens[c].isFree = 1;
						balls[c].x += event.motion.xrel;
						balls[c].y += event.motion.yrel;
					}
				}
			}
		}

		if (lifes > 0){
			testColisions();

			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

			SDL_Rect rect;

			for (c = 0; c < QNT_BALLS; c++){
				rect.x = balls[c].x;
				rect.y = balls[c].y;
				SDL_BlitSurface(balls[c].image, NULL, screen, &rect);
			}

			// for (c = 0; c < QNT_GREEN; c++){
			// 	rect.x = greens[c].x;
			// 	rect.y = greens[c].y;
			// 	SDL_BlitSurface(greens[c].image, NULL, screen, &rect);
			// }

			// for (c = 0; c < QNT_RED; c++){
			// 	rect.x = reds[c].x;
			// 	rect.y = reds[c].y;
			// 	SDL_BlitSurface(reds[c].image, NULL, screen, &rect);
			// }

			// for (c = 0; c < QNT_BLUE; c++){
			// 	rect.x = blues[c].x;
			// 	rect.y = blues[c].y;
			// 	SDL_BlitSurface(blues[c].image, NULL, screen, &rect);
			// }

			SDL_BlitSurface(cursor.image, NULL, screen, &cursor.place);

			SDL_WM_SetCaption("Projeto Final - Jonathan Silva", "Projeto final");

			SDL_Flip(screen);
		} else {
			insertCurrentPoint(points);

			points = 0;
		}
	}


	int i;

	for (i = 0; i < 5; i++){
		printf("Record %d: %d\n", i, records[i]);
	}

	writeRecord();


	SDL_Quit();

	return 0;
}