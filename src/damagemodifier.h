#ifndef FS_DAMAGEMODIFIER_H
#define FS_DAMAGEMODIFIER_H

#include "otpch.h"
#include "tools.h"
#include <memory>
#include <cstdint>

struct ModifierTotals {
	ModifierTotals() = default;
	ModifierTotals(uint16_t flat, uint16_t percent) : flatTotal(flat), percentTotal(percent) {}
	uint16_t flatTotal = 0;
	uint16_t percentTotal = 0;

	ModifierTotals operator+(const ModifierTotals& other) const {
		return ModifierTotals(flatTotal + other.flatTotal, percentTotal + other.percentTotal);
	}
	
	ModifierTotals& operator+=(const ModifierTotals& other) {
		flatTotal += other.flatTotal;
		percentTotal = std::min<uint16_t>(percentTotal + other.percentTotal, 100);
		return *this;
	}
};

enum ModifierAttackType : uint8_t {
	ATTACK_MODIFIER_NONE,
	ATTACK_MODIFIER_LIFESTEAL,
	ATTACK_MODIFIER_MANASTEAL,
	ATTACK_MODIFIER_STAMINASTEAL,
	ATTACK_MODIFIER_SOULSTEAL,
	ATTACK_MODIFIER_CRITICAL,
	ATTACK_MODIFIER_PIERCING,
	ATTACK_MODIFIER_CONVERSION,

	ATTACK_MODIFIER_FIRST = ATTACK_MODIFIER_LIFESTEAL,
	ATTACK_MODIFIER_LAST = ATTACK_MODIFIER_CONVERSION,
};

enum ModifierDefenseType : uint8_t {
	DEFENSE_MODIFIER_NONE,
	DEFENSE_MODIFIER_ABSORB,
	DEFENSE_MODIFIER_RESTORE,
	DEFENSE_MODIFIER_REPLENISH,
	DEFENSE_MODIFIER_REVIVE,
	DEFENSE_MODIFIER_REFLECT,
	DEFENSE_MODIFIER_DEFLECT,
	DEFENSE_MODIFIER_RICOCHET,
	DEFENSE_MODIFIER_RESIST,
	DEFENSE_MODIFIER_REFORM,
	DEFENSE_MODIFIER_WEAKNESS,

	DEFENSE_MODIFIER_FIRST = DEFENSE_MODIFIER_NONE,
	DEFENSE_MODIFIER_LAST = DEFENSE_MODIFIER_WEAKNESS
};

enum ModFactor : uint8_t {
	PERCENT_MODIFIER,
	FLAT_MODIFIER
};

enum ModifierStance : uint8_t {
	NO_MOD,
	ATTACK_MOD,
	DEFENSE_MOD
};

class DamageModifier : public std::enable_shared_from_this<DamageModifier> {
public:
	DamageModifier() = default;
	~DamageModifier() = default;

	// allow copying
	DamageModifier(const DamageModifier&) = default;
	DamageModifier& operator=(const DamageModifier&) = default;

	DamageModifier(uint8_t stance, uint8_t modType, uint16_t amount, ModFactor factorType, uint8_t chance, 
		CombatType_t combatType = COMBAT_NONE, CombatOrigin source = ORIGIN_NONE, 
		CreatureType_t creatureType = CREATURETYPE_ATTACKABLE, RaceType_t race = RACE_NONE, 
		const std::string& creatureName = "none") :
		m_mod_stance(stance),
		m_mod_type(modType),
		m_value(amount),
		m_factor(factorType),
		m_chance(chance),
		m_damage_type(combatType),
		m_origin_type(source),
		m_creature_type(creatureType),
		m_race_type(race),
		m_creature_name(creatureName)
	{}

	static std::shared_ptr<DamageModifier> makeModifier(uint8_t stance, uint8_t modType, uint16_t amount, 
		ModFactor factorType, uint8_t chance, CombatType_t combatType = COMBAT_NONE, 
		CombatOrigin source = ORIGIN_NONE, CreatureType_t creatureType = CREATURETYPE_ATTACKABLE, 
		RaceType_t race = RACE_NONE, std::string_view creatureName = "none");

