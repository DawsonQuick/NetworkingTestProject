#pragma once
#ifndef CHARACTERPARTICLEFACTORY_H
#define CHARACTERPARTICLEFACTORY_H
#include "./../../../Utils/ParticalDataBase.h"
#include "./Numbers/Particle_0.h"
#include "./Numbers/Particle_1.h"
#include "./Numbers/Particle_2.h"
#include "./Numbers/Particle_3.h"
#include "./Numbers/Particle_4.h"
#include "./Numbers/Particle_5.h"
#include "./Numbers/Particle_6.h"
#include "./Numbers/Particle_7.h"
#include "./Numbers/Particle_8.h"
#include "./Numbers/Particle_9.h"

#include "./SpecialCharacters/Particle_Minus.h"
#include "./SpecialCharacters/Particle_Plus.h"

#include "./Characters/LowerCase/Particle_a.h"
#include "./Characters/LowerCase/Particle_b.h"
#include "./Characters/LowerCase/Particle_c.h"
#include "./Characters/LowerCase/Particle_d.h"
#include "./Characters/LowerCase/Particle_e.h"
#include "./Characters/LowerCase/Particle_f.h"
#include "./Characters/LowerCase/Particle_g.h"
#include "./Characters/LowerCase/Particle_h.h"
#include "./Characters/LowerCase/Particle_i.h"
#include "./Characters/LowerCase/Particle_j.h"
#include "./Characters/LowerCase/Particle_k.h"
#include "./Characters/LowerCase/Particle_l.h"
#include "./Characters/LowerCase/Particle_m.h"
#include "./Characters/LowerCase/Particle_n.h"
#include "./Characters/LowerCase/Particle_o.h"
#include "./Characters/LowerCase/Particle_p.h"
#include "./Characters/LowerCase/Particle_q.h"
#include "./Characters/LowerCase/Particle_r.h"
#include "./Characters/LowerCase/Particle_s.h"
#include "./Characters/LowerCase/Particle_t.h"
#include "./Characters/LowerCase/Particle_u.h"
#include "./Characters/LowerCase/Particle_v.h"
#include "./Characters/LowerCase/Particle_w.h"
#include "./Characters/LowerCase/Particle_x.h"
#include "./Characters/LowerCase/Particle_y.h"
#include "./Characters/LowerCase/Particle_z.h"

#include "./Characters/UpperCase/Particle_A.h"
#include "./Characters/UpperCase/Particle_B.h"
#include "./Characters/UpperCase/Particle_C.h"
#include "./Characters/UpperCase/Particle_D.h"
#include "./Characters/UpperCase/Particle_E.h"
#include "./Characters/UpperCase/Particle_F.h"
#include "./Characters/UpperCase/Particle_G.h"
#include "./Characters/UpperCase/Particle_H.h"
#include "./Characters/UpperCase/Particle_I.h"
#include "./Characters/UpperCase/Particle_J.h"
#include "./Characters/UpperCase/Particle_K.h"
#include "./Characters/UpperCase/Particle_L.h"
#include "./Characters/UpperCase/Particle_M.h"
#include "./Characters/UpperCase/Particle_N.h"
#include "./Characters/UpperCase/Particle_O.h"
#include "./Characters/UpperCase/Particle_P.h"
#include "./Characters/UpperCase/Particle_Q.h"
#include "./Characters/UpperCase/Particle_R.h"
#include "./Characters/UpperCase/Particle_S.h"
#include "./Characters/UpperCase/Particle_T.h"
#include "./Characters/UpperCase/Particle_U.h"
#include "./Characters/UpperCase/Particle_V.h"
#include "./Characters/UpperCase/Particle_W.h"
#include "./Characters/UpperCase/Particle_X.h"
#include "./Characters/UpperCase/Particle_Y.h"
#include "./Characters/UpperCase/Particle_Z.h"

#include <random>
#include <chrono>
#include <memory>
class CharacterParticleFactory {

private:
	// Delete copy constructor and assignment operator
	CharacterParticleFactory(const CharacterParticleFactory&) = delete;
	CharacterParticleFactory& operator=(const CharacterParticleFactory&) = delete;

	CharacterParticleFactory() {
	};

public:
	// Singleton Constructor
	static CharacterParticleFactory& getInstance() {
		static CharacterParticleFactory instance;
		return instance;
	}

	void generateTextParticle(float positionX, float positionY , std::string text , DamageTypes type) {

		//Get a vertical offset to text is not spawning on the player
		positionY = positionY + GlobalConfigurations::getInstance().getScale() / 2;
		//------------------------------------------------------------------------------------
		// Apply some random offset to the Text
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 gen(seed);
		std::uniform_int_distribution<> dis;
		dis = std::uniform_int_distribution<>(-GlobalConfigurations::getInstance().getScale()*1.5, GlobalConfigurations::getInstance().getScale()*1.5);
		positionX += dis(gen);
		positionY += dis(gen);
		//------------------------------------------------------------------------------------

		

		float textSize = 1.5f;
		glm::vec4 color = playerFieDamageTypeEnumToColor(type);
		std::cout << color.r << " - " << color.g << " - " << color.b << std::endl;
		int spacing = (GlobalConfigurations::getInstance().getScale()/4)* textSize;
		int index = 0;
		for (char character : text) {
			switch (character) {
			case '-':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_Minus>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '+':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_Plus>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '0':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_0>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '1':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_1>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '2':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_2>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '3':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_3>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '4':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_4>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '5':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_5>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '6':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_6>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '7':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_7>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '8':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_8>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case '9':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_9>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'a':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_a>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'b':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_b>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'c':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_c>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'd':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_d>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'e':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_e>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'f':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_f>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'g':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_g>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'h':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_h>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'i':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_i>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'j':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_j>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'k':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_k>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'l':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_l>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'm':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_m>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'n':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_n>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'o':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_o>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'p':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_p>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'q':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_q>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'r':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_r>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 's':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_s>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 't':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_t>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'u':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_u>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'v':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_v>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'w':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_w>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'x':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_x>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'y':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_y>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'z':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_z>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'A':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_A>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'B':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_B>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'C':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_C>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'D':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_D>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'E':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_E>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'F':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_F>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'G':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_G>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'H':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_H>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'I':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_I>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'J':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_J>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'K':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_K>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'L':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_L>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'M':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_M>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'N':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_N>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'O':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_O>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'P':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_P>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'Q':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_Q>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'R':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_R>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'S':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_S>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'T':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_T>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'U':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_U>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'V':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_V>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'W':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_W>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'X':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_X>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'Y':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_Y>(positionX + (spacing * index), positionY, textSize, color));
				break;
			case 'Z':
				ParticleDatabase::getInstance().addParticle("Text", std::make_shared<Particle_Z>(positionX + (spacing * index), positionY, textSize, color));
				break;
			}
			index++;

		}
	}


};
#endif
