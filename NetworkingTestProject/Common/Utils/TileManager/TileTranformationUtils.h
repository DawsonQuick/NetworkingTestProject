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



inline void vectorToMap(std::vector<TileInfo>& MapTileInformation, std::unordered_map<glm::vec2, TileInfo, vec2Hash>& tileMap) {
		
		for (int i = 0; i < MapTileInformation.size(); i++) {
			tileMap[MapTileInformation[i].layoutIndex]=MapTileInformation[i];
		}
}


inline std::vector<glm::mat2x2> generateBlockTileWalls(int width, int height, std::unordered_map<glm::vec2, TileInfo, vec2Hash> tileMap , std::vector<TileInfo> MapTileInformation) {
	std::vector<glm::mat2x2> returnVector;

	int tmpCounter = 0;
	for (int i = 0; i < MapTileInformation.size(); i++) {
		if (!MapTileInformation[i].isTraversable) {
			glm::mat4x3 tmp = MapTileInformation[i].vertexPos;
			glm::vec2 index = MapTileInformation[i].layoutIndex;


	/*-----------------------------------------------------------------------------------------------*
	 *									Simple culling check										 *
	 *-----------------------------------------------------------------------------------------------*/
	// For each Tile we check to see if any of the adjacent tiles are also non-traversable.
	// If the tile is non-traversable, we dont need to render the side facing that direction
	// This allows only outward facing sides to be added, reducing the size of the vector

			auto it = tileMap.find(glm::vec2(index.x-1, index.y)); //Check left
			if (it != tileMap.end()) {
				if (it->second.isTraversable) {
			        glm::mat2x2 line = { tmp[3][0],tmp[3][1],tmp[0][0],tmp[0][1]};
					returnVector.push_back(line);
				}
			}

			it = tileMap.find(glm::vec2(index.x + 1, index.y)); //Check right
			if (it != tileMap.end()) {
				if (it->second.isTraversable) {
					glm::mat2x2 line = { tmp[1][0],tmp[1][1],tmp[2][0],tmp[2][1]};
					returnVector.push_back(line);
				}
			}

			it = tileMap.find(glm::vec2(index.x, index.y-1)); //Check below
			if (it != tileMap.end()) {
				if (it->second.isTraversable) {
					glm::mat2x2 line = { tmp[0][0],tmp[0][1],tmp[1][0],tmp[1][1]};
					returnVector.push_back(line);
				}
			}

			it = tileMap.find(glm::vec2(index.x, index.y+1)); //Check above
			if (it != tileMap.end()) {
				if (it->second.isTraversable) {
					glm::mat2x2 line = { tmp[2][0],tmp[2][1],tmp[3][0],tmp[3][1]};
					returnVector.push_back(line);
				}
			}
	//TODO: Could optimize futher by setting a single line if tiles aline on same axis
	/*----------------------------------------------------------------------------------------------*/
			tmpCounter++;
		}
	}
	std::cout << "Generation of non-traversable walls, number of entries in the mat2x2 vector: " << returnVector.size() << " for: " << tmpCounter << " entries." << std::endl;
	return returnVector;

}


/*
* Create a vertex array of all current non-traversable tiles to be rended onto the screen as a visual representation
*/
inline std::vector<float> generateBlockedTileVertexArray(std::vector<TileInfo> MapTileInformation , glm::vec4 color,
	std::vector<float>& vertexArray, std::vector<unsigned int>& indiceArray , float sideLength) {
	unsigned int index = 0;
	float half = (sideLength / 2.0f) - 1.0f;
	int totalNumberofNonTraversableTiles = 0;
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
			totalNumberofNonTraversableTiles++;
		}
	}
	std::cout << "Non-Traversable Tile Vertex Array Size: " << vertexArray.size() << " for "<< totalNumberofNonTraversableTiles << " tiles " << std::endl;
	return vertexArray;
}


#endif