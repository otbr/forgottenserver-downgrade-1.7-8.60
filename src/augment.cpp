#include "otpch.h"
#include "augment.h"

Augment::Augment(const std::string& name, const std::string& description) : m_name(name), m_description(description) {
}

Augment::Augment(std::shared_ptr<Augment>& original) : m_name(original->m_name), m_description(original->m_description) {
	for (const auto& mod : original->m_attack_modifiers) {
		auto copiedMod = std::make_shared<DamageModifier>(*mod);
		m_attack_modifiers.push_back(copiedMod);
	}

	for (const auto& mod : original->m_defense_modifiers) {
		auto copiedMod = std::make_shared<DamageModifier>(*mod);
		m_defense_modifiers.push_back(copiedMod);
	}
}

std::vector<std::shared_ptr<DamageModifier>> Augment::getAttackModifiers(uint8_t modType) {
	std::vector<std::shared_ptr<DamageModifier>> modifiers;
	for (auto& mod : m_attack_modifiers) {
		if (mod->getType() == modType) {
			modifiers.emplace_back(mod);
		}
	}
	return modifiers;
}

std::vector<std::shared_ptr<DamageModifier>> Augment::getDefenseModifiers(uint8_t modType) {
	std::vector<std::shared_ptr<DamageModifier>> modifiers;
	for (auto& mod : m_defense_modifiers) {
		if (mod->getType() == modType) {
			modifiers.emplace_back(mod);
		}
	}
	return modifiers;
}
