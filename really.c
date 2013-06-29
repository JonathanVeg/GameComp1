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
	Purple
}; // tipos das bolas, verde (preciso pegar), azul (pega as verdes e me da pontos), púrpura (ferra tudo), a amarela, que segue o mouse, não entra aqui.

typedef enum typesOfBalls typesOfBall;

typedef struct {
    SDL_Surface *image; // onde será a imagem em si. 

    int incX, incY; // incrementos (positivos e/ou negativos)

    SDL_Rect place;

    usint isFree; // se não está grudada. Só server para as Green Balls

    typesOfBall type; // 0 = verde 1 = 
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



int main2(int argc, char const *argv[])
{
	int running = 1; //variavel para controlar se o loop principal deve continuar
	SDL_Surface* screen;
	SDL_Event event;

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("Erro ao inicializar a SDL: %s\n", SDL_GetError()); //observe que, no Windows, isto é redirecionado para stdout.txt
		return 1;
	}

	if((screen = SDL_SetVideoMode(640, 480, 16, SDL_DOUBLEBUF)) == NULL)
	{
		printf("Erro ao inicializar o vídeo: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption("Meu Programa Legal v1.0 - Pressione Espaco para prender/soltar o mouse e Esc para sair", "MeuProg");

	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				running = 0;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_SPACE)
				{
					SDL_WM_GrabInput(!SDL_WM_GrabInput(SDL_GRAB_QUERY));
				}
				else if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = 0;
				}
			}
		}
	}

	SDL_Quit();

	return 0;
}


#define N_PARAM 9

char* param_text[N_PARAM] = {"SDL_SWSURFACE", "SDL_HWSURFACE", "SDL_ASYNCBLIT", "SDL_ANYFORMAT", "SDL_HWPALETTE", "SDL_DOUBLEBUF", "SDL_FULLSCREEN", "SDL_RESIZABLE", "SDL_NOFRAME"};
int param_code[N_PARAM] = {SDL_SWSURFACE, SDL_HWSURFACE, SDL_ASYNCBLIT, SDL_ANYFORMAT, SDL_HWPALETTE, SDL_DOUBLEBUF, SDL_FULLSCREEN, SDL_RESIZABLE, SDL_NOFRAME};

int str_to_param(char* str)
{
	int i;

	for(i=0; i<N_PARAM; i++)
	{
		if(!strcmp(str, param_text[i])) return param_code[i];
	}

	return -1;
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
	green.place.x = 0;
	green.place.y = 0;
	green.place.w = 40;
	green.place.h = 40;
	green.isFree = 1;
	
	ball greens[5];

	int c;

	for (c = 0; c < QNT_GREEN; c ++){
		greens[c].image = IMG_Load("green.png");
		greens[c].incX = greens[c].incY = 0;
		greens[c].isFree = 1;
		greens[c].type = Green;
		greens[c].place.x = 50*c;
		greens[c].place.y = 0;
		greens[c].place.w = 40;
		greens[c].place.h = 40;
		greens[c].isFree = 1;
	}

	// ball green2;
	// green2.image = IMG_Load("green.png");
	// green2.incX = green2.incY = 0;
	// green2.isFree = 1;
	// green2.type = Green;
	// green2.place.x = 100;
	// green2.place.y = 0;
	// green2.place.w = 40;
	// green2.place.h = 40;
	// green2.isFree = 1;

	cursor.image = IMG_Load("red.png");
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
						greens[c].place.x += event.motion.xrel;
						greens[c].place.y += event.motion.yrel;
					}
				}
			}
		}

		int c2;
		int i;
		for (c = 0; c < QNT_GREEN; c++){
			for (c2 = 0; c2 < QNT_GREEN; c2++){
				if (c != c2){
					if (! greens[c2].isFree){
						i = SDL_CollideBoundingCircle(greens[c2].place.x, greens[c2].place.y, greens[c2].place.h/2-1, greens[c].place.x, greens[c].place.y, greens[c2].place.h/2-1, 5);					
						if (i){
							if (greens[c].isFree){
								printf("Colisão de %d %d\n", c2, c);
								greens[c].isFree = 0;
							}
						}
					}
				}
			}

			i = SDL_CollideBoundingCircle(cursor.place.x, cursor.place.y, cursor.place.h/2-1, greens[c].place.x, greens[c].place.y, cursor.place.h/2-1, 5);
			if (i){
				greens[c].isFree = 0;
			}else{
				if (greens[c].isFree){
					greens[c].place.y += cursor.incY;
					if(((greens[c].place.x + greens[c].place.w) > screen->w) || (greens[c].place.x < 0)) 
					{
						cursor.incX = -cursor.incX;
						greens[c].place.x += cursor.incX;
					}
					if(((greens[c].place.y + greens[c].place.h) > screen->h) || (greens[c].place.y < 0))
					{
						cursor.incY = -cursor.incY;
						greens[c].place.y += cursor.incY;
					}
				}else{

				}
			}
		}
		
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		
		for (c = 0; c < QNT_GREEN; c++){
			SDL_BlitSurface(greens[c].image, NULL, screen, &greens[c].place);
		}
		SDL_BlitSurface(cursor.image, NULL, screen, &cursor.place);

		SDL_WM_SetCaption("Projeto Final - Jonathan Silva", "Projeto final");

		SDL_Flip(screen);
	}

	SDL_Quit();

	return 0;
}