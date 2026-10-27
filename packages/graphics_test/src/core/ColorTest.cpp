#include <boost/test/unit_test.hpp>
#include <graphics/core/Color.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <limits>

/**
 * Tests default construction of a color
 */
BOOST_AUTO_TEST_CASE(Color_default)
{
	Color color;
	BOOST_TEST(color.red() == 0.f);
	BOOST_TEST(color.green() == 0.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 1.f);
}

/**
 * Tests construction of a color from floating-point values
 */
BOOST_AUTO_TEST_CASE(Color_fromFloats)
{
	Color color(0.1f, 0.2f, 0.3f, 0.4f);
	BOOST_TEST(color.red() == 0.1f);
	BOOST_TEST(color.green() == 0.2f);
	BOOST_TEST(color.blue() == 0.3f);
	BOOST_TEST(color.alpha() == 0.4f);
}

/**
 * Tests construction of a color from another color
 */
BOOST_AUTO_TEST_CASE(Color_fromColor)
{
	Color color(Color::GREEN);
	BOOST_TEST(color.red() == 0.f);
	BOOST_TEST(color.green() == 1.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 1.f);
}

/**
 * Tests the ability to create a color with a different alpha channel value
 */
BOOST_AUTO_TEST_CASE(Color_withAlpha)
{
	Color color = Color::RED.withAlpha(0.5f);
	BOOST_TEST(color.red() == 1.f);
	BOOST_TEST(color.green() == 0.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 0.5f);
}

/**
 * Tests the ability to compare colors for equality
 */
BOOST_AUTO_TEST_CASE(Color_equalsChecks)
{
	BOOST_TEST(Color::WHITE == Color::WHITE);
	BOOST_TEST(Color(0.f, 1.f, 1.f, 1.f) != Color::WHITE);
	BOOST_TEST(Color(1.f, 0.f, 1.f, 1.f) != Color::WHITE);
	BOOST_TEST(Color(1.f, 1.f, 0.f, 1.f) != Color::WHITE);
	BOOST_TEST(Color(1.f, 1.f, 1.f, 0.f) != Color::WHITE);
}

/**
 * Tests standard sRGB decoding at the transfer boundary and for mid-gray while preserving alpha
 */
BOOST_AUTO_TEST_CASE(Color_fromSRGB)
{
	auto color = Color::fromSRGB(0.04045f, 0.04046f, 0.5f, 0.4f);
	BOOST_TEST(color.red() == 0.003130805f, boost::test_tools::tolerance(0.000001f));
	BOOST_TEST(color.green() == 0.003131595f, boost::test_tools::tolerance(0.000001f));
	BOOST_TEST(color.blue() == 0.21404114f, boost::test_tools::tolerance(0.000001f));
	BOOST_TEST(color.alpha() == 0.4f);
	BOOST_TEST(Color::fromSRGB(0.f, 1.f, 0.f) == Color::GREEN);
}

/**
 * Tests standard sRGB encoding and conversion round trips while preserving alpha
 */
BOOST_AUTO_TEST_CASE(Color_toSRGB)
{
	const auto encoded = Color(0.0031308f, 0.003131f, 0.21404114f, 0.4f).toSRGB();
	BOOST_TEST(encoded[0] == 0.040449936f, boost::test_tools::tolerance(0.000002f));
	BOOST_TEST(encoded[1] == 0.04045245f, boost::test_tools::tolerance(0.000002f));
	BOOST_TEST(encoded[2] == 0.5f, boost::test_tools::tolerance(0.000001f));
	BOOST_TEST(encoded[3] == 0.4f);
	for (float value : {0.f, 0.01f, 0.04045f, 0.5f, 1.f})
	{
		auto roundTrip = Color::fromSRGB(value, value, value, 0.25f).toSRGB();
		BOOST_TEST(roundTrip[0] == value, boost::test_tools::tolerance(0.000002f));
		BOOST_TEST(roundTrip[1] == value, boost::test_tools::tolerance(0.000002f));
		BOOST_TEST(roundTrip[2] == value, boost::test_tools::tolerance(0.000002f));
		BOOST_TEST(roundTrip[3] == 0.25f);
	}
}

/**
 * Tests hex color entry with optional prefixes, mixed-case digits, and explicit or default alpha
 */
BOOST_AUTO_TEST_CASE(Color_fromSRGBHex)
{
	auto color = Color::fromSRGBHex("#8040fF80");
	BOOST_TEST(color.red() == 0.2158605f, boost::test_tools::tolerance(0.000002f));
	BOOST_TEST(color.green() == 0.05126946f, boost::test_tools::tolerance(0.000002f));
	BOOST_TEST(color.blue() == 1.f);
	BOOST_TEST(color.alpha() == 128.f / 255.f);
	BOOST_TEST(Color::fromSRGBHex("000000") == Color::BLACK);
	BOOST_TEST(Color::fromSRGBHex("#FFFFFF") == Color::WHITE);
	BOOST_TEST(Color::fromSRGBHex("00000000") == Color::TRANSPARENT);
}

/**
 * Tests rejection of invalid sRGB components and hex strings, and existing numeric component bounds
 */
BOOST_AUTO_TEST_CASE(Color_rejectsInvalidSRGB)
{
	for (float invalid : {-0.01f, 1.01f, std::numeric_limits<float>::infinity(),
		 -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()})
	{
		BOOST_CHECK_THROW(Color::fromSRGB(invalid, 0.f, 0.f), IllegalArgumentException);
		BOOST_CHECK_THROW(Color::fromSRGB(0.f, invalid, 0.f), IllegalArgumentException);
		BOOST_CHECK_THROW(Color::fromSRGB(0.f, 0.f, invalid), IllegalArgumentException);
		BOOST_CHECK_THROW(Color::fromSRGB(0.f, 0.f, 0.f, invalid), IllegalArgumentException);
	}
	for (const char* invalid : {"", "#", "fff", "#abcd", "12345", "1234567", "123456789",
		 "0x123456", " 123456", "123456 ", "##123456", "gg0000", "0000000z", "12345-"})
	{
		BOOST_CHECK_THROW(Color::fromSRGBHex(invalid), IllegalArgumentException);
	}
	BOOST_CHECK_THROW(Color(-0.01f, 0.f, 0.f), IllegalArgumentException);
	BOOST_CHECK_THROW(Color(0.f, 1.01f, 0.f), IllegalArgumentException);
	BOOST_CHECK_THROW(Color(0.f, 0.f, -0.01f), IllegalArgumentException);
	BOOST_CHECK_THROW(Color(0.f, 0.f, 0.f, 1.01f), IllegalArgumentException);
}
