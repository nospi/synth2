#include "envelope.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include "util.h"
#include "gain.h"


// begin namespace dsp
namespace dsp
{


// begin envelope_detector
    envelope_detector::envelope_detector(double window, double sampleRate)
    {
        if (window <= 0.0) window = 1.0;
        if (sampleRate <= 0.0) sampleRate = 2.0;
        this->window = window;
        this->sampleRate = sampleRate;
        setCoeff();
    }

    void envelope_detector::setWindow(double ms)
    {
        if (ms < 1.0) ms = 1;
        window = ms;
        setCoeff();
    }

    void envelope_detector::setSampleRate(double sampleRate)
    {
        if (sampleRate <= 0.0) sampleRate = 2.0;
        this->sampleRate = sampleRate;
        setCoeff();
    }

    void envelope_detector::setCoeff()
    {
        coeff = exp(-1000.0 / (window * sampleRate));
    }
// end envelope_detector


// begin envlope_ar
    envelope_ar::envelope_ar(double att_ms, double rel_ms, double sampleRate) :
        att(att_ms, sampleRate),
        rel(rel_ms, sampleRate),
        DC_OFFSET(std::numeric_limits<double>::epsilon())
    {}

    void envelope_ar::setAttack(double ms)
    {
        att.setWindow(ms);
    }

    void envelope_ar::setRelease(double ms)
    {
        rel.setWindow(ms);
    }

    void envelope_ar::setSampleRate(double sampleRate)
    {
        att.setSampleRate(sampleRate);
        rel.setSampleRate(sampleRate);
    }
 // end envelope_ar


// begin envelope_adsr
    envelope_adsr::envelope_adsr(double attack, double decay, double sustain, double release) :
        attack(attack), 
        decay(decay), 
        sustain(sustain), 
        release(release),
        DC_OFFSET(std::numeric_limits<double>::epsilon())
    {}

    void envelope_adsr::setAttack(double att)
    {
        attack = clamp(att, 0.001, 4.0);
    }

    void envelope_adsr::setDecay(double dec)
    {
        decay = clamp(dec, 0.001, 4.0);
    }

    void envelope_adsr::setSustain(double sus)
    {
        sustain = clamp(sus, 0.0, 1.0);
    }

    void envelope_adsr::setRelease(double rel)
    {
        release = clamp(rel, 0.001, 4.0);
    }

    double envelope_adsr::process(const double& globalTime, const double& timeOn, const double& timeOff)
    {
        double amplitude = 0.0;
        double releaseAmplitude = 0.0;

        if (timeOn > timeOff)
        {
            // note is ON
            double lifeTime = globalTime - timeOn;
            if (lifeTime <= attack)
            {
                //amplitude = std::max(0.0001, (lifeTime / attack) * 1.0);
                amplitude = lerp(0.0001, 1.0, lifeTime / attack);
                //state = attack;
            }
            if (lifeTime > attack && lifeTime <= (attack + decay))
            {
                //amplitude = ((lifeTime - attack) / decay) * (sustain - 1.0) + 1.0;
                amplitude = lerp(1.0, sustain, (lifeTime - attack) / decay);
                //state = decay;
            }
            if (lifeTime > (attack + decay))
            {
                amplitude = sustain;
                //state = sustain;
            }
        }
        else
        {
            // note is OFF
            double lifeTime = timeOff - timeOn;
            if (lifeTime <= attack)
                releaseAmplitude = lerp(0.0001, 1.0, lifeTime / attack);
            if (lifeTime > attack && lifeTime <= (attack + decay))
                releaseAmplitude = lerp(1.0, sustain, (lifeTime - attack) / decay);
            if (lifeTime > (attack + decay))
                releaseAmplitude = sustain;
            amplitude = lerp(releaseAmplitude, 0.0, (globalTime - timeOff) / release);
        }

        // amplitude should not be negative
        if (amplitude <= DC_OFFSET)
            amplitude = 0.0;

        return amplitude;
    }

    json envelope_adsr::serializeParams() const
    {
        return {
            { "attack", getAttack() },
            { "decay", getDecay() },
            { "sustain", getSustain() },
            { "release", getRelease() }
        };
    }

    void envelope_adsr::deserializeParams(const json& j)
    {
        if (j.contains("attack")) setAttack(j["attack"]);
        if (j.contains("decay")) setDecay(j["decay"]);
        if (j.contains("sustain")) setSustain(j["sustain"]);
        if (j.contains("release")) setRelease(j["release"]);
    }
// end envelope_adsr


}
// end namespace dsp