function Item:onAugment(augmentName)
	if hasEvent.onAugment then Event.onAugment(self, augmentName) end
	return true
end

function Item:onRemoveAugment(augmentName)
	if hasEvent.onRemoveAugment then Event.onRemoveAugment(self, augmentName) end
	return true
end
