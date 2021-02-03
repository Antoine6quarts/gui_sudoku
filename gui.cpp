#include <iostream>
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include "gui.h"
#include "solver.h"

#define BACKGROUND_R 0xED
#define BACKGROUND_G 0x92
#define BACKGROUND_B 0x0

#define SELECTED_R 0xDF
#define SELECTED_G 0xDF
#define SELECTED_B 0x27

#define WRONG_R 0xFF
#define WRONG_G 0x0
#define WRONG_B 0x0

#define CORRECT_R 0x0
#define CORRECT_G 0xFF
#define CORRECT_B 0x0

#define DEFAULT 0
#define SELECTED 1
#define WRONG 2
#define CORRECT 3


void sdl_error(const char *message)
{
    SDL_Log("\n%s : %s\n",  message, SDL_GetError());
}

void update_box(Game *game)
{
    int w;
    int h;
    SDL_GetRendererOutputSize(game->renderer, &w, &h);

    if (w<h)
	{
	    game->box.size = w;
	    game->box.x = 0;
	    game->box.y = 0;
	}
    else
	{
	    game->box.size = h;
	    game->box.x = w/2-game->box.size/2;
	    game->box.y = 0;
	}    
}

void update_cases_rectangle(Game *game, int smallLine, int bigLine, int caseSize)
{    
    int i = game->board.x + bigLine;
    int j = game->board.y + bigLine;
    int counter_x{0};
    int counter_y{0};
    int position{0};

    while(counter_y < 9)
	{
	    while (counter_x < 9)
		{
		    game->cases[position].rectangle.x = i;
		    game->cases[position].rectangle.y = j;
		    game->cases[position].rectangle.h = caseSize;
		    game->cases[position].rectangle.w = caseSize;
		    position++;
		    counter_x++;
		    i+=caseSize;
		    if (counter_x%3 == 0)
			i+=bigLine;
		    else
			i+=smallLine;
		}
	    counter_x = 0;
	    i = game->board.x+bigLine;
	    counter_y++;
	    j+=caseSize;
	    if (counter_y%3 == 0)
		j+=bigLine;
	    else
		j+=smallLine;
	}
}

void update_case_state(Game *game, int position, int state)
{
    game->cases[position].state = state;
    if (state == SELECTED)
	game->selectedCase = position;
    if (state == DEFAULT)
	game->selectedCase = -1;
    
    switch(game->cases[position].state)
	{
	case DEFAULT:
	    game->cases[position].r = 255;
	    game->cases[position].g = 255;
	    game->cases[position].b = 255;
	    break;
	case SELECTED:
	    game->cases[position].r = SELECTED_R;
	    game->cases[position].g = SELECTED_G;
	    game->cases[position].b = SELECTED_B;
	    break;
	case WRONG:
	    game->cases[position].r = WRONG_R;
	    game->cases[position].g = WRONG_G;
	    game->cases[position].b = WRONG_B;
	    break;
	case CORRECT:
	    game->cases[position].r = CORRECT_R;
	    game->cases[position].g = CORRECT_G;
	    game->cases[position].b = CORRECT_B;
	    break;
	}
    update_case(game, position, 1);
}

void number(Game *game, int position)
{
    if (game->cases[position].value != 0)
	{	    
	    TTF_Init();
	    
	    TTF_Font *font = TTF_OpenFont("myfont.ttf", game->cases[0].rectangle.w*5);
	    if (font == NULL)
		{
		    printf("TTF_Font: %s\n", TTF_GetError());
		    exit(2);
		}
	    SDL_Color color = {0, 0, 0, 255};
	    char number[1+sizeof(char)];
	    std::sprintf(number, "%d", game->cases[position].value);
	    SDL_Surface *surface = TTF_RenderText_Blended(font, number, color);
	    TTF_CloseFont(font);
	    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	    SDL_FreeSurface(surface);

	    SDL_RenderCopy(game->renderer, texture, NULL, &game->cases[position].rectangle);
	    //SDL_RenderPresent(game->renderer);
    
	    SDL_DestroyTexture(texture);

	    TTF_Quit();
	}
    else
	{
	    game->cases[position].value = 0;
	    SDL_SetRenderDrawColor(game->renderer,
				   game->cases[position].r,
				   game->cases[position].g,
				   game->cases[position].b,
				   255);
	    SDL_RenderFillRect(game->renderer, &game->cases[position].rectangle);
	    //SDL_RenderPresent(game->renderer);    
	}   
}

