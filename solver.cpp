#include "gui.h"
#include "solver.h"
#include "SDL2/SDL.h"
#include <iostream>

int find_empty(Game *game)
{
    for (int i = 0; i<81; i++)
	{
	    if(game->cases[i].value == 0)
		return i;
	}
    return -1;
}

int is_possible(Game *game, int position, int x)
{
    int i = position/9;
    int j = position%9;
    
    // Check if x is already in the line    
    for (int column = 0; column<9; column++)
	{
	    if (i*9+column != position &&
		game->cases[i*9+column].value == x)
		return 0;
	}

    // Check if x is already in the column
    for (int line = 0; line<9; line++)
	{
	    if (line*9+j != position &&
		game->cases[line*9+j].value == x)
		return 0;
	}

    // Check if x is already in the block
    int left = j-(j%3);
    int right = left+3;
    int top = i-(i%3);
    int bottom = top+3;
    for (int k = top; k<bottom; k++)	
	for (int l = left; l<right; l++)
	    if (k*9+l != position &&
		game->cases[k*9+l].value == x)
		return 0;

    // If possible, return true (1)
    return 1;    
}

void checkGrid(Game *game)
{
    int correct = 1;
    for (int i = 0; i<81; i++)
	{
	    if(!is_possible(game, i, game->cases[i].value))		
		{
		    correct= 0;
		    update_case_state(game, i, 2);
		}
	    else
		update_case_state(game, i, 0);
	}

    if(correct)
	{
	    for (int i = 0; i<81; i++)
		update_case_state(game, i, 3);
	    game->state = 0;
	}
    else
	game->state = 1;
}

void auto_correct(Game *game)
{
    checkGrid(game);    
    SDL_Delay(1000);
    if (game->state == 1)
	{
	    for (int i = 0; i<81; i++)
		{
		    if (game->cases[i].state == 2)
			{
			    game->cases[i].value = 0;
			    update_case_state(game, i, 0);
			}
		}	    
	    solve(game);
	}
}

int solver(Game *game)
{
    int i = find_empty(game);
    if (i == -1)
	return 1;
    
    for (int k = 1; k<10; k++)
	{
	    if (is_possible(game, i, k))
		{
		    game->cases[i].value = k;
		    update_case(game, i, 1);
		    //SDL_Delay(100);
		    if (solver(game))
			return 1;
		    game->cases[i].value = 0;
		    update_case(game, i, 1);
		}
	}
    return 0;
}

void solve(Game *game)
{
    int succes = solver(game);
    (void)succes;
    checkGrid(game);
}
