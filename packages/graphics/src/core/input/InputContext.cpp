#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputContextMappings.h>
#include <common/Assertions.h>

InputContext::InputContext()
{

}

InputContextPtr InputContext::create()
{
	return std::shared_ptr<InputContext>(new InputContext());
}

void InputContext::add(AxisInput input, const InputAction& action)
{
	add(input, action, InputModifiers());
}

void InputContext::add(AxisInput input, const InputAction& action, const InputModifiers& modifiers)
{
	_axisMappings[input].push_back({ input, action, modifiers });
}

void InputContext::add(DigitalInput key, InputTrigger trigger, const InputAction& action)
{
	add(key, trigger, action, InputModifiers());
}

void InputContext::add(DigitalInput key, InputTrigger trigger, const InputAction& action, const InputModifiers& modifiers)
{
	std::pair<DigitalInput, InputTrigger> mapKey = std::make_pair(key, trigger);
	_digitalMappings[mapKey].push_back({ key, trigger, action, modifiers });
}

size_t InputContext::size() const
{
	std::size_t total = 0;
	for (const auto& pair : _digitalMappings)
	{
		total += pair.second.size();
	}
	for (const auto& pair : _axisMappings)
	{
		total += pair.second.size();
	}
	return total;
}

void InputContext::getDigitalMappings(std::vector<DigitalMapping>& results) const
{
	for (const auto& [key, val] : _digitalMappings) {
		results.insert(results.end(), val.begin(), val.end());
	}
}

void InputContext::getDigitalMappings(DigitalInput key, InputTrigger trigger, std::vector<DigitalMapping>& results) const
{
	auto it = _digitalMappings.find(std::make_pair(key, trigger));
	if (it != _digitalMappings.end()) {
		results.insert(results.end(), it->second.begin(), it->second.end());
	}
}

void InputContext::getAxisMappings(std::vector<AxisMapping>& results) const
{
	for (const auto& [key, val] : _axisMappings) {
		results.insert(results.end(), val.begin(), val.end());
	}
}

void InputContext::getAxisMappings(AxisInput input, std::vector<AxisMapping>& results) const
{
	auto it = _axisMappings.find(input);
	if (it != _axisMappings.end()) {
		results.insert(results.end(), it->second.begin(), it->second.end());
	}
}

std::size_t DigitalInputHash::operator()(const std::pair<DigitalInput, InputTrigger>& p) const {
	return std::hash<int>()(static_cast<int>(p.first)) ^ (std::hash<int>()(static_cast<int>(p.second)) << 1);
}