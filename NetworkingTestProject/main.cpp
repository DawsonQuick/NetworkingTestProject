#include <chrono>
#include <thread>
#include "./server/ServerConnection.h"
#include "./client/ClientMrg.h"
#include "./OpenGL/vendor/glm/glm.hpp"
#include "./Common/Messages/Message.h"
#include "./Common/Utils/json.hpp"
SOCKET serverSocket;

const int BUFFER_SIZE = 1024;

// main.cpp

int main() {
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
    else {

    }
   
    return 0;
}
