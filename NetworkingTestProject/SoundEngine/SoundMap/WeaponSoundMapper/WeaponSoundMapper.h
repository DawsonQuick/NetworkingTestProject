#pragma once

#include "./../../Utils/Headers/SoundBuffer.h"
#include "./../../../Common/Objects/Weapons/SimpleMeleeWeapons/Lighthammer.h"

std::map<std::type_index, std::map<SoundTriggers, uint32_t>> setWeaponSoundMapping() {


	std::map<std::type_index, std::map<SoundTriggers, uint32_t>> SoundMapping;



	SoundMapping.emplace(typeid(Lighthammer), std::map<SoundTriggers, uint32_t>());
	{
		uint32_t /*ALuint*/ soundHit = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Weapons/SimpleMeleeWeapons/Lighthammer/WeaponHit.mp3");
		uint32_t /*ALuint*/ soundMiss = SoundBuffer::get()->addSoundEffect("./SoundEngine/Resources/Weapons/SimpleMeleeWeapons/Lighthammer/WeaponMiss.mp3");
		SoundMapping[typeid(Lighthammer)].emplace(SoundTriggers::HIT, soundHit);
		SoundMapping[typeid(Lighthammer)].emplace(SoundTriggers::MISS, soundMiss);
		
	}


	return SoundMapping;
}