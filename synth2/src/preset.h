#pragma once
#include <string>
#include <json.hpp>

using nlohmann::json;

bool ReadJSONFromFile(const std::string& filename, json& j);

bool WriteJSONToFile(const std::string& filename, const json& j);

json CreateDefaultPreset();