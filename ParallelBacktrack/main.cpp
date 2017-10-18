#include <iostream>
#include <unordered_set>
#include <fstream>
#include "board.h"
#include "backtrack.h"

using namespace std;

vector<string> readLinesFromFile(string filename);

int main()
{
	/*vector<string> boards = readLinesFromFile("/home/vapour101/Development/Uni/PC/ParallelProject/ParallelBacktrack/initial_states/moves:20");

	for (string board : boards) {
		Board check{board};
		backtrack checker(check, 4);

		checker.start();
		checker.print(cout);
		if (!checker.hasSolution())
			checker.print(cout);
	}*/

	Board check{"..+++....+o+..++ooo+++ooooo+ooooooo..+++....+++.."};
	backtrack checker(check, 4);
	checker.start();
	checker.print(cout);

	return 0;
}

vector<string> readLinesFromFile(string filename)
{
	ifstream file(filename);
	string line;
	vector<string> lines;

	if (!file.is_open())
	{
		throw "Can't open file.";
	}

	while (getline(file, line))
	{
		lines.push_back(line);
	}

	file.close();

	return lines;
}
