#include "Eller.h"
#include "../World.h"
#include "Random.h"
#include <climits>


//note: top row is not zero, it is -1 * SIDE_SIZE
// note: w->data is the list of bools
// 
// 
// step 1: check North, if it is false, set setNum equal to node above it. The first row should have all North true so wouldn't apply. This means each Step only needs the rows before it and not the ones after
// 
//will fill in all points without a set with a random set
//				for loop through the Row's vector
//sets will start at 1, if they have 0 then they have not been filled
//have a maxSet int stored that is increased every time a new set is used

//instead of maxSet, could just use 1 through SIDE_SIZE and keep track of which ones are in use. Not sure if this is easier


//after filling in the sets, group them up 
//		maybe: 2 loops through the row, once will randomly place right walls (50% chance for each node maybe) then second will set the setNum of each group to be the same
//		maybe: go through each node, if the one to the right is the same, place a wall, otherwise randomly add a wall (if you do not place a wall, the node on the right should have its set equal to left node, also all other nodes with its set)



//have a check if this is the bottom row, if it is then all of the nodes are the same set and the bottoms should be True for all of them

bool Eller::Step(World* w) 
{
	int sideLength = w->GetSize() / 2;

	if (abs(currentRow) > sideLength)
	{
		return false; //outside of maze
	}

	//sets left and right walls
	w->SetWest({ -sideLength, currentRow }, true);
	w->SetEast({ sideLength, currentRow }, true);

	

	if (currentRow == -sideLength) //first row, north should all be true
	{
		for (int i = -sideLength; i < sideLength; i++)
		{
			w->SetNorth({ i, currentRow }, true);
		}
	}

	//step 1 of Maze Algorithm Notes

	if(currentRow == sideLength)
	{
		for (int i = -sideLength; i <= sideLength; i++)
		{
			w->SetSouth({ i, currentRow }, true);
		}


		return true; //don't need the rest
	}

	
	//step 2 in the Maze Algorithm Notes
	for(int i = -sideLength; i <= sideLength; i++)
	{
		if (!w->GetNorth({ i, rowNum }))
		{
			m[i][rowNum] = m[i][rowNum - 1]; //sets set to the set of above cell
		}
		else
		{
			m[i][rowNum] = maxSet;
			maxSet++;
			//sets cell to new set
		}


	}


	//step 3 in the Maze Algorithm Notes
	//this is where walls get added
	for (int i = -sideLength; i <= sideLength; i++)
	{
		srand((unsigned)time(NULL));

		if (m[i][rowNum] == m[i + 1][rownum]) //the next cell is a different set, must randomly decide to add wall or not
		{
			int randNum = rand() % 100; // 0 - 100 range

			if (randNum > WALL_CHANCE) //put wall, no more needed to be done
			{
				w->SetEast({ i, rowNum }, true);
			}
			else //didn't put wall, must group up the next set
			{
				std::vector<Point2D> checkList = getSet(m[i][rowNum], rowNum, w);
				for(auto p : checkList)
				{
					m[p.x][p.y] = m[i][rowNum];
				}


			}
		}
		else //next cell is same set, must put wall
		{
			w->SetEast({ i, rowNum }, true);
		}
	}

	

	



	return true;


}


void Eller::Clear(World* world) 
{
	m.clear();

	int sideLength = w->GetSize() / 2;

	for (int i = -sideLength; i <= sideLength; i++)
	{
		for (int j = -sideLength; j <= sideLength; j++)
		{
			m[i][j] = 0;
		}
	}
	currentRow = -world->GetSize() / 2;
}

std::vector<Point2D> Eller::getSet(int setNum, int rowNum, World* w)
{
	auto sideOver2 = w->GetSize() / 2;
	std::vector<Point2D> setList;
	for (int i = -sideOver2; i <= sideOver2; i++) {
		if (m[i][rowNum] == setNum)
		{
			setList.push_back({ i, rowNum });
		}
	}
	return setList;
}