#pragma once
#include <string>
#include <json.hpp>

using nlohmann::json;

namespace dsp
{

	/*
	* Abstract fx_unit interface
	* Override process, de/serializeParams methods to implement fx processors
	*/
	class fx_unit
	{
	protected:
		int id = -1;
		std::string title = "";
		double sampleRate = 0.0;
		double mix = 0.0;

	public:
		bool enabled = false;
		
	public:
		fx_unit(const int& id, const std::string& title, double sampleRate = 44100.0);
		virtual ~fx_unit() = default;
		
		virtual int getId() const { return id; }
		virtual std::string getTitle() const { return title; }
		virtual double getSampleRate() const { return sampleRate; }
		virtual double getMix() const { return mix; }

		virtual void setSampleRate(const double& sampleRate);
		virtual void setMix(const double& mix);

		virtual double process(const double& in) = 0;
		virtual bool deserializeParams(const json& j) = 0;
		virtual json serializeParams() = 0;
	};

}

