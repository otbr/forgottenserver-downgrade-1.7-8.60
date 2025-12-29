-- Simple test for augments
local testAugment = TalkAction("/testaugment")

function testAugment.onSay(player, words, param)
	-- Give magic sword with Fire Master augment
	local item = player:addItem(2400, 1) -- magic sword
	
	if item then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Magic Sword created! ID: " .. item:getId())
		
		local augment = item:getAugment()
		
		if augment then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Augment: " .. augment)
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "No augment found on item!")
		end
		
		-- Try to get augment description
		local augmentDesc = item:getAugmentDescription()
		if augmentDesc then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Description: " .. augmentDesc)
		end
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to create item!")
	end
	
	return false
end

testAugment:separator(" ")
testAugment:register()

-- Test fire damage
local testFire = TalkAction("/testfire")

function testFire.onSay(player, words, param)
	local damage = tonumber(param) or 100
	
	-- Check if player has augments
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Checking equipped items for augments...")
	
	-- Check all equipment slots (1 to 10)
	for slot = 1, 10 do
		local item = player:getSlotItem(slot)
		if item then
			local augment = item:getAugment()
			if augment then
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Slot " .. slot .. " has augment: " .. augment)
			end
		end
	end
	
	-- Apply fire damage directly
	local position = player:getPosition()
	position:sendMagicEffect(CONST_ME_HITBYFIRE)
	
	-- Damage the player
	doTargetCombatHealth(0, player, COMBAT_FIREDAMAGE, -damage, -damage, CONST_ME_HITBYFIRE)
	
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Applied " .. damage .. " fire damage!")
	
	return false
end

testFire:separator(" ")
testFire:register()

-- Test ice damage (for REFLECT)
local testIce = TalkAction("/testice")

function testIce.onSay(player, words, param)
	local damage = tonumber(param) or 100
	
	-- Apply ice damage
	local position = player:getPosition()
	position:sendMagicEffect(CONST_ME_ICEATTACK)
	
	doTargetCombatHealth(0, player, COMBAT_ICEDAMAGE, -damage, -damage, CONST_ME_ICEATTACK)
	
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Applied " .. damage .. " ice damage! (should REFLECT)")
	
	return false
end

testIce:separator(" ")
testIce:register()

-- Spawn monster
local spawnMon = TalkAction("/spawnmon")

function spawnMon.onSay(player, words, param)
	local monsterName = param ~= "" and param or "rat"
	
	local position = player:getPosition()
	position:getNextPosition(player:getDirection())
	
	local monster = Game.createMonster(monsterName, position, false, true)
	if monster then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Spawned " .. monsterName)
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Failed to spawn!")
	end
	
	return false
end

spawnMon:separator(" ")
spawnMon:register()
