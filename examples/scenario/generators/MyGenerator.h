
#include "../GeneratorBase.h"

class MyGenerator : public ScenarioGeneratorBase{
 public:
  std::vector<Color32> Generate(int sideSize, float displacement=0) override;
  std::string GetName() override;
  bool blackout = false; //just a test

  bool road = false; 
  bool erosion = false;


private:
	float water = 70; //this and below is water (blue)
	float snow = 200; //this and above is snow (white)
	float grass = 140; //this and below is grass (green)
	//leftover will be mountain (brown)

protected:
	//bool blackout = false; //just a test
};

