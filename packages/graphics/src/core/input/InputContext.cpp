#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputActionMapping.h>
#include <common/Assertions.h>

InputContext::InputContext()
{

}

InputContextPtr InputContext::create()
{
	return std::shared_ptr<InputContext>(new InputContext());
}

void InputContext::add(InputKey key, InputTrigger trigger, const InputAction& action)
{
	add(key, trigger, action, InputModifiers());
}

void InputContext::add(InputKey key, InputTrigger trigger, const InputAction& action, const InputModifiers& modifiers)
{
	std::pair<InputKey, InputTrigger> mapKey = std::make_pair(key, trigger);
	_mappings[mapKey].push_back({ key, trigger, action, modifiers });
}

size_t InputContext::size() const
{
	std::size_t total = 0;
	for (const auto& pair : _mappings) {
		total += pair.second.size();
	}
	return total;
}

void InputContext::getMappings(std::vector<InputActionMapping>& results) const
{
	for (const auto& [key, val] : _mappings) {
		results.insert(results.end(), val.begin(), val.end());
	}
}

void InputContext::getMappings(InputKey key, InputTrigger trigger, std::vector<InputActionMapping>& results) const
{
	auto it = _mappings.find(std::make_pair(key, trigger));
	if (it != _mappings.end()) {
		results.insert(results.end(), it->second.begin(), it->second.end());
	}
}

std::size_t InputContext::PairHash::operator()(const std::pair<InputKey, InputTrigger>& p) const {
	return std::hash<int>()(static_cast<int>(p.first)) ^ (std::hash<int>()(static_cast<int>(p.second)) << 1);
}