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
				int intervalMillis = JsonObject->GetIntegerField(TEXT("intervalMillis"));
				int size = JsonObject->GetIntegerField(TEXT("size"));
				int durationMillis = JsonObject->GetIntegerField(TEXT("durationMillis"));
				file.durationMillis = durationMillis;
				file.size = size;
				file.intervalMillis = intervalMillis;

				TMap<FString,TSharedPtr<FJsonValue>> feedbacks = JsonObject->GetObjectField(TEXT("feedback"))->Values;

				for (auto& kv : feedbacks) {
					int key = FCString::Atoi(*kv.Key);
					TArray<TSharedPtr<FJsonValue>> value = kv.Value->AsArray();
					for (auto& tactosyFeedbackJson : value) {
						HapticFeedback feedback;
						TSharedPtr<FJsonObject> tempObj = tactosyFeedbackJson->AsObject();
						string mode (TCHAR_TO_UTF8(*tempObj->GetStringField(TEXT("mode"))));
						string position (TCHAR_TO_UTF8(*tempObj->GetStringField(TEXT("position"))));
						auto values = tempObj->GetArrayField(TEXT("values"));

						int index = 0;
						for (auto& val : values)
						{
							feedback.values[index] = val->AsNumber();
							index++;
						}

						if ("PATH_MODE" == mode)
						{
							feedback.mode = PATH_MODE;
						}
						else if ("DOT_MODE")
						{
							feedback.mode = DOT_MODE;
						}
						else
						{
							throw runtime_error("unknown mode : " + mode);
						}

						if ("Right" == position)
						{
							feedback.position = Right;
						}
						else if ("Left" == position)
						{
							feedback.position = Left;
						}
						else if ("Head" == position)
						{
							feedback.position = Head;
						}
						else if ("VestBack" == position)
						{
							feedback.position = VestBack;
						}
						else if ("VestFront" == position)
						{
							feedback.position = VestFront;
						}
						else if ("Vest" == position)
						{
							feedback.position = Vest;
						}
						else if ("All" == position)
						{
							feedback.position = All;
						}
						else
						{
							throw runtime_error("unkown position " + position);
						}

						file.feedback[key].push_back(feedback);
					}
				}
			}

            return file;
        }
    };
}

#endif