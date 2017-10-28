#ifndef BACKTRACK_H
#define BACKTRACK_H

#include <list>
#include <unordered_set>
#include <ostream>
#include <chrono>

#include "board.h"

class BackTrack
{
public:
	class BoardSequence; //forward declaration
	typedef std::chrono::nanoseconds SolutionTime;
	typedef std::list<Move> MoveSequence;

	BackTrack(const Board& start);

	void start();
	void clear();

	bool hasSolution() const;

	BoardSequence getBoardSequence() const;
	MoveSequence getMoveSequence() const;
	SolutionTime getDuration() const;
	int getInfeasibleCount() const;
	int getFailed() const;
	int getNodes() const;


	void printSequence(std::ostream& out) const;
	void printTime(std::ostream& out) const;

private:
	void printToStream(std::ostream& out) const;
	bool isInfeasible(const Board& check);
	void addInfeasible(const Board& board);
	MoveSequence recurse(const Board& state);

	const Board initialBoard;
	int failed;
	int nodes;

	bool checked;
	bool solvable;
	MoveSequence solution;

	std::unordered_set<std::string> infeasibleBoards;
	SolutionTime duration;

	friend std::ostream& operator<<(std::ostream& out, const BackTrack& solver);

public:
	class BoardSequence {
	public:
		BoardSequence(const BackTrack& solver);

		const bool isNullSequence;

		static BoardSequence NullSequence;

		friend std::ostream& operator<<(std::ostream& out, const BoardSequence& sequence);

	private:
		BoardSequence();
		const Board initialBoard;
		const std::list<Move> sequence;
	};
};

std::ostream& operator<<(std::ostream& out, const BackTrack::SolutionTime& duration);
std::ostream& operator<<(std::ostream& out, const BackTrack::MoveSequence& sequence);


#endif // BACKTRACK_H
