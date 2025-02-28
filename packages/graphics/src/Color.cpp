#include <graphics/Color.h>
#include <common/Assertions.h>

Color::Color(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a)
{
	assertInRange(r, 0, 1);
	assertInRange(g, 0, 1);
	assertInRange(b, 0, 1);
	assertInRange(a, 0, 1);
}

Color Color::fromFloats(float r, float g, float b, float a)
{
	return Color(r, g, b, a);
}

float Color::getRed() const
{
	return _r;
}

float Color::getGreen() const
{
	return _g;
}

float Color::getBlue() const
{
	return _b;
}

float Color::getAlpha() const
{
	return _a;
}