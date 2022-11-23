
#include "../GeneratorBase.h"
#include <map>
#include <math.h> 

struct TerrainNode
{
public:

	TerrainNode() = default;
	TerrainNode(int setX, int setY, float setHeight, Color32 setColor)
	{
		x = setX;
		y = setY;
		height = setHeight;
		color = setColor;
	}

	void setColor(Color32 newColor)
	{
		color = newColor;
	}

	Color32 getColor()
	{
		return color;
	}

	float getHeight()
	{
		return height;
	}

private:
	int x, y;
	float height; //height is original rgb value from the noise
	Color32 color;
};


class MyGenerator : public ScenarioGeneratorBase{
 public:
  std::vector<Color32> Generate(int sideSize, float displacement=0) override;
  std::string GetName() override;

  void BuildRoads(std::map<int, std::map<int, TerrainNode>>* nodes, int sideSize);

  void Erode(std::map<int, std::map<int, TerrainNode>>* nodes, int sideSize);
  void Erode(std::vector<float>& heights, int sideSize, std::vector<Vec2>& used);

  void Blur(std::vector<float>& heights, int sideSize);

  float getHeightAtPoint(std::vector<float>& heights, int x, int y, int sideSize);
  std::vector<float> getSurrounding(std::vector<float>& heights, int x, int y, int sideSize);

  void toggleBlackout() { blackout = !blackout; }
  void toggleErosion() { erosion = !erosion; }
  void toggleVisibleErosion() { showErosion = !showErosion; }

  bool getErosion() { return erosion; }


  void ChangeHeight(std::vector<float>& heights, int x, int y, int sideSize, float change);

  Vec3 crossProduct(Vec3 a, Vec3 b);

  Vec3 GetNormal(std::vector<float>& heights, Vec3 point, int sideSize);

private:
	float water = 60; //this and below is water (blue)
	float beach = 90; //very light brown
	float grass = 160; //green
	float mountain = 210; //brown
	//leftover will be tops of mountain (white)

	bool blackout = false; //just a test

	bool road = false;
	bool erosion = false;
	bool showErosion = false;
	
	int erosions = 2000; //will add a slider in UI to control how much erosion there is


protected:
	//bool blackout = false; //just a test
};

