#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include "board.h"
#include "backtrack.h"

using namespace std;

vector<string> readLinesFromFile(const string& filename);
int stringToInt(const string& str);

int main(int argc, char* argv[])
{
	int averageOver = 10;

	if (argc < 2)
		return 0;

	if (argc >= 3)
	{
		averageOver = stringToInt(argv[2]);
	}

	string fileName = argv[1];
	vector<string> boards = readLinesFromFile(fileName);

	for (string board : boards)
	{
		Board check{board};
		backtrack checker{check};

		chrono::nanoseconds sequentialTime = chrono::nanoseconds::zero();
		chrono::nanoseconds parallelTime = chrono::nanoseconds::zero();

		for (int i = 0; i < averageOver; ++i)
		{
			checker.clear();
			checker.start();
			sequentialTime += checker.getDuration();
		}

		checker.setParallel(true);

		for (int i = 0; i < averageOver; ++i)
		{
			checker.clear();
			checker.start();
			parallelTime += checker.getDuration();
		}

		sequentialTime /= averageOver;
		parallelTime /= averageOver;

		cout << sequentialTime.count() << ", " << parallelTime.count() << endl;
	}

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

int stringToInt(const string& str)
{
	istringstream stream{str};
	int out;
	stream >> out;
	return out;
}
