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
	chrono::high_resolution_clock::time_point start, stop;

	omp_set_nested(true);
	omp_set_max_active_levels(threadingDepth);

	checked = true;

	start = chrono::high_resolution_clock::now();
	solution = recurse(initialBoard);
	stop = chrono::high_resolution_clock::now();

	duration = stop - start;

	solvable = !solution.empty();
}

list<Move> backtrack::getSolution()
{
	if ( !checked )
		start();

	return solution;
}

chrono::nanoseconds backtrack::getDuration()
{
	if (!checked)
		start();

	return duration;
}

bool backtrack::hasSolution()
{
	if ( !checked )
		start();

	return solvable;
}

void backtrack::print(ostream& out)
{
	out << initialBoard.toString() << endl;

	if (!hasSolution())
	{
		out << "No solution found." << endl;
		return;
	}

	out << "Solution found in " << getDuration().count() / 1000000 << "ms." << endl;
	out << "Length: " << getSolution().size() << " moves." << endl;
	out << "Solution:" << endl;

	for (Move move : getSolution())
	{
		out << "\t";
		printMove(out, move);
		out << endl;
	}
}

void backtrack::printSequence(ostream& out)
{
	if (!hasSolution())
		return;

	Board board = initialBoard;

	out << board.toString() << endl;

	for (Move move : getSolution())
	{
		board.executeMove(move);
		out << board.toString() << endl;
	}
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

