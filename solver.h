#ifndef SOLVER_H
#define SOLVER_H

int find_empty(Game *game);
int is_possible(Game *game, int position, int x);
void checkGrid(Game *game);
void auto_correct(Game *game);
int solver(Game *game);
void solve(Game *game);

#endif
