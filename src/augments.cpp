#include "otpch.h"
#include "augments.h"
#include "pugixml.hpp"
#include <iostream>
#include <filesystem>

static std::unordered_map<std::string, std::shared_ptr<Augment>> global_augments {};

std::shared_ptr<Augment> Augments::MakeAugment(std::string_view augmentName) {
	auto it = global_augments.find(augmentName.data());
	if (it != global_augments.end()) {
		auto augmentClone = Augment::MakeAugment(it->second);
		return augmentClone;
	}
	std::cout << "Failed to find augment named: " << augmentName << std::endl;
	return nullptr;
}

void Augments::loadAll() {
	std::string fileName = std::string(path) + "augments.xml";
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName.c_str());
	
	if (!result) {
		std::cout << "[Error] Failed to load " << fileName << ": " << result.description() << std::endl;
		return;
	}

	for (auto augmentNode : doc.child("augments").children("augment")) {
		std::string name = augmentNode.attribute("name").as_string();
		std::string description = augmentNode.attribute("description").as_string();

		if (name.empty()) {
			std::cout << "[Error] All augments require a name" << std::endl;
			continue;
		}

		std::shared_ptr<Augment> augment = Augment::MakeAugment(name);
		augment->setDescription(description);

		for (auto modifierNode : augmentNode.children("modifier")) {
			std::string modType = modifierNode.attribute("mod").as_string();
			uint16_t amount = modifierNode.attribute("value").as_int();
			std::string factor = modifierNode.attribute("factor").as_string("percent");
			uint8_t chance = modifierNode.attribute("chance").as_int(100);
			std::string damageType = modifierNode.attribute("damage").as_string("none");
			std::string originType = modifierNode.attribute("origin").as_string("none");
			std::string creatureType = modifierNode.attribute("target").as_string("none");
			std::string race = modifierNode.attribute("race").as_string("none");
			std::string creatureName = modifierNode.attribute("monster").as_string("none");

			if (ParseStance(modType) == ATTACK_MOD) {
				std::shared_ptr<DamageModifier> damage_modifier = DamageModifier::makeModifier(
					ParseStance(modType),
					ParseAttackModifier(modType),
					amount,
					ParseFactor(factor),
					chance,
					ParseDamage(damageType),
					ParseOrigin(originType),
					ParseCreatureType(creatureType),
					ParseRaceType(race));

				if (modType == "conversion") {
					auto convertedType = ParseDamage(modifierNode.attribute("toDamage").as_string("none"));
					damage_modifier->setTransformDamageType(convertedType);
				}

				if (creatureName != "none") {
					damage_modifier->setCreatureName(creatureName);
				}

				augment->addModifier(damage_modifier);

			} else if (ParseStance(modType) == DEFENSE_MOD) {
				std::shared_ptr<DamageModifier> damage_modifier = DamageModifier::makeModifier(
					ParseStance(modType),
					ParseDefenseModifier(modType),
					amount,
					ParseFactor(factor),
					chance,
					ParseDamage(damageType),
					ParseOrigin(originType),
					ParseCreatureType(creatureType),
					ParseRaceType(race));

				if (modType == "reform") {
					auto reformType = ParseDamage(modifierNode.attribute("toDamage").as_string("none"));
					damage_modifier->setTransformDamageType(reformType);
				}

				if (creatureName != "none") {
					damage_modifier->setCreatureName(creatureName);
				}

				augment->addModifier(damage_modifier);

			} else {
				std::cout << "Modifier has unknown stance: " << modType << std::endl;
			}
		}
		AddAugment(augment);
	}
	
	std::cout << ">> Loaded " << global_augments.size() << " augments" << std::endl;
}

void Augments::clearAll() {
	global_augments.clear();
}

void Augments::reload() {
	clearAll();
	loadAll();
}

