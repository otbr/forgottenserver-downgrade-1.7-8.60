-- Complete Augment Testing System
-- Commands: /testcritical, /testpiercing, /testlifesteal, /testmanasteal, /teststamina, /testsoul, /testundead

-- Test CRITICAL + CONVERSION
local testCritical = TalkAction("/testcritical")
function testCritical.onSay(player, words, param)
	-- Give magic sword with Fire Master augment (critical + conversion)
	local item = player:addItem(2400, 1) -- magic sword
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Fire Master augment created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Attack a monster to see CRITICAL hits and CONVERSION!")
	end
	return false
end
testCritical:separator(" ")
testCritical:register()

-- Test PIERCING
local testPiercing = TalkAction("/testpiercing")
function testPiercing.onSay(player, words, param)
	-- Give magic sword with Light's Embrace (piercing against undead)
	local item = player:addItem(2400, 1)
	if item then
		item:setAttribute(ITEM_ATTRIBUTE_DESCRIPTION, "Light's Embrace - Pierces undead defenses!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Light's Embrace created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Use /spawnmon skeleton to test PIERCING!")
	end
	return false
end
testPiercing:separator(" ")
testPiercing:register()

-- Test LIFESTEAL
local testLifesteal = TalkAction("/testlifesteal")
function testLifesteal.onSay(player, words, param)
	-- Give magic sword with Vampire Blade (lifesteal)
	local item = player:addItem(2400, 1)
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Vampire Blade created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Attack monsters to steal LIFE (red effect)!")
		
		-- Reduce HP to test healing
		local currentHp = player:getHealth()
		player:addHealth(-(currentHp / 2))
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "HP reduced to 50% - attack to heal!")
	end
	return false
end
testLifesteal:separator(" ")
testLifesteal:register()

-- Test MANASTEAL
local testManasteal = TalkAction("/testmanasteal")
function testManasteal.onSay(player, words, param)
	-- Give magic sword with Mage's Essence (manasteal)
	local item = player:addItem(2400, 1)
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Mage's Essence created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Attack monsters to steal MANA (blue effect)!")
		
		-- Reduce mana to test restoration
		local currentMana = player:getMana()
		player:addMana(-(currentMana / 2))
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Mana reduced to 50% - attack to restore!")
	end
	return false
end
testManasteal:separator(" ")
testManasteal:register()

-- Test STAMINASTEAL
local testStamina = TalkAction("/teststamina")
function testStamina.onSay(player, words, param)
	-- Give magic sword with stamina steal
	local item = player:addItem(2400, 1)
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Stamina Steal created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Attack monsters to steal STAMINA (energy effect)!")
		
		-- Show current stamina
		local stamina = player:getStamina()
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Current stamina: " .. stamina .. " minutes")
	end
	return false
end
testStamina:separator(" ")
testStamina:register()

-- Test SOULSTEAL
local testSoul = TalkAction("/testsoul")
function testSoul.onSay(player, words, param)
	-- Give magic sword with Necromancer's Power (soulsteal against undead)
	local item = player:addItem(2400, 1)
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword with Necromancer's Power created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Use /spawnmon skeleton to steal SOUL (death effect)!")
		
		-- Show current soul
		local soul = player:getSoul()
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Current soul: " .. soul)
	end
	return false
end
testSoul:separator(" ")
testSoul:register()

-- Test against UNDEAD (race modifier) - Light's Embrace
local testUndead = TalkAction("/testundead")
function testUndead.onSay(player, words, param)
	-- Give staff with Light's Embrace augment
	local item = player:addItem(2401, 1) -- staff with Light's Embrace
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Staff with Light's Embrace created!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "This weapon is DEVASTATING against undead!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Now use: /spawnundead")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Then attack and watch the debug logs!")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Expected:")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- PIERCING: 50% chance (ignores armor)")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- CRITICAL: 50% extra damage (100% chance)")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- Damage type: HOLY")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- Only works against race=undead")
	end
	return false
end
testUndead:separator(" ")
testUndead:register()

