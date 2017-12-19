#include "maze.h"
#include <cstring>

Maze* readMazeText(char[]);

class PacMan {
private:
	Maze *m;
	Square* current;	

	vector<Square*> closedList;
	vector<Square*> openList;

	vector<Square*> goalArray;
	vector<Square*> goalVisited;
	Square* currentGoal;

	int heuristicType;	
	int cost;
	int frontierSize;
	int totalClosedListSize;

	bool mazeFound;

	int goalCount; 
public:
	
	PacMan(Maze *maze, int type) {
		m = maze;
		if(m != NULL){
			current = m->getStartingSquare();		
			current->setCumulative(0);
			current->setHeuristic(type,m->getEndSquare().getCol(),m->getEndSquare().getRow());
			current->setFScore();
			
			openList.push_back(current);

			for(int i = 0; i < maze->getLength(); i++) {
				for(int j = 0; j < maze->getWidth(); j++) {
					if(maze->getSquare(i,j)->getItem() == END) {	
						Square *pushed_goal = new Square(i,j,END);
						goalArray.push_back(pushed_goal);					
					}
				}
			}

			cout << "printing maze contents: " << goalArray[0]->getItem() << endl;
			heuristicType = type;
			goalCount = frontierSize = totalClosedListSize = cost = 0;
			mazeFound = true;			
		} else { mazeFound = false; }
	}

	~PacMan(){
		m = NULL;
		currentGoal = current = NULL;
		delete m;
		delete current;
		delete currentGoal;
	}

	bool inStartState() { return mazeFound; }			
	string mazeToString() { return m->toString(); }

	bool solve();
	void setCurrentGoal();
	int selectClosestGoal();
	int firstNotVisitedInGoalArray();
	bool inGoalVisited(Square*);
	void initializeSquareValues();
	void switchCurrentToClosed();
	Square* getLowestCostSquare();
	bool foundOneGoal();	
	bool inGoalVisited();
	void scoutDirections();
	bool addSquare(int,int);
	bool inOpenList(Square*);
	bool mazeCompleted() { cout << "goalCount completeness: ";
					   	   cout << goalCount << " / " << goalArray.size() << endl << endl;
						   return ((goalCount == goalArray.size()) ? true : false); }
	void refresh();

	void printStatistics();
	string reconstructPath();
	void tracePath(Square* path, int goalKey, Square* lastGoal, int substractCoefficient);
	bool lastGoalVisited(Square*);
};

bool PacMan::solve() {		
	bool solved = false;
	
	setCurrentGoal();			
	initializeSquareValues();

	while(!this->openList.empty()) {
		cout << "current coordinates: ";
		cout << current->getRow() << " " << current->getCol() << endl;
		cout << "currentGoal coordinates: ";
		cout << currentGoal->getRow() << " " << currentGoal->getCol() << endl << endl;
		
		switchCurrentToClosed();		
		if(!foundOneGoal()) {
			scoutDirections();
		}
		else {
			cout << "FOUND A GOAL" << endl;
			cout << "Goal coordinates: " << "Row: " << current->getRow();
			cout << " Col: " << current->getCol() << endl;
			if(mazeCompleted()){
				solved = true;
				break;
			} else {
				refresh();
				setCurrentGoal();			
				initializeSquareValues();
			}

		}
	}
	return solved;
}

void PacMan::setCurrentGoal() {	
	Square *g = goalArray[selectClosestGoal()];
	currentGoal = new Square(g->getRow(), g->getCol(), g->getItem());
}

int PacMan::selectClosestGoal() {
	int min = firstNotVisitedInGoalArray(), pos = 0;
	for(vector<Square*>::iterator it = this->goalArray.begin(); it != this->goalArray.end(); it++) {
		if(!inGoalVisited(*it)){
			int sourceX = current->getRow();
			int sourceY = current->getCol();
			int destX = (*it)->getRow();
			int destY = (*it)->getCol();
			int distance = (heuristicType == MD) ? computeManhattanDistance(sourceX, sourceY, destX, destY) :
										  computeStraightLineDistance(sourceX, sourceY, destX, destY);

			(*it)->setHeuristic(heuristicType, sourceX, sourceY);

			cout << "goal Array contents: " << (*it)->getRow() << " " << (*it)->getCol() << " " << (*it)->getItem();
			cout <<" Distance: "<<distance<<" goalArray[min]: "<<goalArray[min]->getHeuristic()<<endl<<endl;
			if(distance <= goalArray[min]->getHeuristic())
				min = pos;
		}
		pos++;			
	}

	return min;
}

int PacMan::firstNotVisitedInGoalArray() {
	int pos = 0;
	for(vector<Square*>::iterator it = this->goalArray.begin(); it != this->goalArray.end(); it++) {
		if(!inGoalVisited(*it)) {
			return pos;
		}
		pos++;
	}
}

bool PacMan::inGoalVisited(Square* s) {
	for(int i = 0; i < goalVisited.size(); i++)
		if(goalVisited[i]->getRow() == s->getRow() && goalVisited[i]->getCol() == s->getCol())
			return true;
	return false;
}

void PacMan::initializeSquareValues() {
	for(int i = 0; i < m->getLength(); i++) {
		for(int j = 0; j < m->getWidth(); j++) {
			m->getSquare(i,j)->setCumulative(100000000);
			m->getSquare(i,j)->setHeuristic(heuristicType, currentGoal->getRow(), currentGoal->getCol());
			m->getSquare(i,j)->setFScore();
		}
	}

	current->setCumulative(0);
	current->setFScore();
}

