#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>

#define SCREEN_W 600 //tamanho da janela que sera criada
#define SCREEN_H 400

SDL_Surface *screen;
SDL_Surface *image;

typedef unsigned short int usint;

typedef struct {
  SDL_Surface *image;
  SDL_Rect frame;
  float horizontalVelocity, verticallVelocity;
} Cursor; // a bolinha que segue o mouse

Cursor cursor;

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

    typesOfBall type; // 0 = verde 1 = 
} ball; // as bolas, todas, menos a amarela

void putpixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    int offset = y * surface->w + x;
    Uint32 color = SDL_MapRGB(surface->format, r, g, b);

    if(surface->format->BytesPerPixel == 1)
        *( ((Uint8*)surface->pixels) + offset) = color;
    else if(surface->format->BytesPerPixel == 2)
        *( ((Uint16*)surface->pixels) + offset) = color;
    else
        *( ((Uint32*)surface->pixels) + offset) = color;
}

void processEvents(){
 SDL_Event event;

 while (SDL_PollEvent(&event) != 0)
 {
   switch (event.type)
   {
     case SDL_KEYDOWN:
           if (event.key.keysym.sym == SDLK_ESCAPE) // esc tbm sai
             exit(0);
           if (event.key.keysym.sym == SDLK_RETURN) // ENTER tbm sai (para teste)
             exit(0);
            if (event.key.keysym.sym == SDLK_w){
                cursor.frame.h += 5;

                printf("%d\n", cursor.frame.h);
            }

         break;
         case SDL_QUIT:
           exit(0); //Fechamos a apliação
           break;
       }
   }
}

void setupScreen(int width, int height, int bpp, usint fullscreen){
   //Inicializamos o subsistema de video.
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) // se n der certo.
     exit(1);

   //Definimos as flags.
 int flags = SDL_SWSURFACE;
 if (fullscreen)
     flags = flags | SDL_FULLSCREEN;

   //Tentamos criar a janela
 screen = SDL_SetVideoMode(width, height, bpp, flags);

 SDL_WM_SetCaption("Programinha do Jonathan =) ", NULL);

 if (screen == NULL)
     exit(1);
}

int main(int argc, char** argv) //funcao de entrada
{
   int quit = 0; //devemos encerrar o programa?

   setupScreen(SCREEN_W, SCREEN_H, 16, SDL_SWSURFACE); //criar uma janela 640x480x16bits

   Uint32 background = SDL_MapRGB(screen->format, 0, 0, 255); // seta a cor do fundo.

   SDL_FillRect(screen, NULL, background); // pinta um rect do tamanho total da tela, na cor de fundo designada.

   image = IMG_Load("image.png");

   cursor.frame.w = 100;
   cursor.frame.h = 100;
   cursor.frame.x = 100;
   cursor.frame.y = 100;

   cursor.image = image;

   while(!quit)
   {
        processEvents();

       SDL_BlitSurface(cursor.image, NULL, screen, &cursor.frame); // prende a bolinha na posição certa

       SDL_Flip(screen); //atualizar a tela
   }

   SDL_Quit(); //encerrar a SDL

   return 0;
}
