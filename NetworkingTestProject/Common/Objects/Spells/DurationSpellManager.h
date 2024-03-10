#pragma once
#ifndef DURATIONSPELLMANAGER_H
#define DURATIONSPELLMANAGER_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <limits>
#include <algorithm>
#include <execution>
#include <mutex>
#include <functional>
#include "./../../../OpenGL/vendor/glm/glm.hpp"
#include "./../../Enums/SpellType.h"

struct DurationSpellInformation {
	std::string spellCastersName;
	std::string spellName;
	SpellType type;
	float duration;
	float updateRate;
	float currentDeltaTime;
	float sideLength;
	glm::vec2 spawnPoint;
	std::function<void(float,float,std::string)> callback;

	// Define the equality operator
	bool operator==(const DurationSpellInformation& other) const {
		return duration == other.duration && updateRate == other.updateRate;
		// You may need to include comparison for the callback as well,
		// depending on how you want to compare the objects.
	}
};






class DurationSpellManager {
private:

	void drawSquare(float centerX, float centerY, float sideLength, std::vector<float>& vertices) {
		// Calculate half the side length
		float halfLength = sideLength;

		// Define the vertices of the square as line segments
		float verticesArray[] = {
			centerX - halfLength, centerY - halfLength, 0.0f, // Bottom-left
			centerX + halfLength, centerY - halfLength, 0.0f, // Bottom-right

			centerX + halfLength, centerY - halfLength, 0.0f, // Bottom-right
			centerX + halfLength, centerY + halfLength, 0.0f, // Top-right

			centerX + halfLength, centerY + halfLength, 0.0f, // Top-right
			centerX - halfLength, centerY + halfLength, 0.0f, // Top-left

			centerX - halfLength, centerY + halfLength, 0.0f, // Top-left
			centerX - halfLength, centerY - halfLength, 0.0f, // Bottom-left
		};

		// Add the vertices to the buffer
		vertices.insert(vertices.end(), std::begin(verticesArray), std::end(verticesArray));
	}
	std::vector<float> durationSpellRangeVertexArray;
	std::mutex VertexArrayMutex;
	std::vector<DurationSpellInformation> durationSpellCollection;

	DurationSpellManager() {}
	// Delete copy constructor and assignment operator
	DurationSpellManager(const DurationSpellManager&) = delete;
	DurationSpellManager& operator=(const DurationSpellManager&) = delete;


public:

	// Singleton Constructor
	static DurationSpellManager& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static DurationSpellManager instance;
		return instance;
	}

	void onUpdate(float deltaTime) {
		std::vector<DurationSpellInformation> expiredSpells;
		for (DurationSpellInformation& durationSpell : durationSpellCollection) {
			if (durationSpell.duration - deltaTime < 0) {
				std::cout << "Duration Spell has Expired removing" << std::endl;
				expiredSpells.push_back(durationSpell);
				continue;
			}
			else {
				durationSpell.duration = durationSpell.duration - deltaTime;
				durationSpell.currentDeltaTime = durationSpell.currentDeltaTime + deltaTime;
				if (durationSpell.currentDeltaTime >= durationSpell.updateRate) {
					std::cout << "Spell interval triggered calling callback function for spell" << std::endl;
					glm::vec2 tmp = durationSpell.spawnPoint;
					float x = tmp.x;
					float y = tmp.y;
					durationSpell.callback(x,y, durationSpell.spellCastersName);
					durationSpell.currentDeltaTime = 0.0f;
				}
			}
		
		}

		for (DurationSpellInformation  expiredSpell : expiredSpells) {
			expiredSpell.callback(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), expiredSpell.spellCastersName);
			durationSpellCollection.erase(std::remove(durationSpellCollection.begin(), durationSpellCollection.end(), expiredSpell), durationSpellCollection.end());
		}
		std::lock_guard<std::mutex> lock(VertexArrayMutex);
		durationSpellRangeVertexArray.clear();
		for (DurationSpellInformation& durationSpell : durationSpellCollection) {
			drawSquare(durationSpell.spawnPoint.x, durationSpell.spawnPoint.y, durationSpell.sideLength, durationSpellRangeVertexArray);
		}

	}

	//Add a player to the database
	void addSpell(std::string playerName, std::string spellName, SpellType type, float duration, float updateRate, float sideLength, glm::vec2 spawnPoint, std::function<void(float, float, std::string)> Tmpcallback) {
		DurationSpellInformation newInfo;
		newInfo.spellCastersName = playerName;
		newInfo.spellName = spellName;
		newInfo.type = type;
		newInfo.duration = duration;
		newInfo.updateRate = updateRate;
		newInfo.callback = Tmpcallback;
		newInfo.spawnPoint = spawnPoint;
		newInfo.sideLength = sideLength;
		newInfo.currentDeltaTime = 0.0f;
		std::cout << "Adding Duration Spell to Manager" << std::endl;
		durationSpellCollection.push_back(newInfo);
	}

	std::vector<float> getDurationSpellVertexArray() {
		std::lock_guard<std::mutex> lock2(VertexArrayMutex);
		return durationSpellRangeVertexArray;
	}

	void removeConcentrationSpell(std::string playerName, std::string spellName) {
		DurationSpellInformation spellToRemove;
		bool foundSpell = false;
		for (DurationSpellInformation spellInformation : durationSpellCollection) {
			if (spellInformation.spellCastersName == playerName
				&& spellInformation.spellName == spellName
				&& spellInformation.type == SpellType::CONCENTRATION) {
				spellToRemove = spellInformation;
				foundSpell = true;
				break;
			}
		}

		if (foundSpell) {
			durationSpellCollection.erase(std::remove(durationSpellCollection.begin(), durationSpellCollection.end(), spellToRemove), durationSpellCollection.end());
		}
	}

};
#endif