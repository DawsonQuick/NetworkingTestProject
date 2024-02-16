#pragma once
#include <vector>
#include <string>
#include <execution>
#include <algorithm>
#include <memory>
typedef struct {
    float x, y;
} vector22;
class PerlinNoiseTessellation{
public:
	PerlinNoiseTessellation(int m_rez, int width, int height, float* vertices, float* heightMap, float* derivativeMap ,unsigned int seed);
	~PerlinNoiseTessellation();
	vector22 randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float interpolate(float a0, float a1, float w);
	float fade(float t);
	float perlin(float x, float y, float frequency, float amplitude);
private:
	unsigned int m_Seed;
};