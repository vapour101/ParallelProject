#ifndef BOARD_H
#define BOARD_H

#include <unordered_set>
#include <utility>
#include <boost/functional/hash.hpp>
#include <string>

struct PairHash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
	std::size_t seed = 0;
	boost::hash_combine(seed, std::hash<T>()(x.first));
	boost::hash_combine(seed, std::hash<T>()(x.second));
	return  seed;
  }
};

enum Direction {
    NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3
};

typedef std::pair<int, int> Coord;
typedef std::pair<Coord, Direction> Move;

class Board
{
public:
	Board();
	bool isFinal() const;
	bool isLegal(Move) const;
	void executeMove(Move);
	std::string toString() const;

private:
	std::unordered_set<Coord, PairHash> pegs;
	Coord getPosition(Move) const;
	bool withinBounds(Coord) const;
	bool isOccupied(Coord) const;
	bool captures(Move) const;
	Coord getJumped(Move) const;

	friend struct BoardHash;
};


struct BoardHash {
public:
	std::size_t operator()(const Board& board) const
	{
		return std::hash<std::string>()(board.toString());
	}
};


#endif // BOARD_H
