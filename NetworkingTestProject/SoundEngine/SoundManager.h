#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <map>
#include <memory>
#include <typeindex>
#include <thread>
#include <chrono>
#include "./../OpenGL/vendor/glm/glm.hpp"
#include "./Utils/Headers/SoundSource.h"

enum SoundTriggers {
	HIT,
	MISS,
	CAST,
	MIDCOURSE,
	NO_TRIGGER,
};

class SoundManager {
private:
	std::map<std::type_index, std::map<SoundTriggers, uint32_t>>classSoundDatabase;

	SoundManager() {}

	// Delete copy constructor and assignment operator
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	float soundGain = 0.1;

public:
	// Singleton Constructor
	static SoundManager& getInstance() {
		static SoundManager instance;
		return instance;
	}

	//Load the ClassID to texture positions in from the "TextureMapping.h"
	void setSoundDatabase(std::map<std::type_index, std::map<SoundTriggers, uint32_t>> textureMap) {
		classSoundDatabase = textureMap;
	}

	//Classes will call this file to get their textures
	void playSound(const std::type_info& classType , SoundTriggers stage) {
		auto it = classSoundDatabase.find(classType);
		if (it != classSoundDatabase.end()) {
			auto it2 = it->second.find(stage);
			if (it2 != it->second.end()) {
				std::thread t([it2,this]() {
					try {
						SoundSource mySpeaker(soundGain);
						mySpeaker.Play(it2->second);
						while (mySpeaker.getState() == AL_PLAYING) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); /*Add a 50 ms sleep to not overload the cpu*/ }
						mySpeaker.~SoundSource();
					}
					catch (const std::exception& e) {
						std::cout << "Did not execute sound properly: " << e.what() << std::endl;
					}

					});
				t.detach();
			}
		}
	}

	void setSoundGain(float gain) {
		soundGain = gain;
	}
	float getCurrentSoundGain() {
		return soundGain;
	}


};

#endif