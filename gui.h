#ifndef GUI_H
#define GUI_H
#include "SDL2/SDL.h"

typedef struct Window_main
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Window_main;

typedef struct Case
{
    /*
      0 : white              : 255, 255, 255
      1 : yellow (selected)  : 223, 223, 039
      2 : red (wrong)        : 255, 000, 000
      3 : green (correct)    : 000, 255, 000
     */
    Uint8 r {255};
    Uint8 g {255};
    Uint8 b {255};
    int value {0};
    SDL_Rect rectangle;
} Case;


void sdl_error(const char *message);
void initialise_cases(int x_start, int y_start, int bigLine, int smallLine,
		      int caseSize, struct Case cases[]);
void update_case(struct Window_main *window_main, Case cases[], int position);
void draw_board(struct Window_main *window_main, Case cases[]);
void gui();

#endif
