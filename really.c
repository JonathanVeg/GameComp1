// clear && gcc really.c -o project -Wall -lSDL -lSDL_image && ./project

// arquivo final do projeto

// main3 - quadrado que anda na tela


#include "math.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "stdlib.h"

#define SCREEN_W 600
#define SCREEN_H 400

#define QNT_GREEN 7
#define QNT_RED 7
#define QNT_BLUE 7

typedef unsigned short int usint;

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
    // SDL_Rect place;

    usint isFree; // se não está grudada. Só server para as Green Balls

    typesOfBall type;
} ball; // as bolas, todas, menos a amarela


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

ball greens[QNT_GREEN];
ball reds[QNT_RED];
ball blues[QNT_BLUE];


int points = 0;
int lifes = 2;

void testColisionsReds(){
	// entre as bolas vermelhas com as verdes

	int i, j;
	int colision;

	for (i = 0; i < QNT_RED; i++){
		for (j = 0; j < QNT_GREEN; j++){
			colision = SDL_CollideBoundingCircle(reds[i].x, reds[i].y, reds[i].h/2-1, greens[j].x, greens[j].y, reds[i].h/2-1, 5);

			if (colision){
				if (! greens[j].isFree){
					lifes --;
					freeAlreadyCounted();
				}
			}
			
		}
	}
	
	for (i = 0; i < QNT_RED; i++){
		colision = SDL_CollideBoundingCircle(reds[i].x, reds[i].y, reds[i].h/2-1, cursor.place.x, cursor.place.y, reds[i].h/2-1, 5);

		if (colision){
			lifes --;
			freeAlreadyCounted();
		}
	}
}

void freeAlreadyCounted(){
	int i;

	for (i = 0; i < QNT_GREEN; i++){
		if (! greens[i].isFree){
			greens[i].isFree = 1;
			greens[i].x = 50 * i;
			greens[i].y = 50;
		}
	}
}

