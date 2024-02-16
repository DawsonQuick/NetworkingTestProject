#pragma once
#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include "./../Objects/Player.h"

class PlayerDatabase {
private:
	std::map<std::string, Player> playerMap;
	PlayerDatabase() {}

	// Delete copy constructor and assignment operator
	PlayerDatabase(const PlayerDatabase&) = delete;
	PlayerDatabase& operator=(const PlayerDatabase&) = delete;

public:

	// Singleton Constructor
	static PlayerDatabase& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static PlayerDatabase instance;
		return instance;
	}

	//Add a player to the database
	void addPlayer(std::string name, Player player) {
		auto it = playerMap.find(name);
		if (it != playerMap.end()) {
			std::cout << "Player: " << name << " already exist in database!" << std::endl;
		}
		else {
			std::cout << "Adding player: " << name << " to the database!" << std::endl;
			playerMap[name] = player;
		}
		
	}

	//remove player from the database
	void removePlayer(std::string name) {
		auto it = playerMap.find(name);
		if (it != playerMap.end()) {
			std::cout << "Removing player: " << name << " from the database!" << std::endl;
			playerMap.erase(name);
		}
		else {
			std::cout << "Player: " << name << " does not exist in map to be removed!" << std::endl;
		}
	}

	Player& getPlayer(std::string name) {
		auto it = playerMap.find(name);
		if (it != playerMap.end()) {
			return it->second; // Return reference to the player if found
		}

	}

	//Print all current users in the database
	void printCurrentPlayers() {
		for (auto const& entry : playerMap) {
			std::cout << entry.first << std::endl;
		}
	}

	std::map<std::string,Player> getPlayers() {
		return playerMap;
	}

};
#endif