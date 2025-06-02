#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputActionMapping.h>

InputContext::InputContext()
{

}

InputContextPtr InputContext::create()
{
	return std::shared_ptr<InputContext>(new InputContext());
}

void InputContext::add(InputKey key, InputTrigger trigger, const InputAction& action, const InputValue& value)
{
	int glfwKey = mapInputKey(key);
	std::pair<int, InputTrigger> mapKey = std::make_pair(glfwKey, trigger);
	_mappings[mapKey].push_back({ glfwKey, trigger, action, value });
}

size_t InputContext::size() const
{
	std::size_t total = 0;
	for (const auto& pair : _mappings) {
		total += pair.second.size();
	}
	return total;
}

void InputContext::get(InputKey key, InputTrigger trigger, std::vector<InputActionMapping>& results) const
{
	int glfwKey = mapInputKey(key);
	auto it = _mappings.find(std::make_pair(glfwKey, trigger));
	if (it != _mappings.end()) {
		results.insert(results.end(), it->second.begin(), it->second.end());
	}
}

std::size_t InputContext::PairHash::operator()(const std::pair<int, InputTrigger>& p) const {
	return std::hash<int>()(p.first) ^ (std::hash<int>()(static_cast<int>(p.second)) << 1);
}