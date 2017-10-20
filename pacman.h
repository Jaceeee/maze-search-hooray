#include "maze.h"

class PacMan {
private:
	Maze* m;
	int heuristicType;
	vector<Square> parentList;
	vector<Square*> closedList;
	vector<Square*> openList;
	Square* current;
	Square goal;
public:
	PacMan(Maze* maze, int type) { 
		m = maze;
		heuristicType = type;
		current = m->getStartingSquare();
		openList.push_back(current);
		current = openList.back();
		goal = m->getEndSquare();
	}

	~PacMan(){
		// delete parentList;
		// delete closedList;
		// delete openList;
		//delete current;
	}

	bool addSquare(int,int);	// change to checkNeighbor
	void scoutDirections();		// change to evaluateCurrentSquare
	Square* getLowestCostSquare();
	void switchCurrentToClosed();		// change to moveToNextSquare; Jace suggests switchCurrentToClosed
	bool inOpenList(Square*);
	bool fin();
	bool solve();
	void pathChange(Square*, int);
	string reconstructPath();
	string pathToString(Square*);
	string mazeToString() { return m->toString(); }

};

bool PacMan::solve() {
	// //initialize Heuristics
	
	// for(int i = 0; i < m->getLength(); i++) {
	// 	for(int j = 0; j < m->getWidth(); j++) {
	// 		m->setHeuristic(heuristicType, goal, i, j);
	// 	}
	// }

	// return true;

	//current
	bool found = false;
	while(!this->openList.empty()) {
		switchCurrentToClosed();
		if(!fin()) {
			scoutDirections();
		}
		else {
			found = true;
			break;
		}
	}

// String aStar() {
	// while(openSet.isNotEmpty) {
		// moveToNextSquare(); //
		// if(fin()) {
			// return reconstructPath()
		// }
		//
		// evaluateCurrentSquare();
	//}
	// return "Failure, no possible path";
// 	return found;
// }
}

void PacMan::switchCurrentToClosed() {
	closedList.push_back(getLowestCostSquare());
	//cout << "did you come here friend ?1" << endl;

	//vector<Square*>::iterator it = openList.back();
	current = openList.back();
	//cout << "did you come here friend ?2" << endl;

	cout << current->getItem() << endl;
	//cout << "asa na mn ka ???" << endl;
	m->setVisited(current->getRow(), current->getCol());
	//cout << "did you come here friend ?3" << endl;

}

Square* PacMan::getLowestCostSquare() {
	int min = numeric_limits<int>::infinity();

	// supposedly it->getCumulative() + it->getHeuristic()
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getCumulative() < min) {
			min = (*it)->getCumulative();
			this->current = *it;
		}
	}
	//cout << " ni ari ka sa lowestCostSquare" << endl;
	return current;
}


string PacMan::reconstructPath() {
	string path = "";
	Square* target;
	vector<Square*>::iterator it;

	for(it = closedList.begin(); it != closedList.end(); it++) {
		if((*it)->getRow() == this->goal.getRow() && (*it)->getCol() == this->goal.getCol())
			target = *it;
	}

	if(target == NULL)
		return "Path not found.";
	else {
		path = pathToString(target);
		return path;
	}
}

string PacMan::pathToString(Square* t) {
	if(t->getParent() == NULL) return t->toString();
	return pathToString(t->getParent()) + t->toString();
}


// 3.1
// change to checkNeighbor
// add condition if square in closed set and if not in open set
bool PacMan::addSquare(int x, int y) {
	Square sq;
	if(x > m->getLength() || y > m->getWidth() || x < 0 || y < 0) {
		return false;
	}
	else if (m->getSquare(x, y)->getItem() == WALL) {
		return false;
	}
	// to be added/changed in condition ^; uncommented by Jace
	else if(m->getSquare(x,y)->isVisited()) {
		return false;
	}
	else if(!inOpenList(m->getSquare(x,y))) {
		m->setHeuristic(this->heuristicType,this->goal,x,y);
		m->setCumulativeCost(x,y,current->getCumulative()+1);
		m->setFScore(x,y);
		openList.push_back(m->getSquare(x,y));
	}
	// special check for g-score to determine if better path; uncommented by Jace
	int tentative_cumulative_cost = current->getCumulative() + 1;
	if (tentative_cumulative_cost < m->getSquare(x, y)->getCumulative())  {
	//		This path is the best until now. Record it
		m->setParent(x,y,current);
		m->setCumulativeCost(x,y,current->getCumulative()+1);
	}
	return true;
}

void PacMan::pathChange(Square *target, int newCost) {
	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {
		if((*it)->getRow() == target->getRow() && (*it)->getCol() == target->getCol()) {
			(*it)->setParent(target);
			(*it)->setCumulative(newCost);
		}
	}
}

bool PacMan::inOpenList(Square* s) {
	for(int i = 0; i < openList.size(); i++)
		if(openList[i]->getRow() == s->getRow() && openList[i]->getCol() == s->getCol())
			return true;
	return false;
}

// 3
// change to evaluateCurrentSquare
void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);
}

// 2
bool PacMan::fin() {
	if(current->getRow() == goal.getRow() && current->getCol() == goal.getCol()){
		return true;
	}
	return false;
}