ModifierStance Augments::ParseStance(std::string_view modName) noexcept {
	if (ParseAttackModifier(modName) != ATTACK_MODIFIER_NONE) {
		return ATTACK_MOD;
	} else if (ParseDefenseModifier(modName) != DEFENSE_MODIFIER_NONE) {
		return DEFENSE_MOD;
	}
	std::cout << "[Augment Error] no such mod by type name: " << std::string{ modName } << std::endl;
	return NO_MOD;
}

ModFactor Augments::ParseFactor(std::string_view factor) noexcept {
	std::string f_type = std::string{ factor };
	if (f_type == "flat") {
		return FLAT_MODIFIER;
	}
	return PERCENT_MODIFIER;
}

CombatType_t Augments::ParseDamage(std::string_view damageName) noexcept {
	const std::array<std::pair<std::string_view, CombatType_t>, 21> static_map{ {
		{"none",            COMBAT_NONE},
		{"all",             COMBAT_NONE},
		{"physical",        COMBAT_PHYSICALDAMAGE},
		{"melee",           COMBAT_PHYSICALDAMAGE},
		{"energy",          COMBAT_ENERGYDAMAGE},
		{"electric",        COMBAT_ENERGYDAMAGE},
		{"earth",           COMBAT_EARTHDAMAGE},
		{"poison",          COMBAT_EARTHDAMAGE},
		{"fire",            COMBAT_FIREDAMAGE},
		{"lifedrain",       COMBAT_LIFEDRAIN},
		{"lifesteal",       COMBAT_LIFEDRAIN},
		{"lifeleech",       COMBAT_LIFEDRAIN},
		{"manadrain",       COMBAT_MANADRAIN},
		{"manasteal",       COMBAT_MANADRAIN},
		{"manaleech",       COMBAT_MANADRAIN},
		{"drown",           COMBAT_DROWNDAMAGE},
		{"water",           COMBAT_DROWNDAMAGE},
		{"ice",             COMBAT_ICEDAMAGE},
		{"holy",            COMBAT_HOLYDAMAGE},
		{"death",           COMBAT_DEATHDAMAGE},
		{"curse",           COMBAT_DEATHDAMAGE},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == damageName) {
			return value;
		}
	}
	return COMBAT_NONE;
}

CombatOrigin Augments::ParseOrigin(std::string_view originName) noexcept {
	const std::array<std::pair<std::string_view, CombatOrigin>, 14> static_map{ {
		{"none",            ORIGIN_NONE},
		{"all",             ORIGIN_NONE},
		{"condition",       ORIGIN_CONDITION},
		{"spell",           ORIGIN_SPELL},
		{"melee",           ORIGIN_MELEE},
		{"ranged",          ORIGIN_RANGED},
		{"absorb",          ORIGIN_ABSORB},
		{"restore",         ORIGIN_RESTORE},
		{"reflect",         ORIGIN_REFLECT},
		{"deflect",         ORIGIN_DEFLECT},
		{"ricochet",        ORIGIN_RICOCHET},
		{"piercing",        ORIGIN_PIERCING},
		{"augment",         ORIGIN_AUGMENT},
		{"imbuement",       ORIGIN_IMBUEMENT},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == originName) {
			return value;
		}
	}
	return ORIGIN_NONE;
}

ModifierAttackType Augments::ParseAttackModifier(std::string_view modName) noexcept {
	const std::array<std::pair<std::string_view, ModifierAttackType>, 8> static_map{ {
		{"none",            ATTACK_MODIFIER_NONE},
		{"lifesteal",       ATTACK_MODIFIER_LIFESTEAL},
		{"manasteal",       ATTACK_MODIFIER_MANASTEAL},
		{"staminasteal",    ATTACK_MODIFIER_STAMINASTEAL},
		{"soulsteal",       ATTACK_MODIFIER_SOULSTEAL},
		{"critical",        ATTACK_MODIFIER_CRITICAL},
		{"piercing",        ATTACK_MODIFIER_PIERCING},
		{"conversion",      ATTACK_MODIFIER_CONVERSION},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == modName) {
			return value;
		}
	}
	return ATTACK_MODIFIER_NONE;
}