void update_case(Game *game, int position, int draw)
{    
    SDL_SetRenderDrawColor(game->renderer,
				   game->cases[position].r,
				   game->cases[position].g,
				   game->cases[position].b,
				   255);
	    
    SDL_RenderFillRect(game->renderer, &game->cases[position].rectangle);

    number(game, position);
    
    if (draw)
	SDL_RenderPresent(game->renderer);    
}

void update_board(Game *game)
{
    int smallLine = 1+game->box.size/1000;
    int bigLine = smallLine*2;
    
    int caseSize = (game->box.size*80/100 - 6*smallLine - 4*bigLine)/9;
    int size = 9*caseSize + 6*smallLine + 4*bigLine;

    game->board.x = game->box.x + game->box.size/2 - size/2;
    game->board.y = game->box.size/100;
    game->board.w = size;
    game->board.h = size;

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &(game->board));

    update_cases_rectangle(game, smallLine, bigLine, caseSize);
    for (int i = 0; i<81; i++)
	{	    
	    update_case(game, i, 0);
	}
}

void draw_main(Game *game)
{
    // Background
    SDL_SetRenderDrawColor(game->renderer,
			   BACKGROUND_R,
			   BACKGROUND_G,
			   BACKGROUND_B,
			   0);
    SDL_RenderClear(game->renderer);

    update_box(game);

    // TEST BOX (REMOVE)
    /*
    SDL_Rect rect;
    rect.x = game->box.x;
    rect.y = game->box.y;
    rect.w = game->box.size;
    rect.h = game->box.size;    
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &rect);
    */
    // END TEST BOX

    update_board(game);    
    
    // Draw the renderer
    SDL_RenderPresent(game->renderer);
}


int get_case_clicked(Game *game, int x, int y)
{
    for (int i = 0; i<81; i++)
	{
	    if (x > game->cases[i].rectangle.x &&
		x < game->cases[i].rectangle.x + game->cases[i].rectangle.w &&
		y > game->cases[i].rectangle.y &&
		y < game->cases[i].rectangle.y + game->cases[i].rectangle.h)
		{
		    return i;
		}	    
	}
    return -1;
}

void on_click_left_event(Game *game)
{
    if (game->selectedCase != -1)	
	update_case_state(game, game->selectedCase, 0);	

    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    int position = get_case_clicked(game, x, y);
    if (position != -1)
	{
	    update_case_state(game, position, SELECTED);
	}
}

void on_key_pressed_event(Game *game, char key)
{
    if (key == 's')
	solve(game);
    if (key == 'c')
	checkGrid(game);
    if (key == 'a')
	auto_correct(game);
    if (key >= '0' && key <= '9' && game->selectedCase != -1)
    	{
	    game->cases[game->selectedCase].value = key - '0';	    
	    update_case_state(game, game->selectedCase, DEFAULT);
	    if(game->state == 0)
		{
		    game->state = 1;
		    for(int i = 0; i<81; i++)
			update_case_state(game, i, DEFAULT);
		}
	}
}

void gui_main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
	sdl_error("SDL_Init failed\n");

    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer) != 0)
	sdl_error("SDL_CreateWindowAndRenderer failed\n");

    SDL_SetWindowMinimumSize(window, 700, 500);

    // Initialise game
    Game game =
	{
	 .window = window,
	 .renderer = renderer,	 
	 .selectedCase{-1},
	 .state{1},
	 .board{},
	 .box{},
	 .cases{}
	};

    draw_main(&game);

    SDL_Event event;
    
    int running = 1;
    while(running)
	{
	    SDL_Delay(70);
	    while(SDL_PollEvent(&event))
		{
		    switch(event.type)
			{
			case SDL_WINDOWEVENT:
			    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				draw_main(&game);
			    break;
			case SDL_MOUSEBUTTONDOWN:
			    if (event.button.button == SDL_BUTTON_LEFT)
				on_click_left_event(&game);
			    break;
			case SDL_KEYDOWN:
			    on_key_pressed_event(&game, event.key.keysym.sym);
			    break;
			case SDL_QUIT:
			    running = 0;
			    break;
			default:
			    break;
			}
		}
	}
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
