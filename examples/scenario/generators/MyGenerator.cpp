#include "MyGenerator.h"
#include "../PerlinNoise.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

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
      

  //Blur(heights, sideSize);
  //modifications end

  for (int x = 0; x < sideSize; x++)
  {
      for (int y = 0; y < sideSize; y++)
      {
          float m = getHeightAtPoint(heights, x, y, sideSize) * 255;
          Color32 c;
          if (blackout)
          {
              c = Color32(m, m, m);
          }
          else if (m <= water)
          {
              c = Color32(0, 0, 180);
          }
          else if (m <= beach)
          {
              c = Color32(194, 178, 128);
          }
          else if (m <= grass)
          {
              c = Color32(0, 180, 0);
          }
          else if (m <= mountain)
          {
              c = Color32(63, 38, 29);
          }
          else
          {
              c = Color32(255, 255, 255);
          }
          colors.emplace_back(c);
      }

  }

  std::cout<<colors.size() << std::endl;
  return colors;
}


std::string MyGenerator::GetName() { return "Tommy's"; }

//https://jobtalle.com/simulating_hydraulic_erosion.html
//https://nickmcd.me/2020/04/10/simple-particle-based-hydraulic-erosion/

void MyGenerator::Erode(std::vector<float>& heights, int sideSize) //this will be run in a for loop in Generate, will be called maxEroders times
{
    //create waater droplet at random point in map
    int dropX = rand() % (sideSize);
    int dropY = rand() % (sideSize);

    //used.push_back(Vec2(dropX, dropY));
    int ox = 0;
    int oy = 0;

    //will be used for previous position of droplet
    int xp = dropX;
    int yp = dropY;

    //storing velocity
    float vx = 0;
    float vy = 0;
    //fraction of volume that is sediment
    float sediment = 0;

    //friction of droplet (using standard for all terrain)
    float friction = 0.1;

    float depositionRate = 0.1;

    float erosionRate = 0.4;

    float maxChange = 0.01;

    float iterationScale = 0.04;

    //while(volume >= minVol)
    for(int i = 0; i < sideSize / 3; i++)
    {        
        
        dropX += ox;
        dropY += oy;

        if (dropX >= sideSize || dropY >= sideSize || dropX <= 0 || dropY <= 0)
        {
            break; //out of bounds
        }
        
        Vec3 normal = GetNormal(heights, Vec3(dropX, dropY, getHeightAtPoint(heights, dropX,dropY, sideSize)), sideSize);
        //normal.print();
        if (normal.z == 1)
        {
            break; //surface is flat
        }
        
        float deposit = sediment * depositionRate * normal.z;
        float erosionCount = erosionRate * (1 - normal.z);

       // std::cout << normal.z << std::endl;

        float heightChange = deposit - erosionCount;
        if (heightChange > maxChange) heightChange = maxChange;
        if (heightChange < -maxChange) heightChange = -maxChange;

        float sedimentChange = erosionCount - deposit;

        heights[(sideSize * xp) + yp] -= heightChange;
        sediment += sedimentChange;
       // std::cout << sediment << std::endl;
        if (sediment > 1) sediment = 1;
        if (sediment < 0) sediment = 0;

        float speed = 1;

        vx = friction * vx + normal.x * speed;
        vy = friction * vy + normal.y * speed;

        xp = dropX;
        yp = dropY;
        dropX += vx;
        dropY += vy;

        ox = 1 - rand() % 3;
        oy = 1 - rand() % 3;
    }
}


Color32 MyGenerator::ColorLerp(Color32 a, Color32 b, float f)
{
    Color32 newCol = Color32(
        (b.r - a.r) * f + a.r,
        (b.g - a.g) * f + a.g,
        (b.b - a.b) * f + a.b
    );
    return newCol;
}

void MyGenerator::Blur(std::vector<float>& heights, int sideSize)
{
    //does not work
    for (int x = 1; x < sideSize; x++)
    {
        for (int y = 1; y < sideSize; y++)
        {
            float f = (
                getHeightAtPoint(heights, x + 1, y, sideSize) +
                getHeightAtPoint(heights, x, y + 1, sideSize) +
                getHeightAtPoint(heights, x, y - 1, sideSize) +
                getHeightAtPoint(heights, x - 1, y, sideSize)
                ) * 0.125;
            f += (
                getHeightAtPoint(heights, x + 1, y + 1, sideSize) +
                getHeightAtPoint(heights, x - 1, y + 1, sideSize) +
                getHeightAtPoint(heights, x + 1, y - 1, sideSize) +
                getHeightAtPoint(heights, x - 1, y - 1, sideSize)
                ) * .0625;
            f += getHeightAtPoint(heights, x, y, sideSize) * .25;

            heights[y + (sideSize * x)] = f;
        }
    }
}

float MyGenerator::getHeightAtPoint(std::vector<float>& heights, int x, int y, int sideSize)
{
    return heights[(sideSize * x) + y];
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
    if (point.x <= 0 || point.x >= sideSize || point.y <= 0 || point.y >= sideSize)
    {
        return Vec3(0, 0, 1);
    }
    
    //cardinal directions
    Vec3 n = Vec3((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x + 1, point.y, sideSize) * 100), 1.0, 0.0).Normalize() * 0.15;
    n = n + Vec3((getHeightAtPoint(heights, point.x - 1, point.y, sideSize)*100 - getHeightAtPoint(heights, point.x, point.y, sideSize) * 100), 1.0, 0.0).Normalize() * 0.15;
    n = n + Vec3(0.0, 1.0, (getHeightAtPoint(heights, point.x, point.y, sideSize)*100 - getHeightAtPoint(heights, point.x, point.y + 1, sideSize)*100)).Normalize() * 0.15;
    n = n + Vec3(0.0, 1.0, (getHeightAtPoint(heights, point.x, point.y - 1, sideSize)*100 - getHeightAtPoint(heights, point.x, point.y, sideSize)*100)).Normalize() * 0.15;
    
    //diagonals

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x + 1, point.y + 1, sideSize) * 100)) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x + 1, point.y + 1, sideSize) * 100)) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x + 1, point.y - 1, sideSize) * 100)) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x + 1, point.y - 1, sideSize) * 100)) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x - 1, point.y + 1, sideSize) * 100)) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x - 1, point.y + 1, sideSize) * 100)) / sqrt(2)).Normalize() * 0.1;

    n = n + Vec3(((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x - 1, point.y - 1, sideSize) * 100)) / sqrt(2),
        sqrt(2),
        ((getHeightAtPoint(heights, point.x, point.y, sideSize) * 100 - getHeightAtPoint(heights, point.x - 1, point.y - 1, sideSize) * 100)) / sqrt(2)).Normalize() * 0.1;
        
    Vec3 m = Vec3(n.x, n.z, n.y);
    return m.Normalize();
    




}