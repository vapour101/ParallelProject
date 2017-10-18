#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "board.h"
#include <list>
#include <unordered_set>
#include <ostream>
#include <chrono>

class backtrack
{
public:
	backtrack(Board start, int depth);

	void start();
	std::list<Move> getSolution();
	std::chrono::nanoseconds getDuration();
	bool hasSolution();
	void print(std::ostream& out);
	void printSequence(std::ostream& out);

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
	std::chrono::nanoseconds duration;
};

#endif // BACKTRACK_H
