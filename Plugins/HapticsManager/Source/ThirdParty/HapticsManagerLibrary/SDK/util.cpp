//Copyright bHaptics Inc. 2017
#include "util.h"

#include <fstream>
#include <string>
#include <map>
#include "model.h"
#include "json.hpp"

	using json = nlohmann::json;

	std::string Util::readFile(const std::string& path)
	{
		std::ifstream file(path);

		if (!file.good())
		{
			//throw std::runtime_error("file does not exist : '" + path + "'");
			return "";
		}

		std::string str;
		std::string file_contents;
		while (getline(file, str))
		{
			file_contents += str;
			file_contents.push_back('\n');
		}

		return file_contents;
	}

	bhaptics::HapticFile Util::parse(const std::string& path)
	{
		std::string jsonStr = readFile(path);

		bhaptics::HapticFile file;

		if (jsonStr == "")
		{
			return file;
		}

		json JsonObject = json::parse(jsonStr.c_str());

		file.intervalMillis = JsonObject.at("intervalMillis").get<int>();
		file.size = JsonObject.at("size").get<int>();
		file.durationMillis = JsonObject.at("durationMillis").get<int>();
		file.ProjectJson = JsonObject["project"].dump();

		return file;
	}