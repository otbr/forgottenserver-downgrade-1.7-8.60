-- Test Dragon - 999999 HP, race=fire
-- Use /spawndragon to spawn
-- Perfect for testing Dragon Slayer augment
local mType = Game.createMonsterType("Test Dragon")
local monster = {}

monster.description = "a test dragon"
monster.experience = 0
monster.outfit = {lookType = 39}

monster.health = 999999
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 5984
monster.speed = 0
monster.maxSummons = 0

monster.changeTarget = {interval = 0, chance = 0}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = false,
	challengeable = false,
	convinceable = true,
	ignoreSpawnBlock = false,
	illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttackChance = 90
}

monster.voices = {}

monster.loot = {}

monster.attacks = {}

monster.defenses = {
	defense = 1,
	armor = 1
}

monster.elements = {}

monster.immunities = {}

mType:register(monster)
