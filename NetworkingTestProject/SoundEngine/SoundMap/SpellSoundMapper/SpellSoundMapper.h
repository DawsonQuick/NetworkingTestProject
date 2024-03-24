#pragma once
#include "./../../Utils/Headers/SoundBuffer.h"
//Add supporting Includes
#include "./../../../Common/Objects/Spells/Fireball.h"
#include "./../../../Common/Objects/Spells/MagicMissle.h"
#include "./../../../Common/Objects/Spells/CloudOfDaggers.h"




std::map<std::type_index, std::map<SoundTriggers, uint32_t>> setSpellSoundMapping() {
	std::map<std::type_index, std::map<SoundTriggers, uint32_t>> SoundMapping;

	SoundMapping.emplace(typeid(Fireball), std::map<SoundTriggers, uint32_t>());
	{

		uint32_t soundCast = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Spells/Fireball/FireballCast.mp3");
		uint32_t soundHit = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Spells/Fireball/Fireball.mp3");
		SoundMapping[typeid(Fireball)].emplace(SoundTriggers::CAST, soundCast);
		SoundMapping[typeid(Fireball)].emplace(SoundTriggers::HIT, soundHit);
	}

	SoundMapping.emplace(typeid(MagicMissle), std::map<SoundTriggers, uint32_t>());
	{
		uint32_t soundHit = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Spells/MagicMissile/MMImpact.mp3");
		SoundMapping[typeid(MagicMissle)].emplace(SoundTriggers::HIT, soundHit);
	}

	SoundMapping.emplace(typeid(CloudOfDaggers), std::map<SoundTriggers, uint32_t>());
	{
		uint32_t sound = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Spells/CloudOfDaggers/CloudOfDaggerSpawn.mp3");
		SoundMapping[typeid(CloudOfDaggers)].emplace(SoundTriggers::NO_TRIGGER, sound);
	}

	return SoundMapping;
}