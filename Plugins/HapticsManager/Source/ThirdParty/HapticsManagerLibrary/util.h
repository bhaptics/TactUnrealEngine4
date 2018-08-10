//Copyright bHaptics Inc. 2017
#ifndef BHAPTICS_UTIL
#define BHAPTICS_UTIL

#include "model.h"
	class Util
	{
	public:
		static std::string readFile(const std::string& path);

		static bhaptics::HapticFile parse(const std::string& path);
	};

#endif