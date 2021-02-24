#include <catch.hpp>
#include <dsp.h>

using namespace dsp;

TEST_CASE("note to frequency", "[scale]")
{
	CHECK(scale::noteToHz(21) == Approx(27.5).margin(1.e-1));
	CHECK(scale::noteToHz(33) == Approx(55.0));
	CHECK(scale::noteToHz(35) == Approx(61.735).margin(1.e-3));
	CHECK(scale::noteToHz(36) == Approx(65.406).margin(1.e-3));
	CHECK(scale::noteToHz(45) == Approx(110.0));
	CHECK(scale::noteToHz(50) == Approx(146.83).margin(1.e-2));
	CHECK(scale::noteToHz(57) == Approx(220.0));
	CHECK(scale::noteToHz(60) == Approx(261.63).margin(1.e-2));
	CHECK(scale::noteToHz(69) == Approx(440.0));
	CHECK(scale::noteToHz(76) == Approx(659.26).margin(1.e-2));
}

TEST_CASE("frequency to note", "[scale]")
{
	CHECK(scale::hzToNoteId(27.5) == 21);
	CHECK(scale::hzToNoteId(55.0) == 33);
	CHECK(scale::hzToNoteId(110.0) == 45);
	CHECK(scale::hzToNoteId(220.0) == 57);
	CHECK(scale::hzToNoteId(440.0) == 69);
	CHECK(scale::hzToNoteId(880.0) == 81);
}

TEST_CASE("note id to name", "[scale]")
{
	CHECK(scale::noteName(21) == "A0");
	CHECK(scale::noteName(69) == "A4");
	CHECK(scale::noteName(81) == "A5");
}