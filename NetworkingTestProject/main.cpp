#include <chrono>
#include <thread>
#include "./server/ServerConnection.h"
#include "./client/ClientMrg.h"
#include "./OpenGL/vendor/glm/glm.hpp"
#include "./Common/Messages/Message.h"

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
    else {

    }
   
    return 0;
}
