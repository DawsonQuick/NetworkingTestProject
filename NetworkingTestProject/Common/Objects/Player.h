#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <chrono>
#include <tuple>
#include "./Spells/Spell.h"
#include "./Weapons/Weapon.h"
#include "./../../Common/Enums/PlayerFields.h"
#include "./../../Common/Enums/AbilityType.h"
#include "./../../Common/Enums/Dice.h"
#include "./../../Common/Enums/DamageTypes.h"
class Player {
private:
	std::vector<std::shared_ptr<Spell>> spells;
	std::vector<std::shared_ptr<Weapon>> weapons;
	std::string name;
	Position position;
	int health;
	int AC;
	double movementSpeed;
	std::shared_ptr<Spell> selectedSpell;
	std::shared_ptr<Weapon> selectedWeapon;
	bool spellChangedState;
	float currentSpellShotCount;

	bool isCurrentlyConcentrating;
	std::shared_ptr<Spell> concentrationSpell;
	float concentrationSpellLocationX;
	float concentrationSpellLocationY;
	int concentrationRoundsLeft;

	bool isTurnReady;
	bool isTurnComplete;

	AbilityScore playerAbilityScore;

	std::vector<int> additionalAttackModifiers;
	std::vector<std::tuple<int, DiceType, DamageTypes>> additionalDamageModifiers;
	std::map<DamageTypes, DamageInteraction> playerDamageAttributes;

	long long getCurrentTimeInMillis() {
		// Get the current time point
		auto now = std::chrono::system_clock::now();

		// Convert time point to milliseconds since epoch
		return static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
	}
public:

	//Constructor
	Player(std::string name, double posX, double posY, double posZ, int health, int AC)
		: name(name),
		health(health),
		position(posX, posY, posZ, getCurrentTimeInMillis()), AC(AC),
		movementSpeed(30.0),
		spellChangedState(false)
	{

	}


	//Default Constructor
	Player()
		: name("Frodo"),
		health(5),
		position(0.0, 0.0, 0.0, getCurrentTimeInMillis()), AC(10), movementSpeed(30.0) {}

	~Player() {

	}


	void addSpell(std::shared_ptr<Spell> spell) { spells.push_back(spell); }
	void addWeapon(std::shared_ptr<Weapon> weapon) { weapons.push_back(weapon); }

	/*
	* -------------------------SETTERS--------------------------------
	*/
	void setIsTurnReady(bool readyState) {
		isTurnReady = readyState;
	}
	void setIsTurnComplete(bool completeState) {
		isTurnComplete = completeState;
	}
	void setName(std::string tmpName) {
		name = tmpName;
	}
	void setHealth(int tmpHealth) {
		health = tmpHealth;

	}
	void setPosition(double posX, double posY, double posZ, long long tmpPointTime) {
		position.X = posX;
		position.Y = posY;
		position.Z = posZ;
		position.pointTime = tmpPointTime;
	}
	void setPositionX(double positionX) {
		position.X = positionX;
	}
	void setPositionY(double positionY) {
		position.Y = positionY;
	}
	void setPositionZ(double positionZ) {
		position.Z = positionZ;
	}
	void setAC(int tmpAC) {
		AC = tmpAC;
	}
	void setMovementSpeed(double tmpSpeed) {
		movementSpeed = tmpSpeed;
	}
	void setSelectedSpell(std::shared_ptr<Spell> spell) {
		currentSpellShotCount = 1.0f; //reset shot count for change in spell
		spellChangedState = true;
		selectedSpell = spell;
	}
	void setSelectedWeapon(std::shared_ptr<Weapon> weapon) {
		selectedWeapon = weapon;
	}
	void setCurrentSpellShotCount(float count) {
		currentSpellShotCount = count;
	}
	float getCurrentSpellShotCount() {
		return currentSpellShotCount;
	}
	void setIsCurrentlyConcentrating(bool concentrating) {
		isCurrentlyConcentrating = concentrating;
	}
	void setConcentrationSpell(std::shared_ptr<Spell> spell) {
		concentrationSpell = spell;
	}

	void setConcentrationSpellLocationX(float x) {
		concentrationSpellLocationX = x;
	}
	void setConcentrationSpellLocationY(float y) {
		concentrationSpellLocationY = y;
	}

	void setAbilityScore(AbilityScore tmpAbilityScore) {
		playerAbilityScore = tmpAbilityScore;
	}
	void addAdditionalAttackModifier(int tmp) {
		additionalAttackModifiers.push_back(tmp);
	}
	void addAdditionalDamageModifier(std::tuple<int, DiceType, DamageTypes> tmp){
		additionalDamageModifiers.push_back(tmp);
	}
	void addPlayerDamageAttribute(DamageTypes type, DamageInteraction interaction) {
		playerDamageAttributes[type] = interaction;
	}


	void setAbilityScore(int strength, int dexterity, int intelligence, int wisdom, int charisma) {
		playerAbilityScore.Strength = strength;
		playerAbilityScore.Dexterity = dexterity;
		playerAbilityScore.Intelligence = intelligence;
		playerAbilityScore.Wisdom = wisdom;
		playerAbilityScore.Charisma = charisma;
	}

