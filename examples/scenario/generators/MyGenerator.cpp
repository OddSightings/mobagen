#include "MyGenerator.h"
#include "../PerlinNoise.hpp"
#include <iostream>
// do not use this one to your assignment. this is my sample generator
std::vector<Color32> MyGenerator::Generate(int sideSize, float displacement) {
  std::vector<Color32> colors;
//   create your own function for noise generation
  siv::BasicPerlinNoise<float> noise;
  noise.reseed(1337);
  for(int l=0; l<sideSize; l++){
    for(int c=0;c<sideSize; c++){
      float rgb = ((noise.octave3D(c / 50.0, l / 50.0, displacement, 2) + 1) / 2) * 255;
      //i think from 1 - 255
      if (blackout)
      {
          colors.emplace_back(0, 0, 0);
      }
      else if (rgb <= water)
      {
          colors.emplace_back(0, 0, 180);
      }
      else if (rgb >= snow)
      {
          colors.emplace_back(255, 255, 255);
      }
      else if (rgb <= grass)
      {
          colors.emplace_back(0, 180, 0);
      }
      else
      {
          colors.emplace_back(63, 38, 29);
      }



      //colors.emplace_back(rgb, rgb, rgb);
    }
  }
  std::cout<<colors.size() << std::endl;
  // add altitude filtering behavior
  // add island behaviors / peaks
  return colors;
}
std::string MyGenerator::GetName() { return "Tommy's"; }