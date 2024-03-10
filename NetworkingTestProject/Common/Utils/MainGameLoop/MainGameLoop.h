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
		for (std::string name : playerListing) {
			if (PlayerDatabase::getInstance().getPlayer(name).getIsTurnReady()) {
				if (PlayerDatabase::getInstance().getPlayer(name).getIsTurnComplete()) {

					if (PlayerDatabase::getInstance().getPlayer(name).isPlayerCurrentlyConcentrating()) {
						int roundsLeft = DurationSpellManager::getInstance().updatePlayerConcentrationSpellDuration(name, PlayerDatabase::getInstance().getPlayer(name).getCurrentConcentrationSpellName());
						PlayerDatabase::getInstance().getPlayer(name).setRoundsRemainingForConcentrationSpell(roundsLeft);
					}

					//Once turn is complete, reset the players flags and move on to the next player in the queue
					PlayerDatabase::getInstance().getPlayer(name).setIsTurnReady(false);
					PlayerDatabase::getInstance().getPlayer(name).setIsTurnComplete(false);


					auto it = std::find(playerListing.begin(), playerListing.end(), name);
					if (it != playerListing.end()) {

						// Element found, calculate its index
						int index = std::distance(playerListing.begin(), it);

						//If the player is at the end of the queue, loop back around to the beginning
						if (index+1 >= playerListing.size()) {
							PlayerDatabase::getInstance().getPlayer(playerListing.at(0)).setIsTurnReady(true);
						}
						//If not at the end of the Initiative Queue, then just move to next in line
						else {
							PlayerDatabase::getInstance().getPlayer(playerListing.at(index+1)).setIsTurnReady(true);
						}

					}
					else {
						std::cout << "Player of name: " << name << " not found in Combat Initiative" << std::endl;
					}
				}
			}
		}
	}




};
#endif