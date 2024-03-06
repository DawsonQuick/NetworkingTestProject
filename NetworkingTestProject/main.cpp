#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include "./server/ServerConnection.h"
#include "./client/ClientMrg.h"
#include "./OpenGL/vendor/glm/glm.hpp"
#include "./Common/Messages/Message.h"
#include "./Common/Objects/Spells/MagicMissle.h"
#include "./Common/Objects/Spells/FireBall.h"
#include "./Common/Objects/Armour/BreastPlate.h"
#include "./Common/Utils/TextureMapping.h"
#include "./Common/Utils/json.hpp"
#include "./client/ClientEventListener/ClientEventListener.h"
SOCKET serverSocket;

const int BUFFER_SIZE = 1024;

// main.cpp
//MAIN LIST OF TODOS:
/*
*   -Add in a A* Algorith to the Input processing file just on player move, so they step on valid cells and not jumping directly to the end cell (needed for later, when implementing difficult terrain or spells that effect areas)
*   -To follow on to the previous, need to find an the best way to hold statuses inside a grid cell, (if cell is traversable, if there is a spell placed on cell ie. Wall of Thorns) and if player
*    walks on the cell during their movment they take the applied effect.
*   -Similar checks need to be added for spells/projectiles , a simple (LineonLine or LineonSquare) check can be added in to see if the player has a valid line of sight of their target destination
*/



int main() {
    setPositions();
    int choice;
    std::cout << "Enter 0 for server or 1 for client: ";
    std::cin >> choice;
    if (choice == 0) {
        ServerConnection server(8080);
        server.Start();
    }
    else if (choice == 1) {
        ClientMrg mrg;
        mrg.StartOpenGL();
    }
    else if (choice == 2) {

        //Example of loading in a json file.
        //TODO: need to turn this into a utility class used for loading texture atlas meta data
        // and general configuration data for the application
        std::ifstream fJson("Test.json");

        std::stringstream buffer;
        buffer << fJson.rdbuf();
        auto json = nlohmann::json::parse(buffer.str());
        std::cout << json["PerTextureHeight"] << std::endl;
    }
    else if (choice == 3) {
        std::vector<std::shared_ptr<Spell>> spells;
        spells.push_back(std::make_unique<MagicMissle>());
        spells.push_back(std::make_unique<Fireball>());
        std::vector<std::shared_ptr<Armour>> armour;
        armour.push_back(std::make_unique<BreastPlate>(spells));

        for (auto& armourPeice : armour) {
            if (armourPeice->hasSpells()) {
                for (auto& spell : armourPeice->getSpells()) {
                    std::cout << spell->getName() << std::endl;
                }
            }
        }

    }
    else {
        
    }
   
    return 0;
}
