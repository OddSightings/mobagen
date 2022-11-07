
#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <map>
#include <vector>
#include<iostream>
#include<cstdlib>

//http://www.neocomputer.org/projects/eller.html
//this site explains the steps really well

//https://docs.google.com/document/d/1BqtOUXnWo0vrQLjQ0Xq7DQ8lwiTlZSfrVPs0eOroa1c/edit
//my notes




class Eller : public MazeGeneratorBase {
private:

	const int WALL_CHANCE = 20; //currently just 50/50, higher chance will make more vertical maze, lower will make more horizontal


	std::map<int, std::map<int, int>> m; //to give each point a set number

	int currentRow = 0; //will add 1 each Step to move down the last row
	int maxSet = 1; //starting at 1 so i can check to see if there is no set
	std::vector<int> usedSets; //saves used sets while adding south walls


	std::vector<Point2D> getSet(int setNum, int rowNum, World* w);

public:
	
	Eller() = default;
	std::string GetName() override { return "Eller's Algorithm"; };
	bool Step(World* world) override; //returns true if it worked basically
	void Clear(World* world) override; //clears rows and sets all elements to false (except outer walls maybe)

	void setRow(int n) { currentRow = n; };
};