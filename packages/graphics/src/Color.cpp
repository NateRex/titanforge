#include <graphics/Color.h>
#include <common/Assertions.h>

Color::Color() : Color(0.f, 0.f, 0.f, 1.f)
{

}

Color::Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a)
{
	assertInRange(r, 0, 1);
	assertInRange(g, 0, 1);
	assertInRange(b, 0, 1);
	assertInRange(a, 0, 1);
}

Color::Color(const Color& color): Color(color.red, color.green, color.blue, color.alpha)
{

}