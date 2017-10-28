#include "board.h"
#include "cmath"
#include <vector>
#include <algorithm>

using namespace std;

ostream& operator<<(ostream& out, const Board& board)
{
	out << board.toHumanString();
	return out;
}

Board::Board()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
		{
			if (i == 3 && j == 3)
				continue;

			Coord coord = make_pair(i, j);
			if (withinBounds(coord))
				pegs.insert(coord);
		}
}

Board::Board(const Board& other)
{
	for (Coord peg : other.pegs) {
		pegs.insert(peg);
	}
}

Board::Board(const string& input)
{
	char temp;
	istringstream inStream{input};

	for (int j = 6; j >= 0; j--)
	{
		for (int i = 0; i < 7; i++)
		{
			Coord pos = make_pair(i, j);
			inStream >> temp;

			if (withinBounds(pos))
			{
				if (temp == 'o')
					pegs.insert(pos);
				else if (temp != '+')
					throw "Parse error";
			}
			else {
				if (temp != '.')
					throw "Parse error";
			}
		}
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

	for (int j = 6; j >= 0; j--)
	{
		for (int i = 0; i < 7; i++)
		{
			Coord coord = make_pair(i, j);
			if (withinBounds(coord))
			{
				if (isOccupied(coord))
					out += "o";
				else
					out += "+";
			}
			else
				out += ".";
		}
	}

	return out;
}

vector<string> Board::toImageStrings() const
{
	vector<string> out = {"", "", "", "", "", "", "", ""};

	for (int j = 6; j >= 0; j--)
	{
		for (int i = 0; i < 7; i++)
		{
			Coord coord = make_pair(i, j);

			for (int k = 0; k < 8; k++)
			{
				Coord transCoord = transformCoord(coord, k);

				if (withinBounds(transCoord))
				{
					if (isOccupied(transCoord))
						out[k] += "o";
					else
						out[k] += "+";
				}
				else
					out[k] += ".";
			}
		}
	}

	return out;
}

bool Board::isInvalid() const
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

std::vector<Move> Board::getLegalMoves() const
{
	std::vector<Move> res{};

	for (Coord peg : pegs) {
		for (int i = 0; i < 4; ++i)
		{
			Move move=std::make_pair(peg, (Direction) i);

			if (isLegal(move)){
				res.push_back(move);
			}
		}
	}

	return res;
}

std::vector<size_t> Board::getImageHashes() const
{
	vector<size_t> out;

	for (string board : toImageStrings())
		out.push_back(hash<string>()(board));

	return out;
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
		break;
	case EAST:
		start.first += 1;
		break;
	case WEST:
		start.first -= 1;
		break;
	}

	return start;
}

string Board::toHumanString() const
{
	string out = "";

	for (int j = 6; j >= 0; j--)
	{
		for (int i = 0; i < 7; i++)
		{
			Coord coord = make_pair(i, j);
			if (withinBounds(coord))
			{
				if (isOccupied(coord))
					out += "o";
				else
					out += "+";
			}
			else
				out += ".";
		}

		out += "\n";
	}

	return out;
}


void printMove(ostream& out, Move move)
{
	printCoords(out, move.first);
	out << " - ";
	printDirection(out, move.second);
	out << endl;
}

void printDirection(ostream& out, Direction direction)
{
	switch (direction) {
	case NORTH:
		out << "NORTH";
		break;
	case SOUTH:
		out << "SOUTH";
		break;
	case WEST:
		out << "WEST";
		break;
	case EAST:
		out << "EAST";
		break;
	}
}

void printCoords(ostream& out, Coord coord)
{
	out << "(" << coord.first << ", " << coord.second << ")";
}

Coord transformCoord(const Coord& in, int angle)
{
	angle %= 8;

	int x = in.first;
	int y = in.second;

	if ( angle & 1 ) //flip
		x = 6 - x;

	if ( angle & 2 ) //rotate 90*
	{
		int temp = x;
		x = 6 - y;
		y = temp;
	}

	if ( angle & 4 ) //rotate 180*
	{
		x = 6 - x;
		y = 6 - y;
	}

	return make_pair(x, y);


}
