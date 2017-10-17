#include <iostream>
#include <unordered_set>
#include "board.h"
#include "backtrack.h"

using namespace std;

int main()
{
	Board check{};
	backtrack checker(check, 4);

	checker.start();

	if (checker.hasSolution())
	{
		cout << "Solution found" << endl;
	for (Move m : checker.getSolution())
	{
		cout << "(" << m.first.first << ", " << m.first.second << ")" << " - " << m.second << endl;
	}}
	else {
		cout << "No solution found" << endl;
	}

	return 0;
}

