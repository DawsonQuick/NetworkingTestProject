#pragma once
#include <vector>
#include <string>
#include "./../../Utils/stb_image.h"
#include <execution>
#include <algorithm>
typedef struct {
    float x, y;
} vector2;
class PerlinNoiseGeneration{
public:
	PerlinNoiseGeneration(int width, int height, float* verticesArray, unsigned int* indicesArray, unsigned int seed);
	~PerlinNoiseGeneration();
	vector2 randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float interpolate(float a0, float a1, float w);
	float fade(float t);
	float perlin(float x, float y, float frequency, float amplitude);
	void processElement(unsigned int i, unsigned int j, int width, int height, unsigned int* indicesArray,
		float* verticesArray, int& vertexIndex);
private:
	unsigned int m_Seed;
};