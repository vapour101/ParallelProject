#include "backtrack.h"
#include <omp.h>
#include <iostream>

using namespace std;

void printDirection(Direction direction) {
	switch(direction) {
	case NORTH:
		cout << "NORTH";
		break;
	case SOUTH:
		cout << "SOUTH";
		break;
	case EAST:
		cout << "EAST";
		break;
	case WEST:
		cout << "WEST";
		break;
	}
}

void printCoord(Coord coord) {

		cout << "(" << coord.first << ", " << coord.second << ")";

}

void printMove(Move move)
{
#pragma omp critical
	{
		printCoord(move.first);
		cout << " - ";
		printDirection(move.second);
		cout << endl;
	}

}

list<Move> backtrack::recurse(Board state)
{
	list<Move> result{};

	vector<Move> legal = state.getLegalMoves();

#pragma omp parallel for schedule(dynamic) private(state) shared(result)
	for (int i = 0; i < legal.size(); i++)
	{
		Move move = legal.at(i);
		printMove(move);

		if ( !result.empty() )
			continue;

		state.executeMove(move);

		if ( state.isFinal() )
		{
			list<Move> temp{};
			temp.push_back(move);

			if ( result.empty() )
				result = temp;

			continue;
		}

		if (isInfeasible(state))
			continue;

		list<Move> childResult = recurse(state);

		if ( childResult.empty())
		{
			addInfeasible(state);
			continue;
		}

		childResult.push_front(move);

		if ( result.empty() )
			result = childResult;
	}

	return result;
}

backtrack::backtrack(Board start, int depth)
{
	initialBoard = start;
	threadingDepth = depth;

	checked = false;
	solvable = false;
}

void backtrack::start()
{
	omp_set_nested(true);
	omp_set_max_active_levels(threadingDepth);

	checked = true;

	solution = recurse(initialBoard);

	solvable = !solution.empty();
}

std::list<Move> backtrack::getSolution()
{
	if ( !checked )
		start();

	return solution;
}

bool backtrack::hasSolution()
{
	if ( !checked )
		start();

	return solvable;
}

bool backtrack::isInfeasible(Board check)
{
	size_t hash = BoardHash()(check);
	return infeasibleBoards.count(hash);
}


void backtrack::addInfeasible(Board board)
{
	size_t hash = BoardHash()(board);
	#pragma omp critical
	{
	infeasibleBoards.insert(hash);
	}
}

