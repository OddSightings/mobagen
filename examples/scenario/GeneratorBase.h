#ifndef SCENARIOGENERATORBASE_H
#define SCENARIOGENERATORBASE_H
#include <ColorT.h>
#include <string>
#include <iostream>
#include <vector>



struct Vec2
{
public:
	float x, y;
	Vec2() = default;
	Vec2(float nx, float ny)
	{
		x = nx;
		y = ny;
	}



	Vec2 operator*(const float& other) const
	{
		return Vec2(this->x * other, this->y * other);
	}

	Vec2 operator/(const float& other) const
	{
		return Vec2(this->x / other, this->y / other);
	}
	bool operator==(const Vec2& other) const
	{
		return (this->x == other.x && this->y == other.y);
	}

};

struct Vec3
{
public:

	Vec3() = default;
	Vec3(float nx, float ny, float nz)
	{
		x = nx;
		y = ny;
		z = nz;
	}
	float x, y, z;

	Vec3 operator-(const Vec3& other) const
	{
		return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	Vec3 operator+(const Vec3& other) const
	{
		return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	Vec3 operator*(const float& other) const
	{
		return Vec3(this->x * other, this->y * other, this->z * other);
	}

	Vec3 operator/(const float& other) const
	{
		return Vec3(this->x / other, this->y / other, this->z / other);
	}

	Vec3 Normalize()
	{
		Vec3* base = this;
		float len = sqrt((base->x * base->x) + (base->y * base->y) + (base->z * base->z)); //magnitude of the vector

		if (len != 0)
		{
			base->x /= len;
			base->y /= len;
			base->z /= len;
		}
		return *base;
	}


	void print()
	{
		std::cout << "x: " << this->x << ", y: " << this->y << ", z: " << this->z << std::endl;
	}

};


class ScenarioGeneratorBase {
 public:
  virtual std::vector<Color32> Generate(int sideSize, float displacement=0)=0;
  virtual std::string GetName()=0;
};

#endif  // SCENARIOGENERATORBASE_H
