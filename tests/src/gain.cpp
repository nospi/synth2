#include <catch.hpp>
#include <dsp.h>

using namespace dsp;

/*
* 
* Data derived using desmos graphing calculator:
*
*	https://www.desmos.com/calculator/ywqrbxl2gy
*
* 1.14 and 29.353 are the intersection points of
* the conversion functions; where input ~= output.
*
* following the voltage gain weighting:
*	a 50% attenuation equates to around 6dB.
* 
*/
TEST_CASE("gain conversions (linear <-> deciBels)", "[gain]")
{
	SECTION("linear amplitude to decibels")
	{
		REQUIRE(gain::lin2dB(1.0) == Approx(0.0).scale(1.0));
		REQUIRE(gain::lin2dB(0.5) == Approx(-6.02059991328));
		REQUIRE(gain::lin2dB(0.25) == Approx(-12.0411998266));
		REQUIRE(gain::lin2dB(2.0) == Approx(6.02059991328));
		REQUIRE(gain::lin2dB(4.0) == Approx(12.0411998266));
		REQUIRE(gain::lin2dB(1.14) == Approx(1.13809702673));
		REQUIRE(gain::lin2dB(29.353) == Approx(29.3530498915));
	}

	SECTION("decibels to linear amplitude")
	{
		REQUIRE(gain::dB2lin(0.0) == 1.0);
		REQUIRE(gain::dB2lin(-6.0) == Approx(0.501187233627));
		REQUIRE(gain::dB2lin(-12.0) == Approx(0.251188643151));
		REQUIRE(gain::dB2lin(6.0) == Approx(1.99526231497));
		REQUIRE(gain::dB2lin(12.0) == Approx(3.98107170553));
		REQUIRE(gain::dB2lin(1.14) == Approx(1.14024978756));
		REQUIRE(gain::dB2lin(29.353) == Approx(29.3528313978));
	}

	SECTION("linear squared amplitude to decibels")
	{
		REQUIRE(gain::linsq2dB(1.0 * 1.0) == Approx(0.0).scale(1.0));
		REQUIRE(gain::linsq2dB(0.5 * 0.5) == Approx(-6.02059991328));
		REQUIRE(gain::linsq2dB(0.25 * 0.25) == Approx(-12.0411998266));
		REQUIRE(gain::linsq2dB(2.0 * 2.0) == Approx(6.02059991328));
		REQUIRE(gain::linsq2dB(4.0 * 4.0) == Approx(12.0411998266));
		REQUIRE(gain::linsq2dB(1.14 * 1.14) == Approx(1.13809702673));
		REQUIRE(gain::linsq2dB(29.353 * 29.353) == Approx(29.3530498915));
	}

	SECTION("gain conversion functions can recurse with accuracy")
	{
		double amplitude = 0.5;
		double dB = 0.0;

		dB = gain::lin2dB(amplitude);
		amplitude = gain::dB2lin(dB);
		dB = gain::lin2dB(amplitude);
		amplitude = gain::dB2lin(dB);
		dB = gain::lin2dB(amplitude);
		amplitude = gain::dB2lin(dB);
		dB = gain::lin2dB(amplitude);
		amplitude = gain::dB2lin(dB);

		REQUIRE(dB == Approx(-6.0206));
		REQUIRE(amplitude == Approx(0.5));
	}
}