#include <catch.hpp>
#include <dsp.h>

using namespace dsp;



TEST_CASE("ratio -> gain reduction", "[compressor]")
{
	SECTION("accurate gain reduction")
	{
		compressor comp;
		comp.enabled = true;
		comp.setAttack(1.0);
		comp.setRelease(100.0);
		comp.setRatio(0.5);
		comp.setThreshold(-9.0);

		double input = gain::dB2lin(-3.0);

		comp.setMakeupGain(0.0);
		for (int i = 0; i < 441; i++) comp.process(input);
		REQUIRE(comp.process(input) == Approx(gain::dB2lin(-6.0)).margin(1.e-4));

		comp.setMakeupGain(3.0);
		for (int i = 0; i < 441; i++) comp.process(input);
		REQUIRE(comp.process(input) == Approx(gain::dB2lin(-3.0)).margin(1.e-4));
		
		comp.setMakeupGain(0.0);
		comp.setRatio(0.25);
		for (int i = 0; i < 441; i++) comp.process(input);
		REQUIRE(comp.process(input) == Approx(gain::dB2lin(-7.5)).margin(1.e-4));
	}

	SECTION("no gain reduction when threshold is not met")
	{
		compressor comp;
		comp.enabled = true;
		comp.setAttack(0.00001);
		comp.setRelease(1.0);
		comp.setRatio(0.5);
		comp.setThreshold(-3.0);
		double input = gain::dB2lin(-9.0);
		REQUIRE(comp.process(input) == Approx(input).margin(1.e-02));
		REQUIRE(comp.getGainReduction() == 0.0);
	}
}

//TEST_CASE("auto makeup gain", "[compressor]")
//{
//	SECTION("makeup gain does not push output over unity")
//	{
//		compressor comp;
//		comp.enabled = true;
//		comp.autoMakeupGain = true;
//		comp.setAttack(0.00001);
//		comp.setRelease(1.0);
//		comp.setRatio(0.5);
//		comp.setThreshold(-9.0);
//
//		double input = gain::dB2lin(18.0);
//		REQUIRE(comp.process(input) <= gain::dB2lin(0.0));
//	}
//}