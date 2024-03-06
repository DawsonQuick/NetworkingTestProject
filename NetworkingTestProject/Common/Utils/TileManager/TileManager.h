#pragma once
#ifndef TILEMANAGER_H
#define TILEMANAGER_H
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include "./../../../OpenGL/vendor/glm/glm.hpp"
#include "./../../../OpenGL/vendor/glm/vec2.hpp"
#include "AStartPathFinding.h"

struct TileInfo {
	glm::mat4x3 vertexPos;
	glm::vec2 centerPos;
	glm::vec2 layoutIndex;
	bool isTraversable;

};
struct vec2Hash {
	std::size_t operator()(const glm::vec2& v) const {
		// Combine the hash of x and y coordinates
		std::size_t hashX = std::hash<float>{}(v.x);
		std::size_t hashY = std::hash<float>{}(v.y);
		return hashX ^ (hashY << 1); // Example combining hash function
	}
};


class TileManager {
private:

	std::vector<TileInfo> MapTileInformation;
	TileInfo currentlyHoveredTile;
	TileInfo currentPlayerTile;

	int numberTileWidth;
	int numberTileHeight;
	std::unique_ptr<std::unique_ptr<float[]>[]> tileArray;
	std::unordered_map<glm::vec2, glm::vec2, vec2Hash> tileMap;

	TileManager() {
	}

	// Delete copy constructor and assignment operator
	TileManager(const TileManager&) = delete;
	TileManager& operator=(const TileManager&) = delete;


	// Function to convert a 1D vector to a dynamically allocated 2D array
	std::unique_ptr<std::unique_ptr<float[]>[]> vectorTo2DArray() {
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
				tileArray[i][j] = (MapTileInformation[index++].isTraversable? 1.0f : 0.0f);
			}
		}

		return std::move(tileArray);
	}


	void vectorToMap() {
		
		for (int i = 0; i < MapTileInformation.size(); i++) {
			tileMap[MapTileInformation[i].layoutIndex]=MapTileInformation[i].centerPos;
		}

	}


public:
	// Singleton Constructor
	static TileManager& getInstance() {
		static TileManager instance;
		return instance;
	}
	void setMapTileInformation(std::vector<TileInfo> tileInformation) {
		MapTileInformation = tileInformation;
		tileArray = vectorTo2DArray();
		vectorToMap();
	}

	std::vector<TileInfo> getMapTileInformation() {
		return MapTileInformation;
	}

	void setCurrentlyHoveredTile(TileInfo tile) {
		currentlyHoveredTile = tile;
	}

	TileInfo getCurrentlyHoveredTile() {
		return currentlyHoveredTile;
	}

	void updateTileMapInformation(float indexX, float indexY, bool isTraversable) {
		for (int i = 0; i < MapTileInformation.size(); i++) {
			if ((MapTileInformation[i].layoutIndex.x == indexX) && (MapTileInformation[i].layoutIndex.y == indexY)) {
				MapTileInformation[i].isTraversable = isTraversable;
			}
		}
		//Regenerate Tile ArrayMap
		tileArray = vectorTo2DArray();

	}


	void setCurrentlPlayerTile(TileInfo tile) {
		currentPlayerTile = tile;
	}

	TileInfo getCurrentlPlayerTile() {
		return currentPlayerTile;
	}
	
	void setTileDimemsions(int width, int height) {
		numberTileHeight = height - 1;
		numberTileWidth = width - 1;
	}

	std::vector<float> getA_Starpath() {

		std::vector<float> lineToCursor;
		// Source is the left-most bottom-most corner
		Pair src = make_pair(15, 15);

		// Destination is the left-most top-most corner
		Pair dest = make_pair((currentlyHoveredTile.layoutIndex.x - currentPlayerTile.layoutIndex.x + 15), (currentlyHoveredTile.layoutIndex.y - currentPlayerTile.layoutIndex.y + 15));
		float tmp[30][30];

		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				//Partially working , illiegal memory access when the player gets too far on the -plane . But progress
				if (((currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15)) < 0) || ((currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15)) < 0)) {
					tmp[i][j] = 0.0f;
				}
				else if (((currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15)) > numberTileWidth-1) || ((currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15)) > numberTileHeight-1)) {
					tmp[i][j] = 0.0f;
				}
				else {
					tmp[i][j] = tileArray[(int)(currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15))][(int)(currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15))];
				}
			}
		}

		stack<Pair> Path = aStarSearch(tmp, src, dest);
		auto it = tileMap.find(glm::vec2(currentPlayerTile.layoutIndex.x, currentPlayerTile.layoutIndex.y));

		float prevX = it->second.x;
		float prevY = it->second.y;
		while (!Path.empty()) {
			pair<int, int> p = Path.top();
			Path.pop();
			glm::vec2 layoutIndexToFind((float)((p.first - 15) + currentPlayerTile.layoutIndex.x), (float)((p.second - 15) + currentPlayerTile.layoutIndex.y));
			// Print out layoutIndexToFind for debugging
			//std::cout << "layoutIndexToFind: (" << layoutIndexToFind.x << ", " << layoutIndexToFind.y << ")" << std::endl;
			
			// Check if the key exists in tileMap
			auto it = tileMap.find(layoutIndexToFind);
			if (it != tileMap.end()) {
				glm::vec2 centerPos = it->second;
				lineToCursor.push_back((float)prevX);
				lineToCursor.push_back((float)prevY);
				lineToCursor.push_back(0.0f);

				lineToCursor.push_back((float)centerPos.x);
				lineToCursor.push_back((float)centerPos.y);
				lineToCursor.push_back(0.0f);

				prevX = centerPos.x;
				prevY = centerPos.y;
			}
			else {
				printf("Key not found in tileMap\n");
				// Handle the case when the key is not found
			}
		}
		
		return std::move(lineToCursor);
	}

};

#endif
