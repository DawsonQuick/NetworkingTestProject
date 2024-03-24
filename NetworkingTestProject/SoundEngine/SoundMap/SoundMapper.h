#pragma once
#include <typeindex>
#include <map>
#include "./../SoundManager.h"
#include "./../Utils/Headers/SoundDevice.h"
#include "./WeaponSoundMapper/WeaponSoundMapper.h"
#include "./SpellSoundMapper/SpellSoundMapper.h"









void setSounds() {
	SoundDevice* mysounddevice = SoundDevice::get();
	std::map<std::type_index, std::map<SoundTriggers ,uint32_t>> SoundMapping;
	
	std::map<std::type_index, std::map<SoundTriggers, uint32_t>> spellMapping = setSpellSoundMapping();
	std::map<std::type_index, std::map<SoundTriggers, uint32_t>> weaponMapping = setWeaponSoundMapping();

	SoundMapping.insert(spellMapping.begin(), spellMapping.end());
	SoundMapping.insert(weaponMapping.begin(), weaponMapping.end());

	SoundManager::getInstance().setSoundDatabase(SoundMapping);
}