	const uint8_t& getStance() const;
	const uint8_t& getType() const;
	const uint16_t& getValue() const;
	const uint8_t& getChance() const;
	const CombatType_t& getDamageType() const;
	const CombatOrigin& getOriginType() const;
	bool isPercent() const;
	bool isFlatValue() const;
	bool appliesToDamage(const CombatType_t damageType) const;
	bool appliesToOrigin(const CombatOrigin origin) const;
	bool appliesToTarget(const CreatureType_t creatureType, const RaceType_t race, const std::string_view creatureName) const;
	bool isAttackStance() const;
	bool isDefenseStance() const;
	const std::string& getMonsterName() const;
	const CombatType_t& getConversionType() const;

	void setType(uint8_t modType);
	void setStance(uint8_t stance);
	void setValue(uint16_t amount);
	void setChance(uint8_t chance);
	void setFactor(uint8_t factor);
	void setCombatType(CombatType_t combatType);
	void setOriginType(CombatOrigin origin);
	void setRaceType(RaceType_t race);
	void setCreatureType(CreatureType_t c_type);
	void increaseValue(uint16_t amount);
	void decreaseValue(uint16_t amount);
	void setTransformDamageType(CombatType_t damageType);
	void setCreatureName(std::string_view creatureName);

	void serialize(PropWriteStream& propWriteStream) const {
		propWriteStream.write<uint8_t>(m_mod_stance);
		propWriteStream.write<uint8_t>(m_mod_type);
		propWriteStream.write<uint16_t>(m_value);
		propWriteStream.write<uint8_t>(m_factor);
		propWriteStream.write<uint8_t>(m_chance);
		propWriteStream.write<CombatType_t>(m_damage_type);
		propWriteStream.write<CombatType_t>(m_to_damage_type);
		propWriteStream.write<CombatOrigin>(m_origin_type);
		propWriteStream.write<CreatureType_t>(m_creature_type);
		propWriteStream.write<RaceType_t>(m_race_type);
		propWriteStream.writeString(m_creature_name);
	}

	bool unserialize(PropStream& propReadStream) {
		if (!propReadStream.read<uint8_t>(m_mod_stance)) return false;
		if (!propReadStream.read<uint8_t>(m_mod_type)) return false;
		if (!propReadStream.read<uint16_t>(m_value)) return false;
		if (!propReadStream.read<uint8_t>(m_factor)) return false;
		if (!propReadStream.read<uint8_t>(m_chance)) return false;
		if (!propReadStream.read<CombatType_t>(m_damage_type)) return false;
		if (!propReadStream.read<CombatType_t>(m_to_damage_type)) return false;
		if (!propReadStream.read<CombatOrigin>(m_origin_type)) return false;
		if (!propReadStream.read<CreatureType_t>(m_creature_type)) return false;
		if (!propReadStream.read<RaceType_t>(m_race_type)) return false;

		auto [creatureName, success] = propReadStream.readString();
		if (!success) return false;
		m_creature_name = std::string(creatureName);

		return true;
	}

private:
	uint8_t m_mod_stance = 0;
	uint8_t m_mod_type = 0;
	uint16_t m_value = 0;
	uint8_t m_factor = 0;
	uint8_t m_chance = 0;
	CombatType_t m_damage_type = COMBAT_NONE;
	CombatType_t m_to_damage_type = COMBAT_NONE;
	CombatOrigin m_origin_type = ORIGIN_NONE;
	CreatureType_t m_creature_type = CREATURETYPE_ATTACKABLE;
	RaceType_t m_race_type = RACE_NONE;
	std::string m_creature_name = "none";
};

// Inline Methods
inline void DamageModifier::setType(uint8_t modType) {
	m_mod_type = modType;
}

inline void DamageModifier::setStance(uint8_t stance) {
	m_mod_stance = stance;
}

