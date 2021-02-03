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
    int state {0};
    Uint8 r {255};
    Uint8 g {255};
    Uint8 b {255};
    int value {0};
    SDL_Rect rectangle{};
} Case;

typedef struct Box
{
    int x{};
    int y{};
    int size{};
} Box;

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int selectedCase{-1};
    /*
      STATE
      0 : correct
      1 : wrong
    */
    int state{1};
    SDL_Rect board{};
    Box box{};
    Case cases[81]{};
} Game;


void sdl_error(const char *message);
void update_box(Game *game);
void update_cases_rectangle(Game *game, int smallLine, int bigLine, int caseSize);
void update_case_state(Game *game, int position, int state);
void number(Game *game, int position);
void update_case(Game *game, int position, int draw);
void update_board(Game *game);
void draw_main(Game *game);
int get_case_clicked(Game *game, int x, int y);
void on_click_left_event(Game *game);
void on_key_pressed_event(Game *game, char key);
void gui_main();
    
#endif
