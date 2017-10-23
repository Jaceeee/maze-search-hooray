#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <algorithm>
#include "pacman.h"

using namespace std;

// TODO; FINISH

int main() {
	char fileName[100];	
	int option;
	cout << "Enter file name: ";
	cin >> fileName;	
	cout << endl << "Enter option type: ";
	cin >> option;
	cout << endl;
	
	PacMan p(readMazeText(fileName),option);
	if(p.inStartState()){
		cout << p.mazeToString() << endl;
		string sol = (p.solve()) ? "Yes" : "No";
		cout << "Solved? " << sol << endl << endl;	
		p.printStatistics();	
	} else {
		cout << "recheck file name arguments" << endl;
	}
	return 0;
}

