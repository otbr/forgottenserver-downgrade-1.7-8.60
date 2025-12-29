#ifndef FS_AUGMENTS_H
#define FS_AUGMENTS_H

#include "augment.h"
#include <unordered_map>
#include <string>
#include <memory>

class Augments {
public:
	// No Constructors! Purely static class.
	Augments() = delete;
	~Augments() = delete;
	Augments(const Augments&) = delete;
	Augments& operator=(const Augments&) = delete;
	Augments(Augments&&) = delete;
	Augments& operator=(Augments&&) = delete;

	static constexpr auto path = "data/augments/";
	
	static ModifierStance ParseStance(std::string_view stanceName) noexcept;
	static ModFactor ParseFactor(std::string_view factor) noexcept;
	static ModifierAttackType ParseAttackModifier(std::string_view modName) noexcept;
	static ModifierDefenseType ParseDefenseModifier(std::string_view modName) noexcept;
	static CombatType_t ParseDamage(std::string_view damageName) noexcept;
	static CombatOrigin ParseOrigin(std::string_view originName) noexcept;
	static RaceType_t ParseRaceType(std::string_view raceType) noexcept;
	static CreatureType_t ParseCreatureType(std::string_view creatureType) noexcept;

	static std::shared_ptr<Augment> MakeAugment(std::string_view augmentName);

	static void loadAll();
	static void clearAll();
	static void reload();
	static void AddAugment(std::shared_ptr<Augment> augment);
	static void RemoveAugment(const std::shared_ptr<Augment>& augment);
	static void RemoveAugment(std::string_view augName);
	static void RemoveAugment(const std::string& augName);
	static std::shared_ptr<Augment> GetAugment(std::string_view augName);
};

#endif