	float getConcentrationSpellLocationX() {
		return concentrationSpellLocationX;
	}
	float getConcentrationSpellLocationY() {
		return concentrationSpellLocationY;
	}
	void resetChangeState(bool changeState) {
		spellChangedState = changeState;
	}

	bool hasSpellChanged() {
		return spellChangedState;
	}


	std::shared_ptr<Spell> getSelectedSpell() {
		return selectedSpell;
	}
	std::shared_ptr<Weapon> getSelectedWeapon() {
		return selectedWeapon;
	}
	void castSpell(float targetX, float targetY) {
		//Do upkeep spell tracking here before casting, if current casting spell is concentration , check to see if another concentration spell
		//is currently active , if so then need to stop that spell and replace with this one.

		if (selectedSpell->getSpellType() == SpellType::CONCENTRATION) {
			//Check to see if player currently has a active concentration spell
			if (isCurrentlyConcentrating) {
				//If currently concentrating and then cast another concentration spell, break the old connection and cast the new one
				DurationSpellManager::getInstance().removeConcentrationSpell(name, concentrationSpell->getName());
			}
			isCurrentlyConcentrating = true;
			concentrationRoundsLeft = selectedSpell->getSpellDuration() / 6000; //Divide by the time taken per round
			concentrationSpellLocationX = targetX;
			concentrationSpellLocationY = targetY;
			concentrationSpell = selectedSpell;
		}


		selectedSpell->castSpell(name, targetX, targetY);
	}

	void makeWeaponAttack(float targetX, float targetY) {
		selectedWeapon->attack(name, targetX, targetY);
	}

	void cancelConcentration() {
		if (isCurrentlyConcentrating) {
			DurationSpellManager::getInstance().removeConcentrationSpell(name, concentrationSpell->getName());
			concentrationSpell.reset();
			concentrationRoundsLeft = 0;
			isCurrentlyConcentrating = false;
		}
	}

	bool isPlayerCurrentlyConcentrating() {
		return isCurrentlyConcentrating;
	}
	/*
	* -------------------------GETTERS--------------------------------
	*/
	std::string getName() {
		return name;
	}
	int getHealth() {
		return health;
	}
	Position getPosition() {
		return position;
	}
	double getPositionX() {
		return position.X;
	}
	double getPositionY() {
		return position.Y;
	}
	double getPositionZ() {
		return position.Z;
	}
	int getAC() {
		return AC;
	}
	double getMovementSpeed() {
		return movementSpeed;
	}
	std::vector<std::shared_ptr<Spell>> getAvailableSpells() {
		return spells;
	}

	std::vector<std::shared_ptr<Weapon>> getAvailableWeapons() {
		return weapons;
	}

	std::shared_ptr<Spell> getCurrentConcentrationSpell() {
		return concentrationSpell;
	}
	bool getIsTurnReady() {
		return isTurnReady;
	}
	bool getIsTurnComplete() {
		return isTurnComplete;
	}
	std::string getCurrentConcentrationSpellName() {
		return concentrationSpell->getName();
	}

	void setRoundsRemainingForConcentrationSpell(int rounds) {
		concentrationRoundsLeft = rounds;
	}

	int getRoundsRemainingForConcentrationSpell() {
		return concentrationRoundsLeft;
	}
	AbilityScore getAbilityScore() {
		return playerAbilityScore;
	}

	std::vector<int> getAdditionalAttackModifiers(){
		return additionalAttackModifiers;
	}
	std::vector<std::tuple<int, DiceType, DamageTypes>>  getAdditionalDamageModifiers() {
		return additionalDamageModifiers;
	}
	std::map<DamageTypes, DamageInteraction> getPlayerDamageAttributes() {
		return playerDamageAttributes;
	}

	

/*
* ----------------------DISPLAY STATS------------------------------
*/
	std::string toString() {
		std::ostringstream builder;
		builder << " Name: " << name << " \n"
			<< " Position X: " << position.X << " \n"
			<< " Position Y: " << position.Y << " \n"
			<< " Position Z: " << position.Z << " \n"
			<< " Health: " << health << " \n"
			<< " AC: " << AC << "\n";
		return builder.str();
	}

/*
* ----------------------SERIZALIZE---------------------------------
*/
	std::stringstream serialize() const {
		std::stringstream ss;
		ss << name << "|" << position.X << "|" << position.Y << "|" << position.Z
			<< "|" << health << "|" << AC << "|" << movementSpeed << "|"
			<< "StartSpells"
			<< "|";
		for (std::shared_ptr<Spell> spell : spells) {
			ss << spell->getName() << "|";
		}
		ss << "EndSpells" << "|";
		if (selectedSpell != nullptr) {ss << selectedSpell->getName() << "|";}
		else {ss << "None" << "|";}

		ss << spellChangedState << "|"
			<< currentSpellShotCount << "|" << isCurrentlyConcentrating << "|";
		if (concentrationSpell != nullptr) { ss << concentrationSpell->getName() << "|"; }
		else { ss << "None" << "|"; }
			ss<< concentrationRoundsLeft << "|" << concentrationSpellLocationX <<"|"<< concentrationSpellLocationY<<"|"
			<< isTurnReady << "|" << isTurnComplete;
		return ss;
	}

};
#endif