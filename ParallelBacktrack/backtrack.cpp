#include "backtrack.h"
#include <omp.h>
#include <iostream>

using namespace std;

list<Move> backtrack::recurse(Board state)
{
	list<Move> result{};

	vector<Move> legal = state.getLegalMoves();

#pragma omp parallel for schedule(dynamic) shared(state) shared(result)
	for (int i = 0; i < legal.size(); i++)
	{
		Board privateState = state;
		Move move = legal.at(i);

		if ( !result.empty() )
			continue;

		privateState.executeMove(move);

		if ( privateState.isFinal() )
		{
			list<Move> temp{};
			temp.push_back(move);

			if ( result.empty() )
				result = temp;

			continue;
		}

		if (isInfeasible(privateState))
			continue;

		list<Move> childResult = recurse(privateState);

		if ( childResult.empty())
		{
			addInfeasible(privateState);
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

