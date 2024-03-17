#pragma once
#include <typeindex>
#include <map>
#include "./TextureManager.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"
#include "./../../Common/Objects/Particles/GuidedParticle.h"
#include "./../../Common/Objects/Particles/ImplosionParticle.h"
#include "./../../Common/Objects/Particles/TestParticle.h"
#include "./../../Common/Objects/Particles/CloudOfDaggersParticle.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_0.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_1.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_2.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_3.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_4.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_5.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_6.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_7.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_8.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Numbers/Particle_9.h"

#include "./../../Common/Objects/Particles/CharacterParticles/SpecialCharacters/Particle_Minus.h"
#include "./../../Common/Objects/Particles/CharacterParticles/SpecialCharacters/Particle_Plus.h"

#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_a.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_b.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_c.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_c.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_d.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_e.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_f.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_g.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_h.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_i.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_j.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_k.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_l.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_m.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_n.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_o.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_p.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_q.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_r.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_s.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_t.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_u.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_v.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_w.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_x.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_y.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/LowerCase/Particle_z.h"

#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_A.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_B.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_C.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_D.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_E.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_F.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_G.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_H.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_I.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_J.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_K.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_L.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_M.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_N.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_O.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_P.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_Q.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_R.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_S.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_T.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_U.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_V.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_W.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_Q.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_Y.h"
#include "./../../Common/Objects/Particles/CharacterParticles/Characters/UpperCase/Particle_Z.h"

glm::mat4x2 getTexturePositions(glm::vec2 position);


void setPositions() {
std::map<std::type_index, glm::mat4x2> texturePositions;
	//Particles
	texturePositions.emplace(typeid(GuidedParticle),				getTexturePositions(glm::vec2(0.0,0.0)));
	texturePositions.emplace(typeid(ImplosionParticle),				getTexturePositions(glm::vec2(0.0,3.0)));
	texturePositions.emplace(typeid(TestParticle),					getTexturePositions(glm::vec2(0.0,3.0)));
	texturePositions.emplace(typeid(CloudOfDaggersParticle),		getTexturePositions(glm::vec2(0.0,4.0)));


	texturePositions.emplace(typeid(Particle_0),					getTexturePositions(glm::vec2(16.0,29.0)));
	texturePositions.emplace(typeid(Particle_1),					getTexturePositions(glm::vec2(16.0,20.0)));
	texturePositions.emplace(typeid(Particle_2),					getTexturePositions(glm::vec2(16.0,21.0)));
	texturePositions.emplace(typeid(Particle_3),					getTexturePositions(glm::vec2(16.0,22.0)));
	texturePositions.emplace(typeid(Particle_4),					getTexturePositions(glm::vec2(16.0,23.0)));
	texturePositions.emplace(typeid(Particle_5),					getTexturePositions(glm::vec2(16.0,24.0)));
	texturePositions.emplace(typeid(Particle_6),					getTexturePositions(glm::vec2(16.0,25.0)));
	texturePositions.emplace(typeid(Particle_7),					getTexturePositions(glm::vec2(16.0,26.0)));
	texturePositions.emplace(typeid(Particle_8),					getTexturePositions(glm::vec2(16.0,27.0)));
	texturePositions.emplace(typeid(Particle_9),					getTexturePositions(glm::vec2(16.0, 28.0)));


	texturePositions.emplace(typeid(Particle_Minus),				getTexturePositions(glm::vec2(14.0,26.0)));
	texturePositions.emplace(typeid(Particle_Plus),					getTexturePositions(glm::vec2(14.0,27.0)));


	texturePositions.emplace(typeid(Particle_a),					getTexturePositions(glm::vec2(15.0,24.0)));
	texturePositions.emplace(typeid(Particle_b),					getTexturePositions(glm::vec2(15.0,25.0)));
	texturePositions.emplace(typeid(Particle_c),					getTexturePositions(glm::vec2(15.0,26.0)));
	texturePositions.emplace(typeid(Particle_d),					getTexturePositions(glm::vec2(15.0,27.0)));
	texturePositions.emplace(typeid(Particle_e),					getTexturePositions(glm::vec2(15.0,28.0)));
	texturePositions.emplace(typeid(Particle_f),					getTexturePositions(glm::vec2(15.0,29.0)));
	texturePositions.emplace(typeid(Particle_g),					getTexturePositions(glm::vec2(16.0,0.0)));
	texturePositions.emplace(typeid(Particle_h),					getTexturePositions(glm::vec2(16.0,1.0)));
	texturePositions.emplace(typeid(Particle_i),					getTexturePositions(glm::vec2(16.0,2.0)));
	texturePositions.emplace(typeid(Particle_j),					getTexturePositions(glm::vec2(16.0,3.0)));
	texturePositions.emplace(typeid(Particle_k),					getTexturePositions(glm::vec2(16.0,4.0)));
	texturePositions.emplace(typeid(Particle_l),					getTexturePositions(glm::vec2(16.0,5.0)));
	texturePositions.emplace(typeid(Particle_m),					getTexturePositions(glm::vec2(16.0,6.0)));
	texturePositions.emplace(typeid(Particle_n),					getTexturePositions(glm::vec2(16.0,7.0)));
	texturePositions.emplace(typeid(Particle_o),					getTexturePositions(glm::vec2(16.0,8.0)));
	texturePositions.emplace(typeid(Particle_p),					getTexturePositions(glm::vec2(16.0,9.0)));
	texturePositions.emplace(typeid(Particle_q),					getTexturePositions(glm::vec2(16.0,10.0)));
	texturePositions.emplace(typeid(Particle_r),					getTexturePositions(glm::vec2(16.0,11.0)));
	texturePositions.emplace(typeid(Particle_s),					getTexturePositions(glm::vec2(16.0,12.0)));
	texturePositions.emplace(typeid(Particle_t),					getTexturePositions(glm::vec2(16.0,13.0)));
	texturePositions.emplace(typeid(Particle_u),					getTexturePositions(glm::vec2(16.0,14.0)));
	texturePositions.emplace(typeid(Particle_v),					getTexturePositions(glm::vec2(16.0,15.0)));
	texturePositions.emplace(typeid(Particle_w),					getTexturePositions(glm::vec2(16.0,16.0)));
	texturePositions.emplace(typeid(Particle_x),					getTexturePositions(glm::vec2(16.0,17.0)));
	texturePositions.emplace(typeid(Particle_y),					getTexturePositions(glm::vec2(16.0,18.0)));
	texturePositions.emplace(typeid(Particle_z),					getTexturePositions(glm::vec2(16.0,19.0)));

	texturePositions.emplace(typeid(Particle_A),					getTexturePositions(glm::vec2(14.0,28.0)));
	texturePositions.emplace(typeid(Particle_B),					getTexturePositions(glm::vec2(14.0,29.0)));
	texturePositions.emplace(typeid(Particle_C),					getTexturePositions(glm::vec2(15.0,0.0)));
	texturePositions.emplace(typeid(Particle_D),					getTexturePositions(glm::vec2(15.0,1.0)));
	texturePositions.emplace(typeid(Particle_E),					getTexturePositions(glm::vec2(15.0,2.0)));
	texturePositions.emplace(typeid(Particle_F),					getTexturePositions(glm::vec2(15.0,3.0)));
	texturePositions.emplace(typeid(Particle_G),					getTexturePositions(glm::vec2(15.0,4.0)));
	texturePositions.emplace(typeid(Particle_H),					getTexturePositions(glm::vec2(15.0,5.0)));
	texturePositions.emplace(typeid(Particle_I),					getTexturePositions(glm::vec2(15.0,6.0)));
	texturePositions.emplace(typeid(Particle_J),					getTexturePositions(glm::vec2(15.0,7.0)));
	texturePositions.emplace(typeid(Particle_K),					getTexturePositions(glm::vec2(15.0,8.0)));
	texturePositions.emplace(typeid(Particle_L),					getTexturePositions(glm::vec2(15.0,9.0)));
	texturePositions.emplace(typeid(Particle_M),					getTexturePositions(glm::vec2(15.0,10.0)));
	texturePositions.emplace(typeid(Particle_N),					getTexturePositions(glm::vec2(15.0,11.0)));
	texturePositions.emplace(typeid(Particle_O),					getTexturePositions(glm::vec2(15.0,12.0)));
	texturePositions.emplace(typeid(Particle_P),					getTexturePositions(glm::vec2(15.0,13.0)));
	texturePositions.emplace(typeid(Particle_Q),					getTexturePositions(glm::vec2(15.0,14.0)));
	texturePositions.emplace(typeid(Particle_R),					getTexturePositions(glm::vec2(15.0,15.0)));
	texturePositions.emplace(typeid(Particle_S),					getTexturePositions(glm::vec2(15.0,16.0)));
	texturePositions.emplace(typeid(Particle_T),					getTexturePositions(glm::vec2(15.0,17.0)));
	texturePositions.emplace(typeid(Particle_U),					getTexturePositions(glm::vec2(15.0,18.0)));
	texturePositions.emplace(typeid(Particle_V),					getTexturePositions(glm::vec2(15.0,19.0)));
	texturePositions.emplace(typeid(Particle_W),					getTexturePositions(glm::vec2(15.0,20.0)));
	texturePositions.emplace(typeid(Particle_X),					getTexturePositions(glm::vec2(15.0,21.0)));
	texturePositions.emplace(typeid(Particle_Y),					getTexturePositions(glm::vec2(15.0,22.0)));
	texturePositions.emplace(typeid(Particle_Z),					getTexturePositions(glm::vec2(15.0,23.0)));




	TextureManager::getInstance().setTextureDatabase(texturePositions);
}



