#include "MyGenerator.h"
#include "../PerlinNoise.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

//replace the colors vector with a map of TerrainNodes
//in the for loop, set each node's color to be the value instead of emplacing it
//run whichever modifying functions you want
//then at the end go through each node and emplace its color to a vector of colors and return it

std::vector<Color32> MyGenerator::Generate(int sideSize, float displacement) {
    
  std::vector<Color32> colors;

  std::vector<float> heights;


  std::map<int, std::map<int, TerrainNode>> terrain;

//   create your own function for noise generation
  siv::BasicPerlinNoise<float> noise;
  noise.reseed(1337);
  for(int l=0; l<sideSize; l++){
    for(int c=0;c<sideSize; c++){
        float rgb = ((noise.octave3D(c / 50.0, l / 50.0, displacement, 3, 0.5) + 1) / 2); //now in range 0-1, when you do colors multiply by 255 again

        heights.push_back(pow(rgb, 1.3));
        //exponent makes terrain overall lower, could make it below 1 to be overall higher
    }
  }

  //do any modifications
  if (erosion)
  {
      for (int i = 0; i < erosions; i++)
      {
          Erode(heights, sideSize);
      }
  }
      
  //modifications end

  for (int x = 0; x < sideSize; x++)
  {
      for (int y = 0; y < sideSize; y++)
      {
          if (blackout)
          {
              float m = getHeightAtPoint(heights, x, y, sideSize) * 255;
              colors.emplace_back(m, m, m);
          }
          else if (getHeightAtPoint(heights, x, y, sideSize)*255 <= water)
          {
              colors.emplace_back(0, 0, 180);
          }
          else if (getHeightAtPoint(heights, x, y, sideSize) * 255 <= beach)
          {
              colors.emplace_back(194, 178, 128);
          }
          else if (getHeightAtPoint(heights, x, y, sideSize) * 255 <= grass)
          {
              colors.emplace_back(0, 180, 0);
          }
          else if (getHeightAtPoint(heights, x, y, sideSize)*255 <= mountain)
          {
              colors.emplace_back(63, 38, 29);
          }
          else
          {
              colors.emplace_back(255, 255, 255);
          }
      }
  }
  std::cout<<colors.size() << std::endl;
  return colors;
}



std::string MyGenerator::GetName() { return "Tommy's"; }



void MyGenerator::BuildRoads(std::map<int, std::map<int, TerrainNode>>* nodes, int sideSize)
{




}
//https://github.com/weigert/SimpleErosion/tree/master/source/include/world
//https://nickmcd.me/2020/04/10/simple-particle-based-hydraulic-erosion/

void MyGenerator::Erode(std::vector<float>& heights, int sideSize) //this will be run in a for loop in Generate, will be called maxEroders times
{
    float st = 1.2;

    //create waater droplet at random point in map
    int dropX = rand() % (sideSize);
    int dropY = rand() % (sideSize);


    //will be used for previous position of droplet
    int xp = dropX;
    int yp = dropY;

    //storing velocity
    float vx = 0;
    float vy = 0;

    //offset for the droplet, keeps paths from being too similar
    int ox = ((rand() % 3) - 1); 
    int oy = ((rand() % 3) - 1); 

    //total volume of droplet
    float volume = 1.0;

    float minVol = 0.01;

    //fraction of volume that is sediment
    float sediment = 0;

    //density of droplet (water is 1000 kg/m^3)
    float density = 1;

    //friction of droplet (using standard for all terrain)
    float friction = 0.05;

    
    float depositionRate = 0.1;
    float evapRate = 0.001;

    float erosionRate = 0.1;

    std::clock_t c_start = std::clock();
    std::clock_t lastTick = c_start;
    float dt = 1.2;

    //while(volume >= minVol)
    for(int i = 0; i < sideSize / 3; i++)
    {        
        
        if (dropX >= sideSize || dropY >= sideSize || dropX <= 0 || dropY <= 0)
        {
            break; //out of bounds
        }
        
        Vec3 normal = GetNormal(heights, Vec3(dropX, dropY, getHeightAtPoint(heights, dropX,dropY, sideSize)), sideSize);

        if (normal.z == 1)
        {
            break; //surface is flat
        }
        

        //talle version

        float deposit = sediment * depositionRate * normal.z;
        float erosion = erosionRate * (1 - normal.z);

        heights[(sideSize * dropY) + dropX] += deposit - erosion;
        sediment += erosion - deposit;

        float speed = 1;

        vx = friction * vx + normal.x * speed;
        vy = friction * vy + normal.y * speed;
        xp = dropX;
        yp = dropY;
        dropX += vx;
        dropY += vy;



        //weigert version
        /*
        Vec2 speed = (Vec2(normal.x,normal.y) * dt) / (volume * density);
        //std::cout << "change in x speed: " << speed.x << ", change in y speed: " << speed.y << std::endl;
        vx += speed.x;
        vy += speed.y;

        dropX += vx * dt;
        dropY += vy * dt;     

        if (dropX > sideSize || dropY > sideSize || dropX < 0 || dropY < 0)
        {
            break; //out of bounds
        }

        vx *= (1.0 - dt * friction);
        vy *= (1.0 - dt * friction);

        float dropSpeed = sqrt(vx * vx * vy * vy);

        float maxSediment = volume * dropSpeed * (getHeightAtPoint(heights, xp, yp, sideSize) - getHeightAtPoint(heights, dropX, dropY, sideSize));

        if (maxSediment < 0) maxSediment = 0;

        float c_diff = maxSediment - sediment;

        sediment += dt * depositionRate * c_diff;

        float diff = dt * volume * depositionRate * c_diff;
        if (diff > volume)
        {
            diff = volume;
        }
        if (diff < -volume)
        {
            diff = -volume;
        }
        std::cout << "diff: " << diff << std::endl;

        heights[(sideSize * (dropY-1)) + dropX+1] -= diff;
        if (heights[(sideSize * (dropY - 1)) + dropX+1] < 0) heights[(sideSize * dropY) + dropX] = 0;

        volume *= (1.0 - dt * evapRate);
        */
    }
}