inline void DamageModifier::setValue(uint16_t amount) {
	m_value = amount;
}

inline void DamageModifier::setChance(uint8_t chance) {
	m_chance = chance;
}

inline void DamageModifier::setFactor(uint8_t factor) {
	m_factor = static_cast<ModFactor>(factor);
}

inline void DamageModifier::setCombatType(CombatType_t combatType) {
	m_damage_type = combatType;
}

inline void DamageModifier::setOriginType(CombatOrigin origin) {
	m_origin_type = origin;
}

inline void DamageModifier::setRaceType(RaceType_t race) {
	m_race_type = race;
}

inline void DamageModifier::setCreatureType(CreatureType_t c_type) {
	m_creature_type = c_type;
}

inline void DamageModifier::setCreatureName(std::string_view creatureName) {
	m_creature_name = creatureName.data();
}

inline bool DamageModifier::isPercent() const {
	return m_factor == PERCENT_MODIFIER;
}

inline bool DamageModifier::isFlatValue() const {
	return m_factor == FLAT_MODIFIER;
}

inline bool DamageModifier::appliesToDamage(const CombatType_t damageType) const {
	return m_damage_type == COMBAT_NONE || m_damage_type == damageType;
}

inline bool DamageModifier::appliesToOrigin(const CombatOrigin origin) const {
	bool matches = (m_origin_type == ORIGIN_NONE || m_origin_type == origin);
	bool applies = (m_origin_type == ORIGIN_AUGMENT
		&& (origin == ORIGIN_ABSORB
			|| origin == ORIGIN_RESTORE
			|| origin == ORIGIN_REFLECT
			|| origin == ORIGIN_DEFLECT
			|| origin == ORIGIN_RICOCHET
			|| origin == ORIGIN_PIERCING));
	
	return matches || applies;
}

inline bool DamageModifier::appliesToTarget(const CreatureType_t creatureType, const RaceType_t race, const std::string_view creatureName) const {
	bool matchesType = (m_creature_type == CREATURETYPE_ATTACKABLE || m_creature_type == creatureType);
	bool isValidTarget =
		((m_creature_type == CREATURETYPE_MONSTER || m_creature_type == CREATURETYPE_SUMMON_ALL) && (
			creatureType == CREATURETYPE_MONSTER ||
			creatureType == CREATURETYPE_SUMMON_ALL ||
			creatureType == CREATURETYPE_SUMMON_OWN ||
			creatureType == CREATURETYPE_SUMMON_GUILD ||
			creatureType == CREATURETYPE_SUMMON_HOSTILE ||
			creatureType == CREATURETYPE_SUMMON_PARTY
			));
	bool matchesRace = (m_race_type == RACE_NONE || m_race_type == race);

	bool attackableTarget = false;

	if ((matchesType || isValidTarget) && matchesRace) {
		if (m_creature_name.empty() || m_creature_name == "none") {
			attackableTarget = true;
		} else {
			attackableTarget = (m_creature_name == std::string(creatureName.data()));
		}
	}
	return attackableTarget;
}

inline const uint8_t& DamageModifier::getStance() const {
	return m_mod_stance;
}

inline const uint8_t& DamageModifier::getType() const {
	return m_mod_type;
}

inline const uint16_t& DamageModifier::getValue() const {
	return m_value;
}

inline const uint8_t& DamageModifier::getChance() const {
	return m_chance;
}

inline const CombatType_t& DamageModifier::getDamageType() const {
	return m_damage_type;
}

inline const CombatOrigin& DamageModifier::getOriginType() const {
	return m_origin_type;
}

inline bool DamageModifier::isAttackStance() const {
	return m_mod_stance == ATTACK_MOD;
}

inline bool DamageModifier::isDefenseStance() const {
	return m_mod_stance == DEFENSE_MOD;
}

inline const std::string& DamageModifier::getMonsterName() const {
	return m_creature_name;
}

inline const CombatType_t& DamageModifier::getConversionType() const {
	return m_to_damage_type;
}

#endif
