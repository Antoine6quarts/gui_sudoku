#ifndef GUI_H
#define GUI_H
#include "SDL2/SDL.h"

typedef struct Case
{
    /*
      STATE
      0 : default    white    : 255, 255, 255
      1 : selected   yellow   : 223, 223, 039
      2 : wrong      red      : 255, 000, 000
      3 : correct    green    : 000, 255, 000
     */
    int state = 0;
    Uint8 r {255};
    Uint8 g {255};
    Uint8 b {255};
    int value {0};
    SDL_Rect rectangle;
} Case;


typedef struct Window_main
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Window_main;


void sdl_error(const char *message);
void initialise_cases(int x_start, int y_start, int bigLine, int smallLine,
		      int caseSize, struct Case cases[]);
void update_case_color(Case cases[], int position);
void update_case(struct Window_main *window_main, Case cases[],
		 int position, int draw);
void draw_board(struct Window_main *window_main, Case cases[]);
int get_case_clicked(Window_main *window_main, Case cases[], int x, int y);
void check_selected_case(Window_main *window_main, Case cases[]);
void on_key_pressed_event(Window_main *window_main, Case cases[], char key);
void on_click_left_event(Window_main *window_main, Case cases[]);
void gui();

#endif
