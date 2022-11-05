
#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <map>
#include <vector>
#include<iostream>
#include<cstdlib>

//todo:
//define sets (maybe just store an int that keeps track of the current maximum int used, then compare ints to others rather than being their own thing
//accessing other rows (accessing previous row is just currentRow - 1, if i fill out the whole maze with Rows at the start it could do currentRow+1 (but that would make it not infinite))


//http://www.neocomputer.org/projects/eller.html
//this site explains the steps really well

//https://docs.google.com/document/d/1BqtOUXnWo0vrQLjQ0Xq7DQ8lwiTlZSfrVPs0eOroa1c/edit
//my notes

struct Row
{
	//stores a set of Point2D for each space in the row
	//array/vector length SIDE
	//creation will run a for loop adding (i, currentRow) to the array


};


class Eller : public MazeGeneratorBase {
private:

	const int WALL_CHANCE = 50; //currently just 50/50, higher chance will make more vertical maze, lower will make more horizontal


	std::map<int, std::map<int, int>> m; //to give each point a set number

	int currentRow = 0; //will add 1 each Step to move down the last row
	int maxSet = 1;



	std::vector<Point2D> getSet(int setNum, int rowNum, World* w);

public:

	Eller() = default;
	std::string GetName() override { return "Eller's Algorithm"; };
	bool Step(World* world) override; //returns true if it worked basically
	//1 step = 1 row
	void Clear(World* world) override; //clears rows and sets all elements to false (except outer walls maybe)
};