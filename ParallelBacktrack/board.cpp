#include "board.h"
#include "cmath"

using namespace std;

Board::Board()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			Coord coord = make_pair(i, j);
			if (withinBounds(coord))
				pegs.insert(coord);
		}
}

bool Board::isFinal() const
{
	return pegs.size() == 1 && pegs.count(make_pair(3,3));
}

bool Board::isLegal(Move move) const
{
	if (!isOccupied(move.first))
		return false;

	Coord pos = getPosition(move);

	if (!withinBounds(pos))
		return false;

	if (isOccupied(pos))
		return false;

	if (!captures(move))
		return false;

	return true;
}

void Board::executeMove(Move move)
{
	if (!isLegal(move))
		return;

	pegs.erase(pegs.find(move.first));
	pegs.erase(pegs.find(getJumped(move)));

	pegs.insert(getPosition(move));
}

string Board::toString() const
{
	string out = "";

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			Coord coord = make_pair(i, j);
			if (withinBounds(coord))
			{
				if (isOccupied(coord))
					out += "o";
				else
					out += "+";
			}
		}

    return out;
}

int Board::calcHeuristic() const //TODO: Vincent check this
{
    int sum=0;
    for (const auto& coord: pegs){
        int exponent=max(abs(coord.first-3), abs(coord.second-3));
        sum += pow(2, exponent);
    }
    return sum;
}

bool Board::isInvalid() const //TODO: Vincent check this
{
    for (const auto& coord: pegs){
        for (int i=0; i<=3; i++){
            Move move=std::make_pair(coord, (Direction) i);
            if (isLegal(move)){
                return false;
            }
        }
    }
    return true;
}

Coord Board::getPosition(Move move) const
{
	Coord coords = move.first;

	switch(move.second) {
	case NORTH:
		coords.second += 2;
		break;
	case SOUTH:
		coords.second -= 2;
		break;
	case EAST:
		coords.first += 2;
		break;
	case WEST:
		coords.first -= 2;
		break;
	}

	return coords;
}

bool Board::withinBounds(Coord coords) const
{
	int x = coords.first;
	int y = coords.second;

	if (x > 3)
		x = 6 - x;

	if (y > 3)
		y = 6 - y;

	if ( x < 0 || y < 0 )
		return false;

	if ( x < 2 && y < 2 )
		return false;

	return true;
}

bool Board::isOccupied(Coord coords) const
{
	return pegs.count(coords);
}

bool Board::captures(Move move) const
{
	return isOccupied(getJumped(move));
}

Coord Board::getJumped(Move move) const
{
	Coord start = move.first;

	switch(move.second) {
	case NORTH:
		start.second += 1;
		break;
	case SOUTH:
		start.second -= 1;
	case EAST:
		start.first += 1;
		break;
	case WEST:
		start.first -= 1;
		break;
	}

	return start;
}