-- Test Necromancer's Power (all modifiers vs undead)
local testNecro = TalkAction("/testnecro")
function testNecro.onSay(player, words, param)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "=== Testing Necromancer's Power vs Undead ===")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Use /spawnundead to spawn Test Undead (999999 HP)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Then equip an item with 'Necromancer's Power' augment")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Expected results:")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- CRITICAL: 40% extra damage (100% chance)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- PIERCING: 40% chance (ignores armor)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- LIFESTEAL: 25% (red effect)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- SOULSTEAL: 10% (death effect)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- Damage type: DEATH")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "- Only works against race=undead")
	return false
end
testNecro:separator(" ")
testNecro:register()

-- Spawn Test Dummy (999999 HP, race=blood)
local spawnDummy = TalkAction("/spawndummy")
function spawnDummy.onSay(player, words, param)
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())
	
	local monster = Game.createMonster("Test Dummy", position, false, true)
	if monster then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Test Dummy spawned! (999999 HP, race=blood)")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Use this to test damage without killing it!")
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to spawn Test Dummy")
	end
	return false
end
spawnDummy:separator(" ")
spawnDummy:register()

-- Spawn Test Undead (999999 HP, race=undead)
local spawnTestUndead = TalkAction("/spawnundead")
function spawnTestUndead.onSay(player, words, param)
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())
	
	local monster = Game.createMonster("Test Undead", position, false, true)
	if monster then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Test Undead spawned! (999999 HP, race=undead)")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Perfect for testing Light's Embrace and Necromancer's Power!")
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to spawn Test Undead")
	end
	return false
end
spawnTestUndead:separator(" ")
spawnTestUndead:register()

-- Spawn Test Dragon (999999 HP, race=fire)
local spawnTestDragon = TalkAction("/spawndragon")
function spawnTestDragon.onSay(player, words, param)
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())
	
	local monster = Game.createMonster("Test Dragon", position, false, true)
	if monster then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Test Dragon spawned! (999999 HP, race=fire)")
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Perfect for testing Dragon Slayer augment!")
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to spawn Test Dragon")
	end
	return false
end
spawnTestDragon:separator(" ")
spawnTestDragon:register()

-- Test ALL modifiers at once
local testAll = TalkAction("/testall")
function testAll.onSay(player, words, param)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "=== AUGMENT TESTING COMMANDS ===")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testcritical - Test CRITICAL + CONVERSION")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testpiercing - Test PIERCING (ignores armor)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testlifesteal - Test LIFESTEAL (steals HP)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testmanasteal - Test MANASTEAL (steals mana)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/teststamina - Test STAMINASTEAL")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testsoul - Test SOULSTEAL")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testundead - Info: Light's Embrace vs undead")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/testnecro - Info: Necromancer's Power vs undead")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "=== TEST MONSTERS (999999 HP) ===")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/spawndummy - Spawn Test Dummy (race=blood)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/spawnundead - Spawn Test Undead (race=undead)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/spawndragon - Spawn Test Dragon (race=fire)")
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "/spawnmon [name] - Spawn any monster")
	return false
end
testAll:separator(" ")
testAll:register()

-- Spawn monster helper with custom HP
local spawnMon = TalkAction("/spawnmon")
function spawnMon.onSay(player, words, param)
	local monsterName = param ~= "" and param or "rat"
	
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())
	
	local monster = Game.createMonster(monsterName, position, false, true)
	if monster then
		-- Set HP to 9999999 like test monsters
		monster:setMaxHealth(9999999)
		monster:addHealth(9999999)
		
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Spawned " .. monsterName .. " (9999999 HP)")
		
		-- Show race info
		local race = monster:getRace()
		local raceNames = {
			[0] = "none",
			[1] = "venom", 
			[2] = "blood",
			[3] = "undead",
			[4] = "fire",
			[5] = "energy",
			[6] = "ink"
		}
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Race: " .. (raceNames[race] or "unknown") .. " (" .. race .. ")")
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to spawn " .. monsterName)
	end
	
	return false
end
spawnMon:separator(" ")
spawnMon:register()

print(">> Augment testing system loaded!")
print(">> Use /testall to see all commands")