glm::mat4x2 getTexturePositions(glm::vec2 position) {
	// Width and height of the entire texture
	float textureWidth = 1920.0f;
	float textureHeight = 1080.0f;

	float onePixelWidth = 1 / textureWidth;
	float onePixelHeight = 1 / textureHeight;

	// Width and height of the sub-texture
	float subTextureWidth = 64.0f;
	float subTextureHeight = 64.0f;


	// Calculate texture coordinates for the sub-texture
	float texCoordLeft = (subTextureWidth * position.y) / textureWidth;               // Left edge of the sub-texture
	float texCoordRight = ((position.y + 1) * subTextureWidth) / textureWidth;         // Right edge of the sub-texture
	float texCoordTop = 1.0f - ((subTextureHeight * position.x) / textureHeight);        // Top edge of the sub-texture
	float texCoordBottom = 1.0f - (((position.x + 1) * (subTextureHeight)) / textureHeight); // Bottom edge of the sub-texture

	float Text00 = texCoordLeft + onePixelWidth;		//TextureX
	float Text01 = texCoordBottom + onePixelHeight;		//TextureY

	float Text10 = texCoordRight - onePixelWidth;		//TextureX
	float Text11 = texCoordBottom + onePixelHeight;		//TextureY


	float Text20 = texCoordRight - onePixelWidth;		//TextureX
	float Text21 = texCoordTop - onePixelHeight;		//TextureY

	float Text30 = texCoordLeft + onePixelWidth;		//TextureX
	float Text31 = texCoordTop - onePixelHeight;		//TextureY


	return glm::mat4x2{ Text00 ,Text01,Text10,Text11,Text20,Text21,Text30,Text31 };
}

