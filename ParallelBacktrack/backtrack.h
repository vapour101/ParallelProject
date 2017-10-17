#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "board.h"
#include <list>
#include <unordered_set>

class backtrack
{
public:
	backtrack(Board start, int depth);

	void start();
	std::list<Move> getSolution();
	bool hasSolution();

private:
	bool isInfeasible(Board check);
	void addInfeasible(Board board);
	std::list<Move> recurse(Board state);

	Board initialBoard;
	int threadingDepth;

	bool checked;
	bool solvable;
	std::list<Move> solution;

	std::unordered_set<std::size_t> infeasibleBoards;
};

#endif // BACKTRACK_H
