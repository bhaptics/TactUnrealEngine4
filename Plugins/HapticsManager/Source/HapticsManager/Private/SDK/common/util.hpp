//Copyright bHaptics Inc. 2017
#ifndef BHAPTICS_UTIL
#define BHAPTICS_UTIL

#include <fstream>
#include <string>
#include <map>
#include "model.hpp"
#include "Engine.h"

namespace bhaptics
{
    using namespace std;
    class Util
    {
    public:
        static string readFile(const string& path)
        {
            ifstream file(path);

            if (!file.good())
            {
                throw runtime_error("file not exists : '" + path + "'");
            }

            string str;
            string file_contents;
            while (getline(file, str))
            {
                file_contents += str;
                file_contents.push_back('\n');
            }

            return file_contents;
        }

        static HapticFile parse(const string& path)
        {
            string jsonStr = readFile(path);
			
			HapticFile file;

			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			FString JsonString(jsonStr.c_str());
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				file.from_json(*JsonObject);
			}
            return file;
        }
    };
}

#endif