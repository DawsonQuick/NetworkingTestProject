#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
class Logger {
private:
	std::ofstream outFile;

	std::string getTime() {
		std::time_t now = std::time(nullptr);
		std::string timeString = std::ctime(&now);

		// Check if the string ends with a newline character
		if (!timeString.empty() && timeString[timeString.length() - 1] == '\n') {
			// Remove the newline character
			timeString = timeString.substr(0, timeString.length() - 1);
		}

		return timeString;
	}
	//Private Constructor
	Logger():outFile("./log/logs.txt") {
	}
public:


	// Singleton Constructor
	static Logger& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static Logger instance;
		return instance;
	}
	~Logger() {
		outFile.close();
	}
	void error(std::string msg) {
		std::cout << msg << std::endl;
		outFile << getTime() << " --- " << "ERROR: " << msg << std::endl;
	}
	void warn(std::string msg) {
		std::cout << msg << std::endl;
		outFile << getTime() << " --- " << "WARN: " << msg << std::endl;
	}
	void info(std::string msg) {
		std::cout << msg << std::endl;
		outFile << getTime() << " --- " << "INFO: " << msg << std::endl;
	}
	void debug(std::string msg){
		std::cout << msg << std::endl;
		outFile << getTime() << " --- " << "DEBUG: " << msg << std::endl;
	}

};