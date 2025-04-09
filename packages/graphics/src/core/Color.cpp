#include <graphics/core/Color.h>
#include <common/Assertions.h>

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