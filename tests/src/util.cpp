#include <catch.hpp>
#include <dsp.h>
#include <limits>

using namespace dsp;

SCENARIO("lerping values", "[utils]")
{
	GIVEN("a t value of 0")
	{
		THEN("result should be the from input")
		{
			REQUIRE(lerp(0.0, 1.0, 0.0) == 0.0);
		}
	}

	GIVEN("a t value of 1.0")
	{
		THEN("result should equal the to input")
		{
			REQUIRE(lerp(0.0, 1.0, 1.0) == 1.0);
		}
	}

	GIVEN("a t value above 1.0")
	{
		THEN("result should exceed the to input")
		{
			REQUIRE(lerp(0.0, 1.0, 2.0) > 1.0);
		}
	}

	GIVEN("a negative t value")
	{
		THEN("result should be less than the from input")
		{
			REQUIRE(lerp(0.0, 1.0, -0.5) < 0.0);
		}
	}

	GIVEN("any parameter is NaN")
	{
		WHEN("the value is NaN")
		{
			THEN("result should be the low point")
			{
				REQUIRE(lerp(0.0, 1.0, (double)NAN) == Approx(0.0));
			}
		}

		WHEN("the value is infinite")
		{
			THEN("result should be infinite")
			{
				REQUIRE(lerp(0.0, 1.0, (double)INFINITY) == Approx((double)INFINITY));
			}
		}
	}
}

SCENARIO("clamping values", "[utils]")
{
	GIVEN("a value within the limits")
	{
		THEN("result should be the input value")
		{
			REQUIRE(clamp(0.5, 0.0, 1.0) == 0.5);
		}
	}

	GIVEN("a value outside the min/max bounds")
	{
		WHEN("value is below the minimum bound")
		{
			THEN("result should equal min")
			{
				REQUIRE(clamp(-1.0, 0.0, 1.0) == 0.0);
			}
		}
		WHEN("value is above the maximum bound")
		{
			THEN("result should equal max")
			{
				REQUIRE(clamp(2.0, 0.0, 1.0) == 1.0);
			}
		}
	}

	GIVEN("a maximum that is lower than the minimum")
	{
		THEN("max and min should swap and perform correctly")
		{
			REQUIRE(clamp(0.5, 1.0, 0.0) == 0.5);
			REQUIRE(clamp(-1.0, 1.0, 0.0) == 0.0);
			REQUIRE(clamp(2.0, 1.0, 0.0) == 1.0);
		}
	}

	GIVEN("an invalid number")
	{
		WHEN("number tends to infinity")
		{
			THEN("return closest bound of matching sign")
			{
				REQUIRE(clamp((double)INFINITY, 0.0, 1.0) == 1.0);
				REQUIRE(clamp((double)INFINITY, -100.0, -10.0) == -10.0);
				REQUIRE(clamp(-(double)INFINITY, -100.0, -10.0) == -100.0);
				REQUIRE(clamp((double)INFINITY, 10.0, 50.0) == 50.0);
				REQUIRE(clamp((double)INFINITY, -5.0, 5.0) == 5.0);
				REQUIRE(clamp(-(double)INFINITY, -5.0, 5.0) == -5.0);
			}
		}

		WHEN("number tends towards zero and zero is NOT between bounds")
		{
			THEN("return bound closest to zero")
			{
				REQUIRE(clamp((double)NAN, 0.0, 1.0) == Approx(0.0).scale(1.0));
				REQUIRE(clamp((double)NAN, -30.0, -5.0) == Approx(-5.0));
				REQUIRE(clamp((double)NAN, 5.0, 30.0) == Approx(5.0));
			}
		}

		WHEN("number tends towards zero and zero IS between bounds")
		{
			THEN("return zero")
			{
				REQUIRE(clamp((double)NAN, -5.0, 5.0) == 0.0);
			}
		}
	}
}