/// gcc main.c -o ex1 -Wall -lSDL -lSDL_image
/// para compilar


#define COLLECTIBLE_BALL_FILENAME "image.png"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define X_AXIS 0
#define Y_AXIS 1

#define MAX_BALLS 200

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct {
    SDL_Surface *image;
    SDL_Rect frame;
    float horizontalVelocity, verticallVelocity;
} Cursor; // a bolinha que segue o mouse

enum typesOfBalls{
    Green,
    Blue,
    Purple
}; // tipos das bolas, verde (preciso pegar), azul (pega as verdes e me da pontos), púrpura (ferra tudo), a amarela, que segue o mouse, não entra aqui.

typedef enum typesOfBalls typesOfBall;

typedef struct {
    SDL_Surface *image; // onde será a imagem em si. 

    int incX, incY; // incrementos (positivos e/ou negativos)

    int px, py;

    typesOfBall type; // 0 = verde 1 = 
} balls; // as bolas, todas, menos a amarela

void changeDirection(Cursor *ball, int axis) {

    switch (axis) {

        case X_AXIS:
        ball->horizontalVelocity = -ball->horizontalVelocity;
        break;

        case Y_AXIS:
        ball->verticallVelocity = -ball->verticallVelocity;
        break;

    }
}

void move(Cursor *ball) {

    ball->frame.x += ball->horizontalVelocity;
    ball->frame.y += ball->verticallVelocity;

    if (ball->frame.x >= (SCREEN_WIDTH - ball->frame.w) || ball->frame.x <= 0)
        changeDirection(ball, X_AXIS);
    if (ball->frame.y >= (SCREEN_HEIGHT - ball->frame.h) || ball->frame.y <= 0)
        changeDirection(ball, Y_AXIS);
}

SDL_Rect SDL_RectMake(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return rect;
}

int surfaceWithFrameDidHitWallAtAxis(SDL_Rect *frame, int axis) {

    Sint16 x = frame->x;
    Sint16 y = frame->y;

    Uint16 w = frame->w;
    Uint16 h = frame->h;

    switch (axis) {

        case X_AXIS:
        return (x >= SCREEN_WIDTH - w);

        case Y_AXIS:
        return (y >= SCREEN_HEIGHT - h);

        default:
        return 0;
    }
}

void moveCursor(Cursor *cursor, SDL_Surface *screen) {

    Sint16 *x = &cursor->frame.x;
    Sint16 *y = &cursor->frame.y;

    Uint16 cursorWidth = cursor->frame.w;
    Uint16 cursorHeight = cursor->frame.h;

    SDL_GetMouseState((int *)x, (int *)y);

    cursor->frame.w = cursorWidth;
    cursor->frame.h = cursorHeight;

    if (surfaceWithFrameDidHitWallAtAxis(&cursor->frame, X_AXIS))
        *x = SCREEN_WIDTH - cursorWidth;
    if (surfaceWithFrameDidHitWallAtAxis(&cursor->frame, Y_AXIS))
        *y = SCREEN_HEIGHT - cursorHeight;

    SDL_BlitSurface(cursor->image, NULL, screen, &cursor->frame);
}

int main (int argc, char **argv) {

    SDL_Surface *screen;
    SDL_Event event;
    Uint32 backgroundColour;
    int quit = 0;

    Cursor cursor;

    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
        puts(SDL_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
    backgroundColour = SDL_MapRGB(screen->format, 255, 255, 255); // White colour
    SDL_ShowCursor(SDL_DISABLE);

    cursor.frame = SDL_RectMake(0, 0, 40, 40);
    if (!(cursor.image = IMG_Load(COLLECTIBLE_BALL_FILENAME))) {
        printf("SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                quit = 1;

            if (event.key.keysym.sym == SDLK_ESCAPE) // esc tbm sai
               exit(0);
           if (event.key.keysym.sym == SDLK_RETURN) // ENTER tbm sai (para teste)
               exit(0);

       }

       SDL_FillRect(screen, NULL, backgroundColour);

       moveCursor(&cursor, screen);

       SDL_Flip(screen);
   }

   SDL_Quit();
   return 0;
}



//#include <sdl.h>
//#include <stdlib.h>

//#define SCREEN_W 600 //tamanho da janela que sera criada
//#define SCREEN_H 400

//SDL_Surface *screen;
//SDL_Surface *image;

//typedef unsigned short int usint;

//void putpixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
//{
//    int offset = y * surface->w + x;
//    Uint32 color = SDL_MapRGB(surface->format, r, g, b);

//    if(surface->format->BytesPerPixel == 1)
//        *( ((Uint8*)surface->pixels) + offset) = color;
//    else if(surface->format->BytesPerPixel == 2)
//        *( ((Uint16*)surface->pixels) + offset) = color;
//    else
//        *( ((Uint32*)surface->pixels) + offset) = color;
//}

//void processEvents(){
//    SDL_Event event;

//    while (SDL_PollEvent(&event) != 0)
//    {
//        switch (event.type)
//        {
//        case SDL_KEYDOWN:
//            if (event.key.keysym.sym == SDLK_ESCAPE) // esc tbm sai
//                exit(0);
//            if (event.key.keysym.sym == SDLK_RETURN) // ENTER tbm sai (para teste)
//                exit(0);
//            break;
//        case SDL_QUIT:
//            exit(0); //Fechamos a apliação
//            break;
//        }
//    }
//}

//void setupScreen(int width, int height, int bpp, usint fullscreen){
//    //Inicializamos o subsistema de video.
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) // se n der certo.
//        exit(1);

//    //Definimos as flags.
//    int flags = SDL_SWSURFACE;
//    if (fullscreen)
//        flags = flags | SDL_FULLSCREEN;

//    //Tentamos criar a janela
//    screen = SDL_SetVideoMode(width, height, bpp, flags);

//    SDL_WM_SetCaption("Programinha do Jonathan =) ", NULL);

//    if (screen == NULL)
//        exit(1);
//}

//int main(int argc, char** argv) //funcao de entrada
//{
//    int quit = 0; //devemos encerrar o programa?

//    setupScreen(SCREEN_W, SCREEN_H, 16, SDL_SWSURFACE); //criar uma janela 640x480x16bits

//    int x = 0;
//    SDL_Delay(x); // da uma pausa de x milisegundos. deixei aqui para n esquecer o evento

//    image = SDL_LoadBMP("image.bmp");

//    SDL_Rect location = {50,50,100,100};

//    SDL_BlitSurface(image, NULL, screen, &location);

//    if(SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

//    for (x = 200; x < 300; x ++){
//        putpixel(screen, 100, x, 255, 0, 0); //pinta um pixel vermelho em (100, 200)
//    }

//    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

//    while(!quit) //rodar enquanto nao for para encerrar :)
//    {
//        processEvents();

//        SDL_Flip(screen); //atualizar a tela
//    }

//    //    SDL_FreeSurface(image);
//    //    SDL_FreeSurface(screen);

//    SDL_Quit(); //encerrar a SDL

//    return 0;
//}
