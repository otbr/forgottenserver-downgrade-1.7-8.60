// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "augment.h"
#include "augments.h"
#include "damagemodifier.h"
#include "luascript.h"

namespace {
using namespace Lua;

// Augment
int luaAugmentCreate(lua_State* L)
{
	// Augment(name) - lookup from global collection
	// Augment(name, description) - create new augment
	// Augment(name, description, modifier) - create with single modifier
	// Augment(name, description, {modifiers}) - create with multiple modifiers
	
	if (isString(L, 2)) {
		const auto name = getString(L, 2);

		// If only name provided, try to get from global collection
		if (lua_gettop(L) == 2) {
			if (auto augment = Augments::GetAugment(name)) { // augment is shared_ptr
				pushSharedPtr(L, augment);
				setMetatable(L, -1, "Augment");
				return 1;
			}
			lua_pushnil(L);
			return 1;
		}

		// Create new augment
		auto description = getString(L, 3);
		auto augment = Augment::MakeAugment(name, description);

		// Check if modifier(s) provided
		if (lua_gettop(L) >= 4) {
			if (isUserdata(L, 4)) {
				// Single modifier
				if (auto& modifier = getSharedPtr<DamageModifier>(L, 4)) {
					augment->addModifier(modifier);
				} else {
					reportErrorFunc(L, "Invalid modifier userdata");
					lua_pushnil(L);
					return 1;
				}
			} else if (isTable(L, 4)) {
				// Multiple modifiers in table
				lua_pushnil(L);
				while (lua_next(L, 4) != 0) {
					if (isUserdata(L, -1)) {
						if (auto& modifier = getSharedPtr<DamageModifier>(L, -1)) {
							augment->addModifier(modifier);
						}
					}
					lua_pop(L, 1);
				}
			}
		}

		pushSharedPtr(L, augment);
		setMetatable(L, -1, "Augment");
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

int luaAugmentRegister(lua_State* L)
{
	// augment:register()
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	if (Augments::GetAugment(augment->getName())) {
		// Already registered
		pushBoolean(L, false);
		return 1;
	}

	Augments::AddAugment(augment);
	pushBoolean(L, true);
	return 1;
}

int luaAugmentSetName(lua_State* L)
{
	// augment:setName(name)
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	augment->setName(getString(L, 2));
	pushBoolean(L, true);
	return 1;
}

int luaAugmentGetName(lua_State* L)
{
	// augment:getName()
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	pushString(L, augment->getName());
	return 1;
}

int luaAugmentSetDescription(lua_State* L)
{
	// augment:setDescription(description)
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	augment->setDescription(getString(L, 2));
	pushBoolean(L, true);
	return 1;
}

int luaAugmentGetDescription(lua_State* L)
{
	// augment:getDescription()
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	pushString(L, augment->getDescription());
	return 1;
}

int luaAugmentAddDamageModifier(lua_State* L)
{
	// augment:addDamageModifier(modifier)
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	auto& modifier = getSharedPtr<DamageModifier>(L, 2);
	if (!modifier) {
		reportErrorFunc(L, "Invalid DamageModifier userdata");
		lua_pushnil(L);
		return 1;
	}

	augment->addModifier(modifier);
	pushBoolean(L, true);
	return 1;
}

int luaAugmentRemoveDamageModifier(lua_State* L)
{
	// augment:removeDamageModifier(modifier)
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	auto& modifier = getSharedPtr<DamageModifier>(L, 2);
	if (!modifier) {
		reportErrorFunc(L, "Invalid DamageModifier userdata");
		lua_pushnil(L);
		return 1;
	}

	augment->removeModifier(modifier);
	pushBoolean(L, true);
	return 1;
}

int luaAugmentGetAttackModifiers(lua_State* L)
{
	// augment:getAttackModifiers()
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	const auto& modifiers = augment->getAttackModifiers();
	lua_createtable(L, modifiers.size(), 0);
	
	int index = 0;
	for (const auto& modifier : modifiers) {
		pushSharedPtr(L, modifier);
		setMetatable(L, -1, "DamageModifier");
		lua_rawseti(L, -2, ++index);
	}
	
	return 1;
}

int luaAugmentGetDefenseModifiers(lua_State* L)
{
	// augment:getDefenseModifiers()
	auto& augment = getSharedPtr<Augment>(L, 1);
	if (!augment) {
		lua_pushnil(L);
		return 1;
	}

	const auto& modifiers = augment->getDefenseModifiers();
	lua_createtable(L, modifiers.size(), 0);
	
	int index = 0;
	for (const auto& modifier : modifiers) {
		pushSharedPtr(L, modifier);
		setMetatable(L, -1, "DamageModifier");
		lua_rawseti(L, -2, ++index);
	}
	
	return 1;
}

} // namespace

void LuaScriptInterface::registerAugment()
{
	// Augment
	registerClass("Augment", "", luaAugmentCreate);
	registerMetaMethod("Augment", "__eq", LuaScriptInterface::luaUserdataCompare);
	
	registerMethod("Augment", "register", luaAugmentRegister);
	registerMethod("Augment", "setName", luaAugmentSetName);
	registerMethod("Augment", "getName", luaAugmentGetName);
	registerMethod("Augment", "setDescription", luaAugmentSetDescription);
	registerMethod("Augment", "getDescription", luaAugmentGetDescription);
	registerMethod("Augment", "addDamageModifier", luaAugmentAddDamageModifier);
	registerMethod("Augment", "removeDamageModifier", luaAugmentRemoveDamageModifier);
	registerMethod("Augment", "getAttackModifiers", luaAugmentGetAttackModifiers);
	registerMethod("Augment", "getDefenseModifiers", luaAugmentGetDefenseModifiers);
}