void PacMan::switchCurrentToClosed() {	
	closedList.push_back(getLowestCostSquare());
	totalClosedListSize++;
	current = closedList.back();	
	m->setVisited(current->getRow(), current->getCol());	
}

Square* PacMan::getLowestCostSquare() {
	int min = 1000000000;	
	vector<Square*>::iterator del;

	for(vector<Square*>::iterator it = openList.begin(); it != openList.end(); it++) {		
		if((*it)->getFScore() <= min) {
			min = (*it)->getFScore();
			del = it;
			this->current = *it;
		}				
	}
	
	openList.erase(del);

	return current;
}

bool PacMan::foundOneGoal() {	

	if(current->getItem() == END && !inGoalVisited()) {		
		goalCount++;
		goalVisited.push_back(m->getSquare(current->getRow(), current->getCol()));
		return true;		
	}
	return false;
}

bool PacMan::inGoalVisited() {
	for(int i = 0; i < goalVisited.size(); i++)
		if(goalVisited[i]->getRow() == current->getRow() && goalVisited[i]->getCol() == current->getCol())
			return true;
	return false;
}

void PacMan::scoutDirections() {
	addSquare(current->getRow()-1,current->getCol());
	addSquare(current->getRow(),current->getCol()-1);
	addSquare(current->getRow()+1,current->getCol());
	addSquare(current->getRow(),current->getCol()+1);	
}

bool PacMan::addSquare(int row, int col) {
	Square* sq = m->getSquare(row,col);
	if(row > m->getLength() || col > m->getWidth() || row < 0 || col < 0) {
		return false;
	} else if (m->getSquare(row, col)->getItem() == WALL) {
		return false;
	} else if(m->getSquare(row,col)->isVisited()) {
		return false;
	} else if(!inOpenList(m->getSquare(row,col))) {
		openList.push_back(m->getSquare(row,col));		
		frontierSize++;
	}

	int tentative_cumulative_cost = current->getCumulative() + 1;
	if (tentative_cumulative_cost < m->getSquare(row, col)->getCumulative())  {
		m->setParent(row,col,current,goalCount);
		m->setCumulativeCost(row,col,current->getCumulative()+1);
		m->setFScore(row, col); 
	}
	return true;
}

bool PacMan::inOpenList(Square* s) {
	for(int i = 0; i < openList.size(); i++)
		if(openList[i]->getRow() == s->getRow() && openList[i]->getCol() == s->getCol())
			return true;
	return false;
}

void PacMan::refresh() {
	openList.clear();
	openList.push_back(current);
	closedList.clear();
	for(int i = 0; i < m->getLength(); i++) {
		for(int j = 0; j < m->getWidth(); j++) {
			m->setVisited(i,j,false);
		}
	}
}

void PacMan::printStatistics() {	
	cout << endl << "Path: " << endl << reconstructPath() << endl;	
	cout << "Path cost: " << cost << endl;
	cout << "Expanded Nodes: " << totalClosedListSize << endl;
	cout << "Frontier Size: " << frontierSize << endl;
}

string PacMan::reconstructPath() {
	string path = "";
	Square* target;
	vector<Square*>::iterator it;

	target = m->getSquare(goalVisited.back()->getRow(), goalVisited.back()->getCol());

	if(target == NULL)
		return "Path not found.";
	else {
		cout << "Tracing path:" << endl;
		tracePath(target, goalCount, target, 1);
		return m->toString();
	}
}

void PacMan::tracePath(Square* path, int goalKey, Square* lastGoal, int substractCoefficient) {
	cost++;

	if(lastGoalVisited(path)) {
		m->getSquare(path->getRow(), path->getCol())->setItem((goalKey - substractCoefficient) + 'a');
		if(!(path->getRow() == lastGoal->getRow() && path->getCol() == lastGoal->getCol())) {
			goalKey -= 1;
		} else {
			substractCoefficient = 2;
		}
	} else if(m->getSquare(path->getRow(), path->getCol())->getItem() == OPENPATH) {
		m->getSquare(path->getRow(), path->getCol())->setItem('.');
	}

	if(path->getParent(goalKey-1) == NULL) return;
	cout<<m->getSquare(path->getRow(), path->getCol())->toString()<<endl;
	
	return tracePath(path->getParent(goalKey-1), goalKey, lastGoal, substractCoefficient);
} 

bool PacMan::lastGoalVisited(Square* sq) {
	if(!goalVisited.empty() && (goalVisited.back()->getRow() == sq->getRow() && 
								goalVisited.back()->getCol() == sq->getCol())) {
		goalVisited.pop_back();
		return true;
	}
	return false;
}

Maze* readMazeText(char fileName[]) {
	string path = "../mazes/";
	char *pathToFile = new char[path.length()+strlen(fileName)+1];

	strcpy(pathToFile,path.c_str());
	strcat(pathToFile,fileName);

	ifstream file(pathToFile);

	if(!file.is_open()) {
		cout << "File not found." << endl;
		return NULL;
	}

	string fileContents="", buffer;
	int lineCount = 0, lineWidth = 0;

	while(getline(file,buffer)){
		if(lineWidth == 0) {
			lineWidth = buffer.length();
		}

		fileContents += buffer.substr(0,lineWidth);
		lineCount++;
	}

	Maze* ret = new Maze(lineCount,lineWidth,fileContents);


	return ret;
}
