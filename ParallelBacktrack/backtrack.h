#ifndef BACKTRACK_H
#define BACKTRACK_H

#include <list>
#include <unordered_set>
#include <ostream>
#include <chrono>

#include "board.h"

class backtrack
{
public:
	backtrack(const Board& start);

	void start();
	std::list<Move> getSolution();
	std::chrono::nanoseconds getDuration();
	int getInfeasibleCount() const;
	int getFailed() const;
	bool hasSolution();

	void print(std::ostream& out);
	void printSequence(std::ostream& out);
	void printTime(std::ostream& out);

	void setParallel(bool parallel);
	bool isParallel() const;
	void clear();

private:
	bool isInfeasible(const Board& check);
	void addInfeasible(const Board& board);
	std::list<Move> recurse(const Board& state);

	const Board initialBoard;
	int failed;

	bool parallelise;
	bool checked;
	bool solvable;
	std::list<Move> solution;

	std::unordered_set<std::size_t> infeasibleBoards;
	std::chrono::nanoseconds duration;
};

#endif // BACKTRACK_H
