#pragma once
#ifndef PARTICALDATABASE_H
#define PARTICALDATABASE_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include "./../Objects/Particles/Particle.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"
#include "./TextureManager.h"
#include <algorithm>
#include <execution>
#include <mutex>


class ParticleDatabase {
private:
	std::map<std::string, std::shared_ptr<Particle>> particleMap;
	std::mutex mapVectorMutex;
	std::vector<glm::vec3> particleVec;
	std::vector<float> particleVecRAW;

	std::vector<float> verticeData;
	std::vector<unsigned int> indiceData;
	long long tagindex = 0;
	ParticleDatabase() {}
	std::mutex mapMutex;
	// Delete copy constructor and assignment operator
	ParticleDatabase(const ParticleDatabase&) = delete;
	ParticleDatabase& operator=(const ParticleDatabase&) = delete;


public:

	// Singleton Constructor
	static ParticleDatabase& getInstance() {
		// Guaranteed to be thread-safe in C++11 and later
		static ParticleDatabase instance;
		return instance;
	}

	void onUpdate(float deltaTime) {


		std::mutex toRemoveMutex;
		std::vector<std::string>toRemove;		
		std::lock_guard<std::mutex> lock(mapMutex);

		//Update the positions for each of the particles, and if they are complete, then add to removal list
		std::for_each(std::execution::par, particleMap.begin(), particleMap.end(), [&,deltaTime](auto& pair) {
			pair.second->onUpdate(deltaTime);
			if (pair.second->isComplete()) { 	
			std::lock_guard<std::mutex> lock(toRemoveMutex);
			toRemove.push_back(pair.first); };
		});


		
		//Remove all particles that were added to the removal list
		for (std::string entry : toRemove) {
			particleMap.erase(entry);
		}

		//Populate the positional Vector Data with the updated partical positions		
		std::lock_guard<std::mutex> lock2(mapVectorMutex);
		std::vector<glm::vec3> tmpVec;
		std::vector<float> tmpVecRAW;
		verticeData.clear();
		indiceData.clear();
		unsigned int index = 0;
		for (auto& pair : particleMap) {
			tmpVec.push_back(glm::vec3(pair.second->getPositionX(), pair.second->getPositionY(), 0.0f));
			tmpVecRAW.push_back(pair.second->getPositionX());
			tmpVecRAW.push_back(pair.second->getPositionY());
			tmpVecRAW.push_back(0.0f);
			generateBufferData(pair.second->getPositionX(), pair.second->getPositionY(),pair.second->getVertexPos(), index, pair.second->getTexturePos(),pair.second->getColor());
			index+=4;

		}

		particleVecRAW.clear();
		particleVec = std::move(tmpVec);
		particleVecRAW = std::move(tmpVecRAW);

	}

	//Add a player to the database
	void addParticle(std::string name, std::shared_ptr<Particle> particle) {
		particleMap.emplace(name+std::to_string(tagindex++), particle);
	}

	std::vector<glm::vec3> getParticles() {
		return std::vector<glm::vec3>(particleVec);
	}

	std::vector<float> getParticlesRAW() {
		return std::vector<float>(particleVecRAW);
	}

	std::vector<float> getVerticies() {
		std::lock_guard<std::mutex> lock2(mapVectorMutex);
		return std::vector<float>(verticeData);
	}
	std::vector<unsigned int> getIndicies() {
		std::lock_guard<std::mutex> lock2(mapVectorMutex);
		return std::vector<unsigned int>(indiceData);
	}

	void generateBufferData(float transX, float transY, glm::mat4x3 vertexPos , unsigned int index , glm::mat4x2 texture , glm::vec4 color) {
		verticeData.push_back(vertexPos[0][0]);										//PosX
		verticeData.push_back(vertexPos[0][1]);										//PosY
		verticeData.push_back(vertexPos[0][2]);										//PosZ
		verticeData.push_back(texture[0][0]);		//TextureX
		verticeData.push_back(texture[0][1]);		//TextureY
		verticeData.push_back(transX);												//TransformX
		verticeData.push_back(transY);												//TransformY
		verticeData.push_back(0.0f);												//TransformZ
		verticeData.push_back(color.r);												//Color.R
		verticeData.push_back(color.g);												//Color.G
		verticeData.push_back(color.b);												//Color.B
		verticeData.push_back(color.a);												//Color.A


		verticeData.push_back(vertexPos[1][0]);										//PosX
		verticeData.push_back(vertexPos[1][1]);										//PosY
		verticeData.push_back(vertexPos[1][2]);										//PosZ
		verticeData.push_back(texture[1][0]);		//TextureX
		verticeData.push_back(texture[1][1]);		//TextureY
		verticeData.push_back(transX);												//TransformX
		verticeData.push_back(transY);												//TransformY
		verticeData.push_back(0.0f);												//TransformZ
		verticeData.push_back(color.r);												//Color.R
		verticeData.push_back(color.g);												//Color.G
		verticeData.push_back(color.b);												//Color.B
		verticeData.push_back(color.a);												//Color.A

		verticeData.push_back(vertexPos[2][0]);										//PosX
		verticeData.push_back(vertexPos[2][1]);										//PosY
		verticeData.push_back(vertexPos[2][2]);										//PosZ
		verticeData.push_back(texture[2][0]);		//TextureX
		verticeData.push_back(texture[2][1]);		//TextureY
		verticeData.push_back(transX);												//TransformX
		verticeData.push_back(transY);												//TransformY
		verticeData.push_back(0.0f);												//TransformZ
		verticeData.push_back(color.r);												//Color.R
		verticeData.push_back(color.g);												//Color.G
		verticeData.push_back(color.b);												//Color.B
		verticeData.push_back(color.a);												//Color.A

		verticeData.push_back(vertexPos[3][0]);										//PosX
		verticeData.push_back(vertexPos[3][1]);										//PosY
		verticeData.push_back(vertexPos[3][2]);										//PosZ
		verticeData.push_back(texture[3][0]);		//TextureX
		verticeData.push_back(texture[3][1]);		//TextureY
		verticeData.push_back(transX);												//TransformX
		verticeData.push_back(transY);												//TransformY
		verticeData.push_back(0.0f);												//TransformZ
		verticeData.push_back(color.r);												//Color.R
		verticeData.push_back(color.g);												//Color.G
		verticeData.push_back(color.b);												//Color.B
		verticeData.push_back(color.a);												//Color.A

		indiceData.push_back(index);
		indiceData.push_back(index+ (unsigned int)1);
		indiceData.push_back(index+ (unsigned int)2);
		indiceData.push_back(index+ (unsigned int)2);
		indiceData.push_back(index+ (unsigned int)3);
		indiceData.push_back(index);


	}

};
#endif