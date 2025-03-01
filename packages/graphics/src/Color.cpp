#include <graphics/Color.h>
#include <common/Assertions.h>

Color::Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a)
{
	assertInRange(r, 0, 1);
	assertInRange(g, 0, 1);
	assertInRange(b, 0, 1);
	assertInRange(a, 0, 1);
}

Color::Color(const Color& color): red(color.red), green(color.green), blue(color.blue), alpha(color.alpha)
{

}

Color Color::fromFloats(float r, float g, float b, float a)
{
	return Color(r, g, b, a);
}