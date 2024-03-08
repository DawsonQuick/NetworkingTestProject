#pragma once
#ifndef TILETRANSFORMATIONUTILS_H
#define TILETRANSFORMATIONUTILS_H
#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "./../../../OpenGL/vendor/glm/glm.hpp"

/*
* Structure used to hold all information regarding individual tiles in the Grid
*/
struct TileInfo {
	glm::mat4x3 vertexPos;
	glm::vec2 centerPos;
	glm::vec2 layoutIndex;
	bool isTraversable;

};

/*
* Conversion struct
*/
struct vec2Hash {
	std::size_t operator()(const glm::vec2& v) const {
		// Combine the hash of x and y coordinates
		std::size_t hashX = std::hash<float>{}(v.x);
		std::size_t hashY = std::hash<float>{}(v.y);
		return hashX ^ (hashY << 1); // Example combining hash function
	}
};


// Function to convert a 1D vector to a dynamically allocated 2D array
inline  std::unique_ptr<std::unique_ptr<float[]>[]> vectorTo2DArray(std::vector<TileInfo> MapTileInformation, int numberTileWidth , int numberTileHeight) {
		if (MapTileInformation.size() != ((numberTileWidth) * (numberTileHeight))) {
			std::cout << MapTileInformation.size() << " - " << ((numberTileWidth) * (numberTileHeight)) << std::endl;
			std::cout << "Improper size" << std::endl;
			// Handle error
			return nullptr;
		}

		// Create a unique_ptr for the 2D array
		auto tileArray = std::make_unique<std::unique_ptr<float[]>[]>(numberTileWidth);
		for (int i = 0; i < numberTileWidth; ++i) {
			tileArray[i] = std::make_unique<float[]>(numberTileHeight);
		}

		// Populate the 2D array
		int index = 0;
		for (int i = 0; i < numberTileWidth; ++i) {
			for (int j = 0; j < numberTileHeight; ++j) {
				tileArray[i][j] = (MapTileInformation[index++].isTraversable? 1.0f : 0.0f); //Simple check for a boolean inside each of the TileInfo structures
			}
		}

		return std::move(tileArray);
}



inline void vectorToMap(std::vector<TileInfo>& MapTileInformation, std::unordered_map<glm::vec2, glm::vec2, vec2Hash>& tileMap) {
		
		for (int i = 0; i < MapTileInformation.size(); i++) {
			tileMap[MapTileInformation[i].layoutIndex]=MapTileInformation[i].centerPos;
		}
}

//TODO: Optimize later, if a tile is not traverable , check to see if the neighboring tiles to see if the are also not traversable, if so,
//then the edge that is facing the other non-traversable tile is benign . So dont add it to the list.
// 
//TLDR , prune all edges that are not outward facing
inline std::vector<glm::mat4x3> generateBlockTileWalls(std::vector<TileInfo> MapTileInformation) {
	std::vector<glm::mat4x3> returnVector;

	for (int i = 0; i < MapTileInformation.size(); i++) {
		if (!MapTileInformation[i].isTraversable) {
			returnVector.push_back(MapTileInformation[i].vertexPos);
		}
	}

	return returnVector;

}


/*
* Create a vertex array of all current non-traversable tiles to be rended onto the screen as a visual representation
*/
inline std::vector<float> generateBlockedTileVertexArray(std::vector<TileInfo> MapTileInformation , glm::vec4 color,
	std::vector<float>& vertexArray, std::vector<unsigned int>& indiceArray , float sideLength) {
	unsigned int index = 0;
	float half = (sideLength / 2.0f) - 1.0f;
	for (int i = 0; i < MapTileInformation.size(); i++) {
		if (!MapTileInformation[i].isTraversable) {

			glm::mat4x3 tmpMap = MapTileInformation[i].vertexPos;

			vertexArray.push_back(tmpMap[0][0] - half);
			vertexArray.push_back(tmpMap[0][1] - half);
			vertexArray.push_back(tmpMap[0][2]);
			vertexArray.push_back(color.r);
			vertexArray.push_back(color.g);
			vertexArray.push_back(color.b);
			vertexArray.push_back(color.w);
			vertexArray.push_back(tmpMap[1][0] - half);
			vertexArray.push_back(tmpMap[1][1] - half);
			vertexArray.push_back(tmpMap[1][2]);
			vertexArray.push_back(color.r);
			vertexArray.push_back(color.g);
			vertexArray.push_back(color.b);
			vertexArray.push_back(color.w);
			vertexArray.push_back(tmpMap[2][0] - half);
			vertexArray.push_back(tmpMap[2][1] - half);
			vertexArray.push_back(tmpMap[2][2]);
			vertexArray.push_back(color.r);
			vertexArray.push_back(color.g);
			vertexArray.push_back(color.b);
			vertexArray.push_back(color.w);
			vertexArray.push_back(tmpMap[3][0] - half);
			vertexArray.push_back(tmpMap[3][1] - half);
			vertexArray.push_back(tmpMap[3][2]);
			vertexArray.push_back(color.r);
			vertexArray.push_back(color.g);
			vertexArray.push_back(color.b);
			vertexArray.push_back(color.w);

			indiceArray.push_back(index);
			indiceArray.push_back(index + (unsigned int)1);
			indiceArray.push_back(index + (unsigned int)2);
			indiceArray.push_back(index + (unsigned int)2);
			indiceArray.push_back(index + (unsigned int)3);
			indiceArray.push_back(index);

			index += (unsigned int)4;

		}
	}

	return vertexArray;
}


#endif