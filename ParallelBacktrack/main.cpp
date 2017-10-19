#include <iostream>
#include <unordered_set>
#include <fstream>
#include "board.h"
#include "backtrack.h"

using namespace std;

vector<string> readLinesFromFile(const string& filename);

int main()
{
      /*vector<string> boards = readLinesFromFile("/home/vapour101/Development/Uni/PC/ParallelProject/ParallelBacktrack/initial_states/moves:20"); */

    vector<string> boards = readLinesFromFile("/home/tau/Documents/Bsc3/Coms3/Parallel Computing/ParallelProject/ParallelBacktrack/initial_states/hard");

    for (string board : boards) {
		Board check{board};
		backtrack checker(check);

        /*checker.start();
		checker.printTime(cout);
        checker.clear();*/

        checker.setParallel(true);
		checker.start();
        checker.print(cout);

		cout << endl;

		//if (checker.hasSolution())
        //checker.printSequence(cout);
    }

    /*
	Board check{"..+++....+o+..++ooo+++ooooo+ooooooo..+++....+++.."};
	backtrack checker(check);

	chrono::nanoseconds time = chrono::nanoseconds::zero();

	for (int i = 0; i < 10; i++)
	{
		checker.clear();
		checker.start();
		time += checker.getDuration();
	}

	time /= 10;
	cout << "Sequential: " << time.count() / 1000000 << "ms" << endl;
	checker.setParallel(true);

	time = chrono::nanoseconds::zero();

	for (int i = 0; i < 10; i++)
	{
		checker.clear();
		checker.start();
		time += checker.getDuration();
	}

	time /= 10;
    cout << "Parallel: " << time.count() / 1000000 << "ms" << endl;*/

	return 0;
}

vector<string> readLinesFromFile(const string& filename)
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
