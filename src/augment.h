#ifndef FS_AUGMENT_H
#define FS_AUGMENT_H

#include "damagemodifier.h"
#include <memory>
#include <string>
#include <vector>

class Augment : public std::enable_shared_from_this<Augment> {
public:
	Augment() = default;
	Augment(const std::string& name, const std::string& description = "");
	Augment(std::shared_ptr<Augment>& original);
	
	bool operator==(const Augment& other) const {
		return getName() == other.getName();
	}

	~Augment() = default;

	// allow copying
	explicit Augment(const Augment&) = default;
	Augment& operator=(const Augment&) = default;

	const std::string getName() const;
	const std::string getDescription() const;

	void setName(const std::string& name);
	void setDescription(const std::string& description);

	static std::shared_ptr<Augment> MakeAugment(std::string augmentName, std::string description = "");
	static std::shared_ptr<Augment> MakeAugment(std::shared_ptr<Augment>& originalPointer);

	void addModifier(std::shared_ptr<DamageModifier>& modifier);
	void removeModifier(std::shared_ptr<DamageModifier>& modifier);

	std::vector<std::shared_ptr<DamageModifier>>& getAttackModifiers();
	std::vector<std::shared_ptr<DamageModifier>>& getDefenseModifiers();

	std::vector<std::shared_ptr<DamageModifier>> getAttackModifiers(uint8_t modType);
	std::vector<std::shared_ptr<DamageModifier>> getDefenseModifiers(uint8_t modType);

private:
	std::vector<std::shared_ptr<DamageModifier>> m_attack_modifiers;
	std::vector<std::shared_ptr<DamageModifier>> m_defense_modifiers;
	std::string m_name;
	std::string m_description;
};

inline std::shared_ptr<Augment> Augment::MakeAugment(std::string augmentName, std::string /*description*/) {
	auto augment = std::make_shared<Augment>(augmentName);
	return augment;
}

inline std::shared_ptr<Augment> Augment::MakeAugment(std::shared_ptr<Augment>& originalRef) {
	return std::make_shared<Augment>(originalRef);
}

inline const std::string Augment::getName() const {
	return m_name;
}

inline const std::string Augment::getDescription() const {
	return m_description;
}

inline void Augment::setName(const std::string& name) {
	m_name = name;
}

inline void Augment::setDescription(const std::string& description) {
	m_description = description;
}

inline void Augment::addModifier(std::shared_ptr<DamageModifier>& mod) {
	if (mod->getStance() == ATTACK_MOD) {
		m_attack_modifiers.push_back(mod);
	} else if (mod->getStance() == DEFENSE_MOD) {
		m_defense_modifiers.push_back(mod);
	}
}

inline void Augment::removeModifier(std::shared_ptr<DamageModifier>& mod) {
	if (mod->getStance() == ATTACK_MOD) {
		m_attack_modifiers.erase(std::remove(m_attack_modifiers.begin(), m_attack_modifiers.end(), mod), m_attack_modifiers.end());
	} else if (mod->getStance() == DEFENSE_MOD) {
		m_defense_modifiers.erase(std::remove(m_defense_modifiers.begin(), m_defense_modifiers.end(), mod), m_defense_modifiers.end());
	}
}

inline std::vector<std::shared_ptr<DamageModifier>>& Augment::getAttackModifiers() {
	return m_attack_modifiers;
}

inline std::vector<std::shared_ptr<DamageModifier>>& Augment::getDefenseModifiers() {
	return m_defense_modifiers;
}

#endif