ModifierDefenseType Augments::ParseDefenseModifier(std::string_view modName) noexcept {
	const std::array<std::pair<std::string_view, ModifierDefenseType>, 11> static_map{ {
		{"none",            DEFENSE_MODIFIER_NONE},
		{"absorb",          DEFENSE_MODIFIER_ABSORB},
		{"restore",         DEFENSE_MODIFIER_RESTORE},
		{"replenish",       DEFENSE_MODIFIER_REPLENISH},
		{"revive",          DEFENSE_MODIFIER_REVIVE},
		{"reflect",         DEFENSE_MODIFIER_REFLECT},
		{"deflect",         DEFENSE_MODIFIER_DEFLECT},
		{"ricochet",        DEFENSE_MODIFIER_RICOCHET},
		{"resist",          DEFENSE_MODIFIER_RESIST},
		{"reform",          DEFENSE_MODIFIER_REFORM},
		{"weakness",        DEFENSE_MODIFIER_WEAKNESS},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == modName) {
			return value;
		}
	}
	return DEFENSE_MODIFIER_NONE;
}

RaceType_t Augments::ParseRaceType(std::string_view raceType) noexcept {
	const std::array<std::pair<std::string_view, RaceType_t>, 6> static_map{ {
		{"none",            RACE_NONE},
		{"venom",           RACE_VENOM},
		{"blood",           RACE_BLOOD},
		{"undead",          RACE_UNDEAD},
		{"fire",            RACE_FIRE},
		{"energy",          RACE_ENERGY},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == raceType) {
			return value;
		}
	}
	return RACE_NONE;
}

CreatureType_t Augments::ParseCreatureType(std::string_view creatureType) noexcept {
	const std::array<std::pair<std::string_view, CreatureType_t>, 14> static_map{ {
		{"player",                      CREATURETYPE_PLAYER},
		{"monster",                     CREATURETYPE_MONSTER},
		{"npc",                         CREATURETYPE_NPC},
		{"allsummon",                   CREATURETYPE_SUMMON_ALL},
		{"summons",                     CREATURETYPE_SUMMON_ALL},
		{"ownedsummon",                 CREATURETYPE_SUMMON_OWN},
		{"mysummon",                    CREATURETYPE_SUMMON_OWN},
		{"hostilesummon",               CREATURETYPE_SUMMON_HOSTILE},
		{"enemysummon",                 CREATURETYPE_SUMMON_HOSTILE},
		{"guildsummon",                 CREATURETYPE_SUMMON_GUILD},
		{"partysummon",                 CREATURETYPE_SUMMON_PARTY},
		{"boss",                        CREATURETYPE_BOSS},
		{"none",                        CREATURETYPE_ATTACKABLE},
		{"all",                         CREATURETYPE_ATTACKABLE},
	} };

	for (const auto& [key, value] : static_map) {
		if (key == creatureType) {
			return value;
		}
	}
	return CREATURETYPE_ATTACKABLE;
}

void Augments::AddAugment(std::shared_ptr<Augment> augment) {
	auto [it, inserted] = global_augments.try_emplace(augment->getName().data(), augment);
	if (!inserted) {
		std::cout << "[Warning][Augments] " << augment->getName() << " already exists!" << std::endl;
	}
}

void Augments::RemoveAugment(const std::shared_ptr<Augment>& augment) {
	auto it = global_augments.find(augment->getName().data());
	if (it != global_augments.end()) {
		global_augments.erase(it);
	}
}

void Augments::RemoveAugment(std::string_view augName) {
	auto it = global_augments.find(std::string(augName));
	if (it != global_augments.end()) {
		global_augments.erase(it);
	}
}

void Augments::RemoveAugment(const std::string& augName) {
	auto it = global_augments.find(augName);
	if (it != global_augments.end()) {
		global_augments.erase(it);
	}
}

std::shared_ptr<Augment> Augments::GetAugment(std::string_view augName) {
	auto it = global_augments.find(augName.data());
	if (it != global_augments.end()) {
		// Return the shared_ptr directly, don't clone
		return it->second;
	}
	return nullptr;
}
