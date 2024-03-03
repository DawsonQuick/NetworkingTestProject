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

int main() {
    setPositions();
    int choice;
    std::cout << "Enter 0 for server or 1 for client: ";
    std::cin >> choice;
    if (choice == 0) {
        //TODO: Add in capability to pass in a queue or some sortof passable data collection method
        //      Then in the main function be able to process the data
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
