#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <initializer_list>
#include <map>

class StringStreamer {
public:
    // Create stringstream from a string
    static std::stringstream createStream(const std::string& data) {
        std::stringstream ss;
        ss << data;
        return ss;
    }

    // Create stringstream from a vector
    template<typename T>
    static std::stringstream createStream(const std::vector<T>& data) {
        std::stringstream ss;
        for (const auto& item : data) {
            ss << item << "|";
        }
        return ss;
    }

    // Create stringstream from a list
    template<typename T>
    static std::stringstream createStream(const std::list<T>& data) {
        std::stringstream ss;
        for (const auto& item : data) {
            ss << item << "|";
        }
        return ss;
    }

    // Create stringstream from a list
    template<typename T>
    static std::stringstream createStream(const std::initializer_list<T>& data) {
        std::stringstream ss;
        for (const auto& item : data) {
            ss << item << "|";
        }
        return ss;
    }

    // Create stringstream from a map
    template<typename K, typename V>
    static std::stringstream createStream(const std::map<K, V>& data) {
        std::stringstream ss;
        for (const auto& pair : data) {
            ss << pair.first << ": " << pair.second << std::endl;
        }
        return ss;
    }
    // Create stringstream from a int
    static std::stringstream createStream(const int data) {
        std::stringstream ss;
        ss << data;
        return ss;
    }

    // Create stringstream from a double
    static std::stringstream createStream(const double data) {
        std::stringstream ss;
        ss << data;
        return ss;
    }

    // Add more createStream methods for other types as needed
};
