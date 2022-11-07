#include "Eller.h"
#include "../World.h"
#include "Random.h"
#include <climits>

bool Eller::Step(World* w) 
{
	usedSets.clear();

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
		if (!w->GetNorth({ i, currentRow }))
		{
			m[i][currentRow] = m[i][currentRow - 1]; //sets set to the set of above cell
		}
		else
		{
			m[i][currentRow] = maxSet;
			maxSet++;
			//sets cell to new set
		}


	}

	//step 3 in the Maze Algorithm Notes
	//this is where walls get added
	for (int i = -sideLength; i < sideLength; i++)
	{
		srand((unsigned)time(NULL));

		if (m[i][currentRow] != m[i + 1][currentRow]) //the next cell is a different set, must randomly decide to add wall or not
		{
			int randNum = rand() % 100; // 0 - 100 range

			if (randNum > WALL_CHANCE) //put wall, no more needed to be done
			{
				w->SetEast({ i, currentRow }, true);
			}
			else //didn't put wall, must group up the next set
			{
				std::vector<Point2D> checkList = getSet(m[i + 1][currentRow], currentRow, w);
				for (auto p : checkList)
				{
					m[p.x][p.y] = m[i][currentRow];
				}
			}
		}
		else //next cell is same set, must put wall
		{
			w->SetEast({ i, currentRow }, true);
		}

	}

	//Adding south walls

	for (int i = -sideLength; i <= sideLength; i++) //running through row
	{
		if (std::find(usedSets.begin(), usedSets.end(), m[i][currentRow]) == usedSets.end()) //set is not in used sets
		{
			std::vector<Point2D> currentSet = getSet(m[i][currentRow], currentRow, w);
			if (currentSet.size() == 1) //only one member of set, south should be false
			{
				w->SetSouth(currentSet[0], false);
			}
			else //multiple members of set, all but one should have south walls (ideally would do random amount of elements but for now just doing 1)
			{ 
				srand((unsigned)time(NULL));
				int randNum = rand() % currentSet.size()-1;
				for (int j = 0; j <= currentSet.size() - 1; j++)
				{
					if (j == randNum)
					{
						w->SetSouth(currentSet[j], false);
					}
					else
					{
						w->SetSouth(currentSet[j], true);
					}
				}
			}
			usedSets.push_back(m[i][currentRow]); //adding set to used sets so it will not be checked again
		}
	}
	currentRow++;
	return true;
}

void Eller::Clear(World* world) 
{
	m.clear();
	usedSets.clear();

	int sideLength = world->GetSize() / 2;

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