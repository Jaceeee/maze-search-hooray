using namespace std;

int computeManhattanDistance(int, int, int, int);
int computeStraightLineDistance(int, int, int, int);

enum {
	WALL = '%',
	OPENPATH = ' ',
	START = 'P',
	END = '.',
	MD = 1,
	SLD = 2
};

class Square {
private:
	char content;

	// position variables
	int row;
	int col;

	bool visited;
	int cumulativeCost; // g - score
	int heuristic;		// h - score
	int fScore;
	map<int,Square*> parentSpecificGoal;
public:
	Square() {
		row = 0;
		col = 0;
		visited = false;
		content = 0;
		cumulativeCost = heuristic = fScore = 0;
	}

	Square(const Square& sq) {
		this->content = sq.content;
		this->row = sq.row;
		this->col = sq.col;
		this->visited = sq.visited;
		this->cumulativeCost = sq.cumulativeCost;
		this->heuristic = sq.heuristic;
	}

	// set cumulativeCost of each square to infinity
	Square(int row, int col,char item) {
		this->row = row;
		this->col = col;
		visited = false;
		this->content = item;
		cumulativeCost = 100000000;
		heuristic = fScore = 0;
	}

	// ~Square() {
	// 	parent = NULL;
	// 	delete parent;
	// }

	bool isPassable() { return (content == WALL) ? false : true; }

	void setParent(int goalKey, Square *sq);
	Square* getParent(int goalKey) { return parentSpecificGoal[goalKey]; }
	bool isVisited() { return visited; } 

	char getItem() { return content; }
	int getRow() { return row; }
	int getCol() { return col; }
	int getHeuristic() { return heuristic; }
	int getCumulative() { return cumulativeCost; }
	int getFScore() { return fScore; }
	map<int,Square*> getParentMap() { return parentSpecificGoal; }

	void setVisited() { visited = (!visited) ? true : false; }
	void setVisited(bool arg) { visited = arg; }
	void setSquare(int,int);
	void setCumulative(int newCost) { this->cumulativeCost = newCost; }
	void setFScore() { fScore = cumulativeCost + heuristic; }
	void setHeuristic(int,int,int);
	void setItem(char item) { this->content = item; }

	string toString() {
		char buffer[20];
		sprintf(buffer, "Square (%d, %d)", this->getCol(), this->getRow());
		string str = buffer;
		return str;
	}

	Square& operator=(const Square& sq){
		this->content = sq.content;
		this->row = sq.row;
		this->col = sq.col;
		this->visited = sq.visited;
		this->cumulativeCost = sq.cumulativeCost;
		this->heuristic = sq.heuristic;
		return *this;
	}	
};

void Square::setSquare(int h_val, int g_val) {
	heuristic = h_val;
	cumulativeCost = g_val;
}

void Square::setParent(int goalKey, Square* sq) {
	map<int,Square*>::iterator it = parentSpecificGoal.find(goalKey);
	if(it != parentSpecificGoal.end()) {
		it->second = sq;
	} else {
		parentSpecificGoal.insert(pair<int,Square*>(goalKey, sq));	
	}
}

void Square::setHeuristic(int type, int destX, int destY) {
	if(type == MD) {
		heuristic = computeManhattanDistance(this->row, this->col, destX, destY);
	}
	else if(type == SLD) {
		heuristic = computeStraightLineDistance(this->row, this->col, destX, destY);
	}
	else {
		cout << "Heuristic type not valid. Resend arguments." << endl;
	}
}

// produce heuristics

int computeManhattanDistance(int srcX, int srcY, int destX, int destY) {
	return abs(destX-srcX) + abs(destY-srcY);
}

int computeStraightLineDistance(int srcX, int srcY, int destX, int destY) {
	return max(abs(destX-srcX),abs(destY-srcY));
}