#include <iostream>
#include "SDL2/SDL.h"
#include "gui.h"

#define SELECTED_R 223
#define SELECTED_G 223
#define SELECTED_B 39

#define WRONG_R 255
#define WRONG_G 0
#define WRONG_B 0

#define CORRECT_R 0
#define CORRECT_G 255
#define CORRECT_B 0

#define DEFAULT 0
#define SELECTED 1
#define WRONG 2
#define CORRECT 3


void sdl_error(const char *message)
{
    SDL_Log("\n%s : %s\n",  message, SDL_GetError());
}

void initialise_cases(int x_start, int y_start, int bigLine, int smallLine,
		      int caseSize, struct Case cases[])
{
    int i = x_start+bigLine;
    int j = y_start+bigLine;
    int counter_x = 0;
    int counter_y = 0;
    int position = 0;

    while(counter_y < 9)
	{
	    while (counter_x < 9)
		{
		    cases[position].rectangle.x = i;
		    cases[position].rectangle.y = j;
		    cases[position].rectangle.h = caseSize;
		    cases[position].rectangle.w = caseSize;
		    position++;
		    counter_x++;
		    i+=caseSize;
		    if (counter_x%3 == 0)
			i+=bigLine;
		    else
			i+=smallLine;
		}
	    counter_x = 0;
	    i = x_start+bigLine;
	    counter_y++;
	    j+=caseSize;
	    if (counter_y%3 == 0)
		j+=bigLine;
	    else
		j+=smallLine;
	}
}

void update_case_color(Case cases[], int position)
{
    switch(cases[position].state)
	{
	case DEFAULT:
	    cases[position].r = 255;
	    cases[position].g = 255;
	    cases[position].b = 255;
	    break;
	case SELECTED:
	    cases[position].r = SELECTED_R;
	    cases[position].g = SELECTED_G;
	    cases[position].b = SELECTED_B;
	    break;
	case WRONG:
	    cases[position].r = WRONG_R;
	    cases[position].g = WRONG_G;
	    cases[position].b = WRONG_B;
	    break;
	case CORRECT:
	    cases[position].r = CORRECT_R;
	    cases[position].g = CORRECT_G;
	    cases[position].b = CORRECT_B;
	    break;
	}
}

void update_case(struct Window_main *window_main, Case cases[],
		 int position, int draw)
{
    update_case_color(cases, position);
    SDL_SetRenderDrawColor(window_main->renderer,
				   cases[position].r,
				   cases[position].g,
				   cases[position].b,
				   255);    
	    
    SDL_RenderFillRect(window_main->renderer, &cases[position].rectangle);
    
    // ADD DRAW NUMBER HERE

    if (draw)
	SDL_RenderPresent(window_main->renderer);
}

void draw_board(struct Window_main *window_main, Case cases[])
{
    // Background
    SDL_SetRenderDrawColor(window_main->renderer, 223, 223, 39, 255);
    SDL_RenderClear(window_main->renderer);

    // Get the size of the window
    int w;
    int h;
    SDL_GetRendererOutputSize(window_main->renderer, &w, &h);    

    // Set the width of the case separator
    // Depending on the size of the window
    int smallLine = 1+h/1000;
    int bigLine = 3+h/1000;
    int caseSize;
    int size;

    // Set the size of the cases, separations, and the board
    if (w < h*85/100)
	{
	    smallLine = 1+w/1000;
	    bigLine = 3+w/1000;
	    caseSize = (w*93/100 - 6*smallLine - 4*bigLine)/9;
	    size = 9*caseSize + 6*smallLine + 4*bigLine;
	}
    else
	{
	    smallLine = 1+h/1000;
	    bigLine = 3+h/1000;
	    caseSize = (h*80/100 - 6*smallLine - 4*bigLine)/9;
	    size = 9*caseSize + 6*smallLine + 4*bigLine;
	}

    // Black square behind the cases (for the lines between)
    SDL_SetRenderDrawColor(window_main->renderer, 0, 0, 0, 255);
    int x_start = w/2-size/2;
    int y_start = h/100;
    SDL_Rect rectangle;
    rectangle.x = x_start;
    rectangle.y = y_start;
    rectangle.w = size;
    rectangle.h = size;
    SDL_RenderFillRect(window_main->renderer, &rectangle);

    // Initialise cases (positions, size, color)
    initialise_cases(x_start, y_start, bigLine, smallLine, caseSize, cases);

    // Add the cases to the renderer
    for (int i = 0; i<81; i++)
	{
	    update_case(window_main, cases, i, 0);
	}    

    // Draw the renderer
    SDL_RenderPresent(window_main->renderer);
}

int get_case_clicked(Window_main *window_main, Case cases[], int x, int y)
{
    for (int i = 0; i<81; i++)	
	{
	    if (x > cases[i].rectangle.x &&
		x < cases[i].rectangle.x + cases[i].rectangle.w &&
		y > cases[i].rectangle.y &&
		y < cases[i].rectangle.y + cases[i].rectangle.h)
		{
		    return i;
		}	    
	}
    return -1;
}

void check_selected_case(Window_main *window_main, Case cases[])
{
    for (int i = 0; i<81; i++)
	{
	    if (cases[i].r == SELECTED_R &&
		cases[i].g == SELECTED_G &&
		cases[i].b == SELECTED_B)
		{
		    cases[i].r = 255;
		    cases[i].g = 255;
		    cases[i].b = 255;
		    update_case(window_main, cases, i, 1);
		}
	}
}

void on_key_pressed_event(Window_main *window_main, Case cases[], char key)
{
    if (key >= '0' && key <= '9')
    	{
	    int position = 0;
	    int searching = 1;
	    while (searching && position<81)
		{
		    if (cases[position].r == SELECTED_R &&
			cases[position].g == SELECTED_G &&
			cases[position].b == SELECTED_B)
			{
			    cases[position].value = key - '0';
			    cases[position].state = DEFAULT;
			    update_case(window_main, cases, position, 1);
			    searching = 0;
			}
		    position++;
		}
	}    
}

void on_click_left_event(Window_main *window_main, Case cases[])
{
    check_selected_case(window_main, cases);
    
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    int position = get_case_clicked(window_main, cases, x, y);
    if (position != -1)
	{
	    // Case selected
	    cases[position].state = SELECTED;
	    update_case(window_main, cases, position, 1);
	    // ask a number :
	    //    1. update cases[position].value
	    //    2. update case[position] color to white
	    //    3. draw case white
	    //    4. draw number
	    
	}
}

void gui()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
	sdl_error("SDL_Init failed");

    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer) != 0)
	sdl_error("SDL_CreateWindowAndRenderer");

    Case cases[81];
    
    Window_main window_main =
	{
	 .window = window,
	 .renderer = renderer,
	};

    SDL_SetWindowMinimumSize(window, 700, 500);

    draw_board(&window_main, cases);

    int program_executing = 1;
    while(program_executing != 0)
	{
	    SDL_Event event;

	    while(SDL_PollEvent(&event))
		{
		    switch(event.type)
			{
			case SDL_WINDOWEVENT:
			    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				draw_board(&window_main, cases);
			    break;
			case SDL_MOUSEBUTTONDOWN:
			    if (event.button.button == SDL_BUTTON_LEFT)
				on_click_left_event(&window_main, cases);
			    break;
			case SDL_KEYDOWN:
			    on_key_pressed_event(&window_main, cases, event.key.keysym.sym);
			    break;
			case SDL_QUIT:
			    program_executing = 0;
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

