#pragma once
#ifndef MAINGAMELOOP_H
#define MAINGAMELOOP_H
#include <iostream>
#include <map>

class MainGameLoop {
private:

	MainGameLoop() {}
	// Delete copy constructor and assignment operator
	MainGameLoop(const MainGameLoop&) = delete;
	MainGameLoop& operator=(const MainGameLoop&) = delete;
	std::vector<std::string> playerListing;
public:

	// Singleton Constructor
	static MainGameLoop& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static MainGameLoop instance;
		return instance;
	}

	void executeInitialtive() {
		//For each player in the database have them roll for initiative and then sort them by there roll
		for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
			playerListing.push_back(player.first);
		}
		PlayerDatabase::getInstance().getPlayer(playerListing.at(0)).setIsTurnReady(true); //Start the first player in the List
	}


	void checkGameState() {
		//This will be called from the main render loop on update. This will keep track of the state of the game.
		//In other words, who is currently playing , update all their specific details like if they are currently casting a duration spell, need to update time spell has left
		int index = 0;
		for (std::string name : playerListing) {
			if (PlayerDatabase::getInstance().getPlayer(name).getIsTurnReady()) {
				if (PlayerDatabase::getInstance().getPlayer(name).getIsTurnComplete()) {
					PlayerDatabase::getInstance().getPlayer(name).setIsTurnReady(false);
					PlayerDatabase::getInstance().getPlayer(name).setIsTurnComplete(false);

					if (playerListing.size() > index) {
						PlayerDatabase::getInstance().getPlayer(playerListing.at(index + 1)).setIsTurnReady(true);
					}
					else {
						PlayerDatabase::getInstance().getPlayer(playerListing.at(0)).setIsTurnReady(true); //Start the first player in the List
					}
				}
			}
			index++;
		}
	}




};
#endif