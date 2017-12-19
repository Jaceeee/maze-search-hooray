#include "square.h"

using namespace std;

class Maze {
private:
	int length, width;
	Square *start;
	Square *end;
	Square **maze;
public:
	Maze() {
		this->length = 0;
		this->width = 0;
	}

	Maze(const Maze& m) {
		this->length = m.length, this->width = m.width;
		this->maze = m.maze;

		for(int i = 0; i < length; i++)
			this->maze[i] = new Square[width];

		for(int i = 0, k = 0; i < length; i++) {
			for(int j = 0; j < width; j++, k++) {
				this->maze[i][j]=m.maze[i][j];
				if(this->maze[i][j].getItem() == START) {
					this->start = &this->maze[i][j];
				}
				else if(this->maze[i][j].getItem() == END) {
					this->end = &this->maze[i][j];
				}
			}
		}
	}

	Maze(int length, int width, string mazeInFile) {
		this->length = length;
		this->width = width;
		this->maze = new Square*[length];

		for(int i = 0; i < length; i++)
			this->maze[i] = new Square[width];

		for(int i = 0, k = 0; i < length; i++) {
			for(int j = 0; j < width; j++, k++) {
				this->maze[i][j]=Square(i,j,mazeInFile[k]);
				if(this->maze[i][j].getItem() == START) {
					this->start = &this->maze[i][j];					
				}
				else if(this->maze[i][j].getItem() == END) {
					this->end = &this->maze[i][j];					
				}
			}
		}		
	}

	~Maze() {
		for (int i = 0; i < length; i++){			
			maze[i] = NULL;
			delete[] maze[i];
		}

		maze = NULL;
		start = end = NULL;
		delete[] maze;
		delete start;
		delete end;		
	}

	Square* getSquare(int x ,int y) { return &maze[x][y]; }
	Square* getStartingSquare() { return this->start; }
	Square getEndSquare() { return *this->end; }
	void setVisited(int x, int y) {	maze[x][y].setVisited(); }
	void setVisited(int x, int y, bool arg)	 { maze[x][y].setVisited(arg); }
	void setParent(int x, int y, Square* parent, int goalKey) { maze[x][y].setParent(goalKey, parent); }
	void setCumulativeCost(int x, int y, int cost) { maze[x][y].setCumulative(cost); }
	void setHeuristic(int type, Square goal, int x, int y) { maze[x][y].setHeuristic(type,goal.getRow(),goal.getCol()); }
	void setFScore(int x, int y) { maze[x][y].setFScore(); }
	int getLength() { return length; }
	int getWidth() { return width; }

	string toString() {
		string stringMaze = "";
		
		for(int i = 0; i < length; i++){
			for(int j = 0; j < width; j++){
				stringMaze += maze[i][j].getItem();
			}
			stringMaze += '\n';
		}

		return stringMaze;
	}

	Maze& operator=(const Maze&);
};

Maze& Maze::operator=(const Maze& maze) {	
	this->length = maze.length, this->width = maze.width;
	this->maze = maze.maze;

	for(int i = 0; i < length; i++)
		this->maze[i] = new Square[width];

	for(int i = 0, k = 0; i < length; i++) {
		for(int j = 0; j < width; j++, k++) {
			this->maze[i][j]=maze.maze[i][j];			
			if(this->maze[i][j].getItem() == START) { this->start = &this->maze[i][j]; }
			else if(this->maze[i][j].getItem() == '.') { this->end = &this->maze[i][j]; }
		}
	}

	return *this;
}