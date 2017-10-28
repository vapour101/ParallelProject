#include "backtrack.h"
#include <iostream>

using namespace std;

BackTrack::BoardSequence BackTrack::BoardSequence::NullSequence;

BackTrack::MoveSequence BackTrack::recurse(const Board& state)
{
	nodes++;

	vector<Move> legal = state.getLegalMoves();

	if (legal.empty())
		failed++;

	for (Move  move : legal)
	{
		Board privateState = state;
		privateState.executeMove(move);

		if (isInfeasible(privateState))
			continue;

		if ( privateState.isFinal() )
		{
			MoveSequence result;
			result.push_front(move);
			return result;
		}

		MoveSequence childResult = recurse(privateState);

		if ( !childResult.empty())
		{
			childResult.push_front(move);
			return childResult;
		}

		addInfeasible(privateState);
	}

	return MoveSequence();
}

ostream& operator<<(ostream& out, const BackTrack& solver)
{
	solver.printToStream(out);
	return out;
}

BackTrack::BackTrack(const Board& start) : initialBoard{start}
{
	checked = false;
	solvable = false;
	failed = 0;
	nodes = 0;
}

void BackTrack::start()
{
	chrono::high_resolution_clock::time_point start, stop;

	checked = true;

	start = chrono::high_resolution_clock::now();
	solution = recurse(initialBoard);
	stop = chrono::high_resolution_clock::now();

	duration = stop - start;

	solvable = !solution.empty();
}

BackTrack::BoardSequence BackTrack::getBoardSequence() const
{
	if ( !hasSolution() )
		return BoardSequence::NullSequence;

	return BoardSequence(*this);
}

BackTrack::MoveSequence BackTrack::getMoveSequence() const
{
	return solution;
}

BackTrack::SolutionTime BackTrack::getDuration() const
{
	if ( !checked )
		return SolutionTime::max();

	return duration;
}

int BackTrack::getInfeasibleCount() const
{
	return infeasibleBoards.size();
}

int BackTrack::getFailed() const
{
	return failed;
}

int BackTrack::getNodes() const
{
	return nodes;
}

bool BackTrack::hasSolution() const
{
	return solvable;
}

void BackTrack::printToStream(ostream& out) const
{
	out << initialBoard << endl;

	if ( !checked )
	{
		out << "Solution not started." << endl;
		return;
	}

	if (!hasSolution())
	{
		out << "No solution found." << endl;
		return;
	}

	out << "Solution found in " << getDuration() << endl;
	out << "Checked dead states: " << getFailed() << endl;
	out << "Infeasible states found: " << getInfeasibleCount() << endl;
	out << "Length: " << getMoveSequence().size() << " moves." << endl;
	out << getMoveSequence() << endl;
}

void BackTrack::printSequence(ostream& out) const
{
	if (!hasSolution())
		return;

	out << getBoardSequence() << endl;
}

void BackTrack::printTime(ostream& out) const
{
	out << "Time:\t\t";
	out << getDuration().count() / 1000000 << "ms" << endl;
}

void BackTrack::clear()
{
	checked = false;
	solvable = false;
	nodes = 0;
	solution.clear();
	infeasibleBoards.clear();
}

bool BackTrack::isInfeasible(const Board& check)
{
	string boardString = check.toString();
	return infeasibleBoards.count(boardString);
}

void BackTrack::addInfeasible(const Board& board)
{
	for (string boardString : board.toImageStrings())
		infeasibleBoards.insert(boardString);
}


ostream& operator<<(ostream& out, const BackTrack::SolutionTime& duration)
{
	out << duration.count() / 1000000 << "ms";
	return out;
}

ostream& operator<<(ostream& out, const BackTrack::BoardSequence& solution)
{
	if (solution.isNullSequence)
		return out;

	Board board = solution.initialBoard;
	out << board << endl;

	for (Move move : solution.sequence)
	{
		board.executeMove(move);
		out << board << endl;
	}

	return out;
}

BackTrack::BoardSequence::BoardSequence(const BackTrack& solver) : isNullSequence{false}, initialBoard{solver.initialBoard}, sequence{solver.solution}
{
}

BackTrack::BoardSequence::BoardSequence() : isNullSequence{true}
{
}

ostream& operator<<(ostream& out, const BackTrack::MoveSequence& sequence)
{
	out << "Move Sequence:" << endl;
	for (Move move : sequence)
	{
		out << "\t";
		printMove(out, move);
	}

	return out;
}
