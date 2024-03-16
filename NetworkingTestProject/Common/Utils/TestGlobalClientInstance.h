#pragma once
#ifndef TESTGLOBALCLIENTINSTANCE_H
#define TESTGLOBALCLIENTINSTANCE_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include "./../../client/ClientConnection.h"

class TestGlobalClientInstance {
private:

	ClientConnection client;

	TestGlobalClientInstance() {}

	// Delete copy constructor and assignment operator
	TestGlobalClientInstance(const TestGlobalClientInstance&) = delete;
	TestGlobalClientInstance& operator=(const TestGlobalClientInstance&) = delete;

public:

	// Singleton Constructor
	static TestGlobalClientInstance& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static TestGlobalClientInstance instance;
		return instance;
	}

	void setGlobalClientConnection(ClientConnection& tmpClient) {
		client = tmpClient;
	}
	void sendMsg(const char* msg) {
		client.sendMsg(msg);
	}


};
#endif