int getPoints(){
	int i;

	int quantity = 0;

	// verei quantas bolinhas verdes tem presas a branca, para retornar a quantidade de pontos
	for (i = 0; i < QNT_GREEN; i++){
		if (! greens[i].isFree)
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

void testColisionsBlues(){
	// entre as bolas vermelhas com as verdes

	int i, j;
	int colision;

	for (i = 0; i < QNT_BLUE; i++){
		for (j = 0; j < QNT_GREEN; j++){
			colision = SDL_CollideBoundingCircle(blues[i].x, blues[i].y, blues[i].h/2-1, greens[j].x, greens[j].y, blues[i].h/2-1, 5);

			if (colision){
				if (! greens[j].isFree){
					points += getPoints();
					printf("Pontos %d\n", points);

					getPoints();
				}
			}
			
		}
	}
	
	for (i = 0; i < QNT_BLUE; i++){
		colision = SDL_CollideBoundingCircle(blues[i].x, blues[i].y, blues[i].h/2-1, cursor.place.x, cursor.place.y, blues[i].h/2-1, 5);

		if (colision){
			points += getPoints();
			printf("Pontos %d\n", points);

			getPoints();
		}
	}
}

void testColisionsGreens(){
	// entre as bolas verdes
	int c, c2, i;

	for (c = 0; c < QNT_GREEN; c++){
		for (c2 = 0; c2 < QNT_GREEN; c2++){
			if (c != c2){
				if (! greens[c2].isFree){
					i = SDL_CollideBoundingCircle(greens[c2].x, greens[c2].y, greens[c2].h/2-1, greens[c].x, greens[c].y, greens[c2].h/2-1, 5);					
					if (i){
						if (greens[c].isFree){
							printf("Colisão de %d %d\n", c2, c);
							greens[c].isFree = 0;
						}
					}
				}
			}
		}

		i = SDL_CollideBoundingCircle(cursor.place.x, cursor.place.y, cursor.place.h/2-1, greens[c].x, greens[c].y, cursor.place.h/2-1, 5);
		if (i){
			greens[c].isFree = 0;
		}else{
			if (greens[c].isFree){
				greens[c].y += cursor.incY;
				if(((greens[c].x + greens[c].w) > screen->w) || (greens[c].x < 0)) 
				{
					cursor.incX = -cursor.incX;
					greens[c].x += cursor.incX;
				}
				if(((greens[c].y + greens[c].h) > screen->h) || (greens[c].y < 0))
				{
					cursor.incY = -cursor.incY;
					greens[c].y += cursor.incY;
				}
			}else{

			}
		}
	}
}


int main(int argc, char** argv)
{
	SDL_Event event;
	int quit = 0;
	
	ball green;
	green.image = IMG_Load("green.png");
	green.incX = green.incY = 0;
	green.isFree = 1;
	green.type = Green;
	green.x = 0;
	green.y = 0;
	green.w = 40;
	green.h = 40;
	green.isFree = 1;
	

	int c;

	for (c = 0; c < QNT_GREEN; c ++){
		greens[c].image = IMG_Load("green.png");
		greens[c].incX = greens[c].incY = 0;
		greens[c].isFree = 1;
		greens[c].type = Green;
		greens[c].x = 50*c;
		greens[c].y = 0;
		greens[c].w = 40;
		greens[c].h = 40;
		greens[c].isFree = 1;
	}

	for (c = 0; c < QNT_RED; c ++){
		reds[c].image = IMG_Load("red.png");
		reds[c].incX = reds[c].incY = 0;
		reds[c].isFree = 1;
		reds[c].type = Red;
		reds[c].x = 60*c;
		reds[c].y = 50;
		reds[c].w = 40;
		reds[c].h = 40;
		reds[c].isFree = 1;
	}

	for (c = 0; c < QNT_BLUE; c ++){
		blues[c].image = IMG_Load("blue.png");
		blues[c].incX = blues[c].incY = 0;
		blues[c].isFree = 1;
		blues[c].type = Red;
		blues[c].x = 60*c;
		blues[c].y = 100;
		blues[c].w = 40;
		blues[c].h = 40;
		blues[c].isFree = 1;
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
				if(event.key.keysym.sym == SDLK_d){
					cursor.place.x += cursor.incX;
				}
				if(event.key.keysym.sym == SDLK_a){
					cursor.place.x -= cursor.incX;
				}
			}
			else if (event.type == SDL_MOUSEMOTION){
				cursor.place.x = event.motion.x - cursor.place.h/2;
				cursor.place.y = event.motion.y - cursor.place.h/2;

				for (c = 0; c < QNT_GREEN; c++){
					if (! greens[c].isFree){
						greens[c].x += event.motion.xrel;
						greens[c].y += event.motion.yrel;
					}
				}
			}
		}

		if (lifes > 0){
			testColisionsGreens();
			testColisionsReds();
			testColisionsBlues();

			SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

			SDL_Rect rect;

			for (c = 0; c < QNT_GREEN; c++){
				rect.x = greens[c].x;
				rect.y = greens[c].y;
				SDL_BlitSurface(greens[c].image, NULL, screen, &rect);
			}

			for (c = 0; c < QNT_RED; c++){
				rect.x = reds[c].x;
				rect.y = reds[c].y;
				SDL_BlitSurface(reds[c].image, NULL, screen, &rect);
			}

			for (c = 0; c < QNT_BLUE; c++){
				rect.x = blues[c].x;
				rect.y = blues[c].y;
				SDL_BlitSurface(blues[c].image, NULL, screen, &rect);
			}

			SDL_BlitSurface(cursor.image, NULL, screen, &cursor.place);

			SDL_WM_SetCaption("Projeto Final - Jonathan Silva", "Projeto final");

			SDL_Flip(screen);
		}
	}

	SDL_Quit();

	return 0;
}