float MyGenerator::getHeightAtPoint(std::vector<float>& heights, int x, int y, int sideSize)
{
    return heights[(sideSize * y) + x];
}


std::vector<float> MyGenerator::getSurrounding(std::vector<float>& heights, int x, int y, int sideSize)
{
    std::vector<float> surrounding;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (x + i >= 0 && x + i <= sideSize && y + j >= 0 && y + j <= sideSize) //make sure is not out of bounds
            {
                surrounding.push_back(getHeightAtPoint(heights, x + i, y + j, sideSize));
            }

        }
    }
    return surrounding;
}

Vec3 MyGenerator::crossProduct(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = (a.y * b.z) - (a.z - b.y);
    result.y = -((a.x * b.z) - (a.z * b.x));
    result.z = (a.x * b.y) - (a.y * b.x);

    return result;
}

Vec3 MyGenerator::GetNormal(std::vector<float>& heights, Vec3 point, int sideSize)
{
    float scale = 60;
    //get 3 points around point, maybe N, SE, and SW
    //get N-SE and N-SW vectors
    //get cross product of N-SE and N-SW
    if (point.x <= 0 || point.x >= sideSize || point.y <= 0 || point.y >= sideSize)
    {
        return Vec3(0, 0, 1);
    }
    
    //cardinal directions
    Vec3 n = Vec3((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x+1, point.y, sideSize)) * scale, 1.0, 0.0).Normalize() * 0.15;
    n = n + Vec3((getHeightAtPoint(heights, point.x-1, point.y, sideSize) - getHeightAtPoint(heights, point.x + 1, point.y, sideSize)) * scale, 1.0, 0.0).Normalize() * 0.15;
    n = n + Vec3(0.0, 1.0, (getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x, point.y+1, sideSize)) * scale).Normalize() * 0.15;
    n = n + Vec3(0.0, 1.0, (getHeightAtPoint(heights, point.x, point.y-1, sideSize) - getHeightAtPoint(heights, point.x, point.y, sideSize)) * scale).Normalize() * 0.15;

    //diagonals
    //(getHeightAtPoint(heights, point.x, point.y, sideSize)
    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x + 1, point.y + 1, sideSize)) * scale) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x + 1, point.y + 1, sideSize)) * scale) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x + 1, point.y - 1, sideSize)) * scale) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x + 1, point.y - 1, sideSize)) * scale) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x - 1, point.y + 1, sideSize)) * scale) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x - 1, point.y + 1, sideSize)) * scale) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x - 1, point.y - 1, sideSize)) * scale) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) - getHeightAtPoint(heights, point.x - 1, point.y - 1, sideSize)) * scale) / sqrt(2)).Normalize() * 0.1;


    return n;

    /*
    Vec3 north = Vec3(point.x, point.y-1, getHeightAtPoint(heights, point.x, point.y-1, sideSize));
    Vec3 southEast = Vec3(point.x+1, point.y + 1, getHeightAtPoint(heights, point.x+1, point.y + 1, sideSize));
    Vec3 southWest = Vec3(point.x - 1, point.y + 1, getHeightAtPoint(heights, point.x - 1, point.y + 1, sideSize));

    Vec3 NSE = southEast - north;
    Vec3 NSW = southWest - north;

    Vec3 cross = crossProduct(NSE, NSW); //normalize this to get normal vector
    //if (cross.z < 0) cross = crossProduct(NSW, NSE);
    cross = cross.Normalize();
    
    return cross;*/
}



