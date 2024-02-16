#pragma once
#include <vector>
#include <string>
#include "./../../Utils/stb_image.h"
class TerrainBuilder {
public:
	TerrainBuilder(unsigned int m_rez, int *width, int *height,  std::vector<float> *vertices, const std::string& heightMap, int heightMapID, const std::string& textureMap, int textureMapId);
	~TerrainBuilder();
};