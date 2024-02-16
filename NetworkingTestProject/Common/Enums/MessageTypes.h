#pragma once
#include <string>
enum MessageType {
    HANDSHAKE,
    BROADCAST,
    ADDPLAYER,
    UPDATEPLAYERDATA,
    CLIENTDISCONNECT,
};

// Function to convert enum value to string
std::string enumToString(MessageType value) {
    switch (value) {
        case MessageType::HANDSHAKE: return "HANDSHAKE";
        case MessageType::BROADCAST: return "BROADCAST";
        case MessageType::ADDPLAYER: return "ADDPLAYER";
        case MessageType::UPDATEPLAYERDATA: return "UPDATEPLAYERDATA";
        case MessageType::CLIENTDISCONNECT: return "CLIENTDISCONNECT";
    }
}