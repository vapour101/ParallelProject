#include "backtrack.h"
#include <iostream>

using namespace std;

list<Move> backtrack::recurse(const Board& state)
{
	++nodes;
	list<Move> result;

	vector<Move> legal = state.getLegalMoves();

	if (legal.empty())
		++failed;

	for (int i = 0; i < legal.size(); i++)
	{
		Board privateState = state;
		Move move = legal.at(i);

		if ( !result.empty() ){
			continue;
		}


		privateState.executeMove(move);

		if ( privateState.isFinal() )
		{
			list<Move> temp{};
			temp.push_back(move);

			if ( result.empty() )
				result = temp;
			continue;
		}

		if (isInfeasible(privateState)){
			continue;
		}

		list<Move> childResult = recurse(privateState);

		if ( childResult.empty())
		{
			addInfeasible(privateState);
			continue;
		}

		childResult.push_front(move);

		if ( result.empty() ){
			result = childResult;
		}
	}

	return result;
}

backtrack::backtrack(const Board& start) : initialBoard{start}
{
	parallelise = false;
	checked = false;
	solvable = false;
	failed = 0;
	nodes = 0;
}

void backtrack::start()
{
	chrono::high_resolution_clock::time_point start, stop;

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

int backtrack::getInfeasibleCount() const
{
	return infeasibleBoards.size();
}

int backtrack::getFailed() const
{
	return failed;
}

int backtrack::getNodes() const
{
	return nodes;
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
	out << "Solution method: " << (isParallel() ? "Parallel" : "Sequential") << endl;
	out << "Checked dead states: " << getFailed() << endl;
	out << "Infeasible states found: " << getInfeasibleCount() << endl;
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

void backtrack::printTime(ostream& out)
{
	out << (isParallel() ? "Parallel Time:\t\t" : "Sequential Time:\t");
	out << getDuration().count() / 1000000 << "ms" << endl;
}

void backtrack::setParallel(bool parallel)
{
	parallelise = parallel;
}

bool backtrack::isParallel() const
{
	return parallelise;
}

void backtrack::clear()
{
	checked = false;
	solvable = false;
	nodes = 0;
	solution.clear();
	infeasibleBoards.clear();
}

bool backtrack::isInfeasible(const Board& check)
{
	size_t hash = BoardHash()(check);
	return infeasibleBoards.count(hash);
}

void backtrack::addInfeasible(const Board& board)
{
	size_t hash = BoardHash()(board);

	infeasibleBoards.insert(hash);
}

