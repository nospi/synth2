#include <catch.hpp>
#include <limiter.h>
#include <gain.h>

TEST_CASE("limiter does not attenuate values below threshold", "[limiter]")
{
	dsp::limiter l;
	REQUIRE(l.process(0.5) == 0.5);
	REQUIRE(l.process(0.25) == 0.25);
	REQUIRE(l.process(0.0) == 0.0);
}

TEST_CASE("limiter attenuates values above threshold such that they do not clip", "[limiter]")
{
	dsp::limiter l;
	l.setAttack(1);
	l.setRelease(10);

	for (int i = 0; i < 441; i++) l.process(1.0);
	REQUIRE(l.process(1.0) < dsp::gain::dB2lin(0.0));

	for (int i = 0; i < 441; i++) l.process(2.0);
	REQUIRE(l.process(2.0) == Approx(dsp::gain::dB2lin(-0.3)).margin(1.e-4));

	for (int i = 0; i < 441; i++) l.process(4.0);
	REQUIRE(l.process(4.0) == Approx(dsp::gain::dB2lin(-0.3)).margin(1.e-4));
}