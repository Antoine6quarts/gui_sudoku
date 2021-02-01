#include <iostream>
#include "SDL2/SDL.h"
#include "gui.h"

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

void update_case(struct Window_main *window_main, Case cases[], int position)
{
    SDL_SetRenderDrawColor(window_main->renderer,
				   cases[position].r,
				   cases[position].g,
				   cases[position].b,
				   255);
	    
    SDL_RenderFillRect(window_main->renderer, &cases[position].rectangle);
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
    if (h*85/100 > w)
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
	    update_case(window_main, cases, i);
	}    

    // Draw the renderer
    SDL_RenderPresent(window_main->renderer);
    
}

void gui()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
	sdl_error("SDL_Init failed");

    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer) != 0)
	sdl_error("SDL_CreateWindowAndRenderer");

    Window_main window_main =
	{
	 .window = window,
	 .renderer = renderer,
	};

    SDL_SetWindowMinimumSize(window, 700, 500);

    Case cases[81];

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

