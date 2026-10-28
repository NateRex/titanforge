#include <graphics/core/Color.h>
#include <common/Assertions.h>
#include <cmath>

const Color Color::TRANSPARENT(0.f, 0.f, 0.f, 0.f);
const Color Color::BLACK(0.f, 0.f, 0.f, 1.f);
const Color Color::WHITE(1.f, 1.f, 1.f, 1.f);
const Color Color::RED(1.f, 0.f, 0.f, 1.f);
const Color Color::GREEN(0.f, 1.f, 0.f, 1.f);
const Color Color::BLUE(0.f, 0.f, 1.f, 1.f);

Color::Color() : Color(0.f, 0.f, 0.f, 1.f)
{

}

Color::Color(float r, float g, float b, float a) : _red(r), _green(g), _blue(b), _alpha(a)
{
	assertInRange(r, 0, 1);
	assertInRange(g, 0, 1);
	assertInRange(b, 0, 1);
	assertInRange(a, 0, 1);
}

Color::Color(const Color& color): Color(color._red, color._green, color._blue, color._alpha)
{

}

float Color::red() const
{
	return _red;
}

float Color::green() const
{
	return _green;
}

float Color::blue() const
{
	return _blue;
}

float Color::alpha() const
{
	return _alpha;
}

Color Color::withAlpha(float alpha) const
{
	return Color(_red, _green, _blue, alpha);
}

bool Color::operator==(const Color& other) const
{
	return _red == other._red
		&& _green == other._green
		&& _blue == other._blue
		&& _alpha == other._alpha;
}

bool Color::operator!=(const Color& other) const
{
	return !(*this == other);
}

Color Color::fromSRGB(float r, float g, float b, float a)
{
	assertTrue(std::isfinite(r) && std::isfinite(g) && std::isfinite(b) && std::isfinite(a),
		"sRGB components must be finite");
	assertInRange(r, 0, 1);
	assertInRange(g, 0, 1);
	assertInRange(b, 0, 1);
	assertInRange(a, 0, 1);
	auto decode = [](float value) {
		return value <= 0.04045f ? value / 12.92f : std::pow((value + 0.055f) / 1.055f, 2.4f);
	};
	return Color(decode(r), decode(g), decode(b), a);
}

std::array<float, 4> Color::toSRGB() const
{
	auto encode = [](float value) {
		return value <= 0.0031308f ? 12.92f * value : 1.055f * std::pow(value, 1.f / 2.4f) - 0.055f;
	};
	return { encode(_red), encode(_green), encode(_blue), _alpha };
}

Color Color::fromSRGBHex(const std::string& hex)
{
	const size_t start = !hex.empty() && hex[0] == '#' ? 1 : 0;
	const size_t digits = hex.size() - start;
	assertTrue(digits == 6 || digits == 8, "Expected RRGGBB or RRGGBBAA hex color, optionally prefixed by #");
	auto nibble = [](char digit) -> unsigned int {
		if (digit >= '0' && digit <= '9') return digit - '0';
		if (digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
		if (digit >= 'A' && digit <= 'F') return digit - 'A' + 10;
		throw IllegalArgumentException("Invalid hex color digit");
	};
	std::array<float, 4> channels = { 0.f, 0.f, 0.f, 1.f };
	for (size_t i = 0; i < digits / 2; ++i)
	{
		channels[i] = (nibble(hex[start + i * 2]) * 16 + nibble(hex[start + i * 2 + 1])) / 255.f;
	}
	return fromSRGB(channels[0], channels[1], channels[2], channels[3]);
}
