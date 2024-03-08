#pragma once
#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <string>
#include <map>

#include <typeindex>
#include "./../../../OpenGL/vendor/glm/vec2.hpp"
#include "AStartPathFinding.h"
#include "TileTranformationUtils.h"




class TileManager {
private:

	std::vector<TileInfo> MapTileInformation;
	TileInfo currentlyHoveredTile;
	TileInfo currentPlayerTile;
	std::vector<glm::mat2x2> blockTileWalls;
	std::vector<float> blockedTileVectorArray;
	std::vector<unsigned int> blockedTileIndicesArray;
	float tileSize;
	int numberTileWidth;
	int numberTileHeight;
	std::unique_ptr<std::unique_ptr<float[]>[]> tileArray;
	std::unordered_map<glm::vec2,TileInfo, vec2Hash> tileMap;

	TileManager() {
	}

	// Delete copy constructor and assignment operator
	TileManager(const TileManager&) = delete;
	TileManager& operator=(const TileManager&) = delete;

public:
	// Singleton Constructor
	static TileManager& getInstance() {
		static TileManager instance;
		return instance;
	}

/*-----------------------------------------------------------------------------------------------------------------------*
 *														GETTERS															 *		
 *-----------------------------------------------------------------------------------------------------------------------*/
	std::vector<TileInfo> getMapTileInformation() {
		return MapTileInformation;
	}
	TileInfo getCurrentlyHoveredTile() {
		return currentlyHoveredTile;
	}
	TileInfo getCurrentlPlayerTile() {
		return currentPlayerTile;
	}
	std::vector<glm::mat2x2> getBlockedTileWalls() {
		return blockTileWalls;
	}
	std::vector<float> getBlockedTilesVertexArray() {
		return blockedTileVectorArray;
	}
	std::vector<unsigned int> getBlockedTileIndiceArray() {
		return blockedTileIndicesArray;
	}
/*-----------------------------------------------------------------------------------------------------------------------*
 *														SETTERS															 *
 *-----------------------------------------------------------------------------------------------------------------------*/

	void setMapTileInformation(std::vector<TileInfo> tileInformation) {
		MapTileInformation = tileInformation;
		tileArray = vectorTo2DArray(MapTileInformation, numberTileWidth, numberTileHeight);
		vectorToMap(MapTileInformation, tileMap);
	}
	void setCurrentlyHoveredTile(TileInfo tile) {
		currentlyHoveredTile = tile;
	}
	void setCurrentlPlayerTile(TileInfo tile) {
		currentPlayerTile = tile;
	}
	void setTileDimemsions(int width, int height) {
		numberTileHeight = height;
		numberTileWidth = width;
	}
	void setSideLength(float scale) {
		tileSize = scale;
	}
/*-----------------------------------------------------------------------------------------------------------------------*
 *												 SUPPORTING FUNCTIONS												     *
 *-----------------------------------------------------------------------------------------------------------------------*/

	/*
	* When a tile has a state update need to regenerate all the different transmutations of the tile information
	*/
	void updateTileMapInformation(float indexX, float indexY) {
		for (int i = 0; i < MapTileInformation.size(); i++) {
			if ((MapTileInformation[i].layoutIndex.x == indexX) && (MapTileInformation[i].layoutIndex.y == indexY)) {
				//If Tile is currently traversable, then toggle it to non-traversable.
				//If Tile is currently non-traversable, then toggle to traversable	
				MapTileInformation[i].isTraversable = MapTileInformation[i].isTraversable? false : true;
			}
		}
		//Regenerate Tile ArrayMap (Execution order is important here)
		tileArray = vectorTo2DArray(MapTileInformation, numberTileWidth, numberTileHeight);
		vectorToMap(MapTileInformation, tileMap);
		blockTileWalls = generateBlockTileWalls(numberTileWidth, numberTileHeight, tileMap, MapTileInformation);
		regenerateBlockTileRenderInformation();
	}

	/*
	* Update all the transmutations of the Tile Information
	*/
	void regenerateBlockTileRenderInformation() {
		blockedTileVectorArray.clear();
		blockedTileIndicesArray.clear();
		generateBlockedTileVertexArray(MapTileInformation, glm::vec4(1.0f, 0.0f, 0.0f, 0.3f), blockedTileVectorArray, blockedTileIndicesArray, tileSize);
	}

	/*
	*  A* Search Algorithm for player movement based off of the current TileInfo for the board
	*/
	std::vector<float> getA_Starpath() {

		std::vector<float> lineToCursor;

		Pair src = make_pair(15, 15);
		Pair dest = make_pair((currentlyHoveredTile.layoutIndex.x - currentPlayerTile.layoutIndex.x + 15), (currentlyHoveredTile.layoutIndex.y - currentPlayerTile.layoutIndex.y + 15));
		
		//Since array indexs cannot be negative , we need to translate the players current position in the world to translate over
		float tmp[30][30];
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				//Check to see if the translations ever goes negative (Player has hit the left or bottom edges of the world grid), if so then set the value to 0
				if (((currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15)) < 0) || ((currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15)) < 0)) {
					tmp[i][j] = 0.0f;
				}
				//Check to see if the translations ever goes over the maximum valid positive value (Player has hit the right or top edges of the world grid), if so then set the value to 0
				else if (((currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15)) > numberTileWidth-1) || ((currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15)) > numberTileHeight-1)) {
					tmp[i][j] = 0.0f;
				}
				//If neither of the two above checks are executed , then the space is a valid location to look up inside the 2d array of values
				else {
					tmp[i][j] = tileArray[(int)(currentPlayerTile.layoutIndex.x + (numberTileWidth / 2) + (i - 15))][(int)(currentPlayerTile.layoutIndex.y + (numberTileHeight / 2) + (j - 15))];
				}
			}
		}

		//Preform the A* search algorithm and return the resulting path
		stack<Pair> Path = aStarSearch(tmp, src, dest);

		//Get the players current position in the world space
		auto it = tileMap.find(glm::vec2(currentPlayerTile.layoutIndex.x, currentPlayerTile.layoutIndex.y));

		//Since the posistions inside the A* Search algorithm were translated to a positive value between 0-30 , we need to translate them back to world positions to properly map to the grid
		float prevX = it->second.centerPos.x;
		float prevY = it->second.centerPos.y;
		while (!Path.empty()) {
			pair<int, int> p = Path.top();
			Path.pop();
			glm::vec2 layoutIndexToFind((float)((p.first - 15) + currentPlayerTile.layoutIndex.x), (float)((p.second - 15) + currentPlayerTile.layoutIndex.y));
			
			// Check if the key exists in tileMap
			auto it = tileMap.find(layoutIndexToFind);
			if (it != tileMap.end()) {
				glm::vec2 centerPos = it->second.centerPos;
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
