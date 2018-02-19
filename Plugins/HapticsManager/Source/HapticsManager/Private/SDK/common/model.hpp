//Copyright bHaptics Inc. 2017
#ifndef BHAPTICS_MODEL
#define BHAPTICS_MODEL

#include <map>
#include "Engine.h"

namespace bhaptics
{
	using namespace std;

	enum Position {
		All = 0, Left = 1, Right = 2,
		Vest = 3,
		Head = 4,
		Racket = 5,
		HandL = 6, HandR = 7,
		FootL = 8, FootR = 9,
		VestFront = 201, VestBack = 202,
		GloveLeft = 203, GloveRight = 204,
		Custom1 = 251, Custom2 = 252, Custom3 = 253, Custom4 = 254
	};

	enum FeedbackMode {
		PATH_MODE,
		DOT_MODE
	};

	struct DotPoint
	{
		int index;
		int intensity;
		DotPoint(int _index, int _intensity)
		{
			index = _index;
			if (_index < 0)
				index = 0;
			else if (_index > 19)
				index = 19;
			intensity = _intensity;
		}

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("Index", index);
			j.SetNumberField("Intensity", intensity);
		}

	};

	struct PathPoint
	{
		float x;
		float y;
		int intensity;
		int MotorCount;

		PathPoint(float _x, float _y, int _intensity, int motorCount = 3)
		{
			int xRnd = _x * 1000;
			int yRnd = _y * 1000;
			intensity = _intensity;
			if (motorCount < 1)
			{
				MotorCount = 1;
			}
			else if (motorCount > 3)
			{
				MotorCount = 3;
			}
			else
			{
				MotorCount = motorCount;
			}

			if (_x < 0)
			{
				xRnd = 0;
			}
			else if (_x > 1)
			{
				xRnd = 1000;
			}

			if (_y < 0)
			{
				yRnd = 0;
			}
			else if (_y > 1)
			{
				yRnd = 1000;
			}

			x = (float)(xRnd) / 1000;
			y = (float)(yRnd) / 1000;
		}

		void to_json(FJsonObject& j)
		{
			j.SetStringField("X", FString::SanitizeFloat(x));
			j.SetStringField("Y", FString::SanitizeFloat(y));
			j.SetNumberField("Intensity", intensity);
			j.SetNumberField("MotorCount", MotorCount);
		}

	};

		
	class Common
	{
	public:
		template<class T, class V>
		static bool containsKey(T key, const map<T, V> &mapData)
		{
			return mapData.find(key) != mapData.end();
		}
	};


	////////*********

	class Frame
	{
	public:
		int DurationMillis = 0;
		Position Position;
		vector<PathPoint> PathPoints;
		vector<DotPoint> DotPoints;
		int Texture;

		static Frame AsPathPointFrame(vector<PathPoint> points, bhaptics::Position position, int durationMillis, int texture = 0)
		{
			Frame frame;
			frame.Position = position;
			frame.PathPoints = points;
			frame.Texture = texture;
			frame.DurationMillis = durationMillis;
			return frame;
		}

		static Frame AsDotPointFrame(vector<DotPoint> points, bhaptics::Position position, int durationMillis, int texture = 0)
		{
			Frame frame;
			frame.Position = position;
			frame.DotPoints = points;
			frame.Texture = texture;
			frame.DurationMillis = durationMillis;
			return frame;
		}

		void to_json(FJsonObject& jsonObject)
		{
			vector<string> tempVec1, tempVec2;

			TArray<TSharedPtr<FJsonValue>> jVec1, jVec2;

			for (size_t i = 0; i < PathPoints.size(); i++)
			{
				TSharedPtr<FJsonObject> jTemp = MakeShareable(new FJsonObject);
				PathPoints[i].to_json(*jTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(jTemp));
				jVec1.Add(JsonValue);

			}

			for (size_t i = 0; i < DotPoints.size(); i++)
			{
				TSharedPtr<FJsonObject> jTemp = MakeShareable(new FJsonObject);
				DotPoints[i].to_json(*jTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(jTemp));
				jVec2.Add(JsonValue);
			}

			jsonObject.SetNumberField("Position", Position);
			jsonObject.SetNumberField("Texture", Texture);
			jsonObject.SetArrayField("PathPoints", jVec1);
			jsonObject.SetArrayField("DotPoints", jVec2);
			jsonObject.SetNumberField("DurationMillis", DurationMillis);
		}
	};

	struct ScaleOption
	{
		float Intensity;
		float Duration;

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("Intensity", Intensity);
			j.SetNumberField("Duration", Duration);
		}

		string to_string()
		{
			string ret = "{ \"intensity\" : " + std::to_string(Intensity) + ", \"duration\" : " + std::to_string(Duration) + "}";
			return ret;
		}
	};

	struct RotationOption
	{
		float OffsetAngleX;
		float OffsetY;

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("DeltaX", OffsetAngleX);
			j.SetNumberField("DeltaY", OffsetY);
		}

		string to_string()
		{
			string ret = "{ \"offsetAngleX\" : " + std::to_string(OffsetAngleX) + ", \"offsetY\" : " + std::to_string(OffsetY) + "}";
			return ret;
		}
	};


	struct RegisterRequest
	{
		string Key;
		TSharedPtr<FJsonObject> ProjectJson;

		void to_json(FJsonObject& j)
		{
			j.SetStringField("Key", Key.c_str());
			j.SetObjectField("Project", ProjectJson);
		}
	};

	struct SubmitRequest
	{
		string Type;
		string Key;
		Frame Frame;
		map<string, string> Parameters;

		void to_json(FJsonObject& j)
		{
			TSharedPtr<FJsonObject> frameObject = MakeShareable(new FJsonObject);
			TSharedPtr<FJsonObject> parameterObject = MakeShareable(new FJsonObject);

			Frame.to_json(*frameObject);

			j.SetStringField("Type", Type.c_str());
			j.SetStringField("Key", Key.c_str());
			j.SetObjectField("Frame", frameObject);


			for (auto& p : Parameters)
			{
				parameterObject->SetStringField(p.first.c_str(), p.second.c_str());
			}

			if (parameterObject->Values.Num() > 0)
			{
				j.SetObjectField("Parameters", parameterObject);
			}
		}
	};

	class PlayerRequest
	{
	public:
		vector<RegisterRequest> Register;
		vector<SubmitRequest> Submit;

		static PlayerRequest* Create()
		{
			return new PlayerRequest();
		}

		void to_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> registerValues;
			TArray<TSharedPtr<FJsonValue>> submitValues;

			for (size_t i = 0; i < Register.size(); i++)
			{
				TSharedPtr<FJsonObject> registerObject = MakeShareable(new FJsonObject);
				Register[i].to_json(*registerObject);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(registerObject));
				registerValues.Add(JsonValue);
			}

			for (size_t i = 0; i < Submit.size(); i++)
			{
				TSharedPtr<FJsonObject> submitObject = MakeShareable(new FJsonObject);
				Submit[i].to_json(*submitObject);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(submitObject));
				submitValues.Add(JsonValue);
			}

			j.SetArrayField("Register", registerValues);
			j.SetArrayField("Submit", submitValues);
		}
	};

	struct PlayerResponse
	{
		vector<string> RegisteredKeys;
		vector<string> ActiveKeys;
		int ConnectedDeviceCount;
		vector<Position> ConnectedPositions;
		map<string, vector<int>> Status;

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> regKeyValues = j.GetArrayField("RegisteredKeys");
			TArray<TSharedPtr<FJsonValue>> activeKeyValues = j.GetArrayField("ActiveKeys");
			TMap<FString, TSharedPtr<FJsonValue>> statusValues = j.GetObjectField("Status")->Values;
			const TArray<TSharedPtr<FJsonValue>> *connectedKeyValues;

			for (int i = 0; i < regKeyValues.Num(); i++)
			{
				RegisteredKeys.push_back(TCHAR_TO_UTF8(*regKeyValues[i]->AsString()));
			}

			for (int i = 0; i < activeKeyValues.Num(); i++)
			{
				ActiveKeys.push_back(TCHAR_TO_UTF8(*activeKeyValues[i]->AsString()));
			}

			if (j.TryGetArrayField("ConnectedPositions", connectedKeyValues))
			{
				for (int i = 0; i < connectedKeyValues->Num(); i++)
				{
					string DeviceValue = (TCHAR_TO_UTF8(*(*connectedKeyValues)[i]->AsString()));

					if (DeviceValue == "Left")
					{
						ConnectedPositions.push_back(Position::Left);
					}
					else if (DeviceValue == "Right")
					{
						ConnectedPositions.push_back(Position::Right);
					}
					else if (DeviceValue == "Vest")
					{
						ConnectedPositions.push_back(Position::Vest);
					}
					else if (DeviceValue == "Head")
					{
						ConnectedPositions.push_back(Position::Head);
					}
					else if (DeviceValue == "Racket")
					{
						ConnectedPositions.push_back(Position::Racket);
					}
					else if (DeviceValue == "HandL")
					{
						ConnectedPositions.push_back(Position::HandL);
					}
					else if (DeviceValue == "HandR")
					{
						ConnectedPositions.push_back(Position::HandR);
					}
					else if (DeviceValue == "FootL")
					{
						ConnectedPositions.push_back(Position::FootL);
					}
					else if (DeviceValue == "FootR")
					{
						ConnectedPositions.push_back(Position::FootR);
					}
				}
			}


			for (auto& s : statusValues)
			{
				string key = TCHAR_TO_UTF8(*s.Key);
				TArray<TSharedPtr<FJsonValue>> motorValues = s.Value->AsArray();
				vector<int> motors;
				for (int i = 0; i < motorValues.Num(); i++)
				{
					motors.push_back(motorValues[i]->AsNumber());
				}
				Status[key] = motors;
			}

			ConnectedDeviceCount = j.GetIntegerField("connectedDeviceCount");
		}
	};

}

#endif