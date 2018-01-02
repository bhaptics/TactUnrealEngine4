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

        PathPoint(float _x, float _y, int _intensity, int motorCount =3)
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

	struct HapticFeedbackFrame
    {
        vector<PathPoint> pathPoints;
        vector<DotPoint> dotPoints;
        int texture;
        Position position;

        HapticFeedbackFrame(Position _pos, const vector<PathPoint> &_pathPoints)
        {
            position = _pos;
            pathPoints = _pathPoints;
            texture = 0;
        }

        HapticFeedbackFrame(Position _pos, const vector<DotPoint> &_dotPoints)
        {
            position = _pos;
            dotPoints = _dotPoints;
            texture = 0;
        }
		HapticFeedbackFrame()
		{
			
		}
    };

    struct HapticFeedback
    {
        Position position;
        FeedbackMode mode;
        vector<uint8_t> values;

        HapticFeedback(Position _pos, const int _val[], FeedbackMode _mod)
        {
            position = _pos;
            mode = _mod;
            values.assign(20, 0);
            for (int i = 0; i < 20; i++)
            {
                values[i] = _val[i];
            }
        }

        HapticFeedback(Position _pos, const vector<uint8_t> &_val, FeedbackMode _mod)
        {
            position = _pos;
            mode = _mod;
            values.assign(20, 0);
            for (int i = 0; i < 20; i++)
            {
                values[i] = _val[i];
            }
        }

        HapticFeedback() : position(), mode()
        {
            values.assign(20, 0);
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

	enum PlaybackType
	{
		NONE,
		FADE_IN,
		FADE_OUT,
		FADE_IN_OUT
	};

	enum PathMovingPattern
	{
		CONST_SPEED,
		CONST_TDM
	};

	struct DotModeObject
	{
		int index;
		float intensity;

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("Index", index);

			j.SetStringField("Intensity", FString::SanitizeFloat(intensity));
		}

		void from_json(FJsonObject& j)
		{
			index = j.GetNumberField("index");
			intensity = j.GetNumberField("intensity");
		}
	};

	struct DotModeObjectCollection
	{
		int StartTime;
		int EndTime;
		PlaybackType PlaybackType = PlaybackType::NONE;
		std::vector<DotModeObject> PointList;

		void to_json(FJsonObject& j)
		{
			string playbackValue = "";
			TArray <TSharedPtr<FJsonValue>> pointValues;

			switch (PlaybackType)
			{
			case PlaybackType::NONE:
				playbackValue = "NONE";
				break;
			case PlaybackType::FADE_IN:
				playbackValue = "FADE_IN";
				break;
			case PlaybackType::FADE_OUT:
				playbackValue = "FADE_OUT";
				break;
			case PlaybackType::FADE_IN_OUT:
				playbackValue = "FADE_IN_OUT";
				break;
			default:
				playbackValue = "ERROR";
				break;
			}

			for (size_t i = 0; i < PointList.size(); i++)
			{
				TSharedPtr<FJsonObject> pointTemp = MakeShareable(new FJsonObject);
				PointList[i].to_json(*pointTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pointTemp));
				pointValues.Add(JsonValue);
			}

			j.SetNumberField("StartTime", StartTime);
			j.SetNumberField("EndTime", EndTime);
			j.SetStringField("PlaybackType", playbackValue.c_str());
			j.SetArrayField("PointList", pointValues);
		}

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> dotObj = j.GetArrayField("pointList");
			string playbackType = (TCHAR_TO_UTF8(*j.GetStringField(TEXT("playbackType"))));

			StartTime = j.GetIntegerField("startTime");
			EndTime = j.GetIntegerField("endTime");
			
			if (playbackType == "NONE")
			{
				PlaybackType = NONE;
			}
			else if (playbackType == "FADE_IN")
			{
				PlaybackType = FADE_IN;
			}
			else if (playbackType == "FADE_OUT")
			{
				PlaybackType = FADE_OUT;
			}
			else
			{
				PlaybackType = FADE_IN_OUT;
			}

			for (int i = 0; i < dotObj.Num(); i++)
			{
				DotModeObject tempDot;
				tempDot.from_json(*dotObj[i]->AsObject());
				PointList.push_back(tempDot);
			}
			
		}
	};

	struct DotMode
	{
		bool DotConnected;
		std::vector<DotModeObjectCollection> Feedback;

		void to_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> feedbackValues;

			for (size_t i = 0; i < Feedback.size(); i++)
			{
				TSharedPtr<FJsonObject> dotObjectTemp = MakeShareable(new FJsonObject);
				Feedback[i].to_json(*dotObjectTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(dotObjectTemp));
				feedbackValues.Add(JsonValue);
			}

			j.SetBoolField("DotConnected", DotConnected);
			j.SetArrayField("Feedback", feedbackValues);
		}

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> feedbackObj = j.GetArrayField("feedback");

			DotConnected = j.GetBoolField("dotConnected");

			for (int i = 0; i < feedbackObj.Num(); i++)
			{
				DotModeObjectCollection dotCollectionTemp;
				dotCollectionTemp.from_json(*feedbackObj[i]->AsObject());
				Feedback.push_back(dotCollectionTemp);
			}
		}

	};

	struct PathModeObject
	{
		float X;
		float Y;
		float Intensity;
		int Time;

		void to_json(FJsonObject& j)
		{
			j.SetStringField("X", FString::SanitizeFloat(X));
			j.SetStringField("Y", FString::SanitizeFloat(Y));
			j.SetStringField("Intensity", FString::SanitizeFloat(Intensity));

			j.SetNumberField("Time", Time);
		}

		void from_json(FJsonObject& j)
		{
			X = j.GetNumberField("x");
			Y = j.GetNumberField("y");
			Intensity = j.GetNumberField("intensity");
			Time = j.GetIntegerField("time");
		}
	};

	struct PathModeObjectCollection
	{
		PlaybackType PlaybackType = PlaybackType::NONE;
		PathMovingPattern MovingPattern = PathMovingPattern::CONST_TDM;
		std::vector<PathModeObject> PointList;
		

		void to_json(FJsonObject& j)
		{
			string playbackValue = "";
			string patternValue = "";
			TArray<TSharedPtr<FJsonValue>> pointValues;

			switch (PlaybackType)
			{
			case PlaybackType::NONE:
				playbackValue = "NONE";
				break;
			case PlaybackType::FADE_IN:
				playbackValue = "FADE_IN";
				break;
			case PlaybackType::FADE_OUT:
				playbackValue = "FADE_OUT";
				break;
			case PlaybackType::FADE_IN_OUT:
				playbackValue = "FADE_IN_OUT";
				break;
			default:
				playbackValue = "ERROR";
				break;
			}

			switch (MovingPattern)
			{
			case PathMovingPattern::CONST_TDM:
				patternValue = "CONST_TDM";
				break;
			case PathMovingPattern::CONST_SPEED:
				patternValue = "CONST_SPEED";
				break;
			default:
				patternValue = "ERROR";
				break;
			}

			j.SetStringField("PlaybackType",playbackValue.c_str());
			j.SetStringField("MovingPattern", patternValue.c_str());

			for (size_t i = 0; i < PointList.size(); i++)
			{
				TSharedPtr<FJsonObject> pointTemp = MakeShareable(new FJsonObject);
				PointList[i].to_json(*pointTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pointTemp));
				pointValues.Add(JsonValue);
			}

			j.SetArrayField("PointList", pointValues);

		}

		void from_json(FJsonObject& j)
		{
			string playbackType = (TCHAR_TO_UTF8(*j.GetStringField(TEXT("playbackType"))));
			string movingPattern = (TCHAR_TO_UTF8(*j.GetStringField(TEXT("movingPattern"))));

			if (playbackType == "NONE")
			{
				PlaybackType = NONE;
			}
			else if (playbackType == "FADE_IN")
			{
				PlaybackType = FADE_IN;
			}
			else if (playbackType == "FADE_OUT")
			{
				PlaybackType = FADE_OUT;
			}
			else
			{
				PlaybackType = FADE_IN_OUT;
			}

			if (movingPattern == "CONST_SPEED")
			{
				MovingPattern = CONST_SPEED;
			}
			else
			{
				MovingPattern = CONST_TDM;
			}

			TArray<TSharedPtr<FJsonValue>> pathObj = j.GetArrayField("pointList");
			for (int l = 0; l < pathObj.Num(); l++)
			{
				PathModeObject tempDot;
				tempDot.from_json(*pathObj[l]->AsObject());
				PointList.push_back(tempDot);
			}
		}
	};

	struct PathMode
	{
		vector<PathModeObjectCollection> Feedback;

		void to_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> feedbackValues;

			for (size_t i = 0; i < Feedback.size(); i++)
			{
				TSharedPtr<FJsonObject> pathmodeTemp= MakeShareable(new FJsonObject);
				Feedback[i].to_json(*pathmodeTemp);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(pathmodeTemp));
				
				feedbackValues.Add(JsonValue);
			}
			j.SetArrayField("Feedback", feedbackValues);
		}

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> feedbackObj = j.GetArrayField("feedback");

			for (int i = 0; i < feedbackObj.Num(); i++)
			{
				PathModeObjectCollection pathCollectionTemp;
				pathCollectionTemp.from_json(*feedbackObj[i]->AsObject());
				Feedback.push_back(pathCollectionTemp);
			}
		}
	};

	struct HapticEffectMode
	{
		int Texture;
		FeedbackMode Mode;
		DotMode DotMode;
		PathMode PathMode;

		void to_json(FJsonObject& j)
		{
			string modeValue ="";
			TSharedPtr<FJsonObject> dotModeValue = MakeShareable(new FJsonObject);
			TSharedPtr<FJsonObject> pathModeValue = MakeShareable(new FJsonObject);

			switch (Mode)
			{
			case bhaptics::PATH_MODE:
				modeValue = "PATH_MODE";
				break;
			case bhaptics::DOT_MODE:
				modeValue = "DOT_MODE";
				break;
			default:
				modeValue = "ERROR";
				break;
			}

			DotMode.to_json(*dotModeValue);
			PathMode.to_json(*pathModeValue);

			j.SetNumberField("Texture", Texture);
			j.SetStringField("Mode", modeValue.c_str());
			j.SetObjectField("DotMode", dotModeValue);
			j.SetObjectField("PathMode", pathModeValue);
		}

		void from_json(FJsonObject& j)
		{
			string modeEnum = (TCHAR_TO_UTF8(*j.GetStringField(TEXT("mode"))));
			
			if (modeEnum == "PATH_MODE")
			{
				Mode = PATH_MODE;
			}
			else if (modeEnum == "DOT_MODE")
			{
				Mode = DOT_MODE;
			}
			

			Texture = j.GetNumberField("texture");
			DotMode.from_json(*j.GetObjectField("dotMode"));
			PathMode.from_json(*j.GetObjectField("pathMode"));

		}
	};

	struct HapticEffect
	{
		int StartTime;
		int OffsetTime;
		std::map<std::string, HapticEffectMode> Modes;
		
		void to_json(FJsonObject& j)
		{
			TSharedPtr<FJsonObject> modeObject = MakeShareable(new FJsonObject);
			
			for (auto& m : Modes)
			{
				TSharedPtr<FJsonObject> effectObject = MakeShareable(new FJsonObject);

				m.second.to_json(*effectObject);
				modeObject->SetObjectField(m.first.c_str(), effectObject);
			}
			
			j.SetNumberField("StartTime", StartTime);
			j.SetNumberField("OffsetTime", OffsetTime);
			j.SetObjectField("Modes", modeObject);
		}

		void from_json(FJsonObject& j)
		{
			TMap<FString, TSharedPtr<FJsonValue>> modeValues = j.GetObjectField("modes")->Values;

			for (auto& kv : modeValues)
			{
				string key = TCHAR_TO_UTF8(*kv.Key);
				HapticEffectMode tempMode;
				tempMode.from_json(*kv.Value->AsObject());
				Modes[key] = tempMode;
			}

			StartTime = j.GetIntegerField("startTime");
			OffsetTime = j.GetIntegerField("offsetTime");
			
		}

	};

	struct Track
	{
		std::vector<HapticEffect> Effects;

		void to_json(FJsonObject& j)
		{

			TArray<TSharedPtr<FJsonValue>> effectValues;
			
			for (size_t i = 0; i < Effects.size(); i++)
			{
				TSharedPtr<FJsonObject> effectObject = MakeShareable (new FJsonObject);
				Effects[i].to_json(*effectObject);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(effectObject));
				effectValues.Add(JsonValue);
			}

			j.SetArrayField("Effects", effectValues);
		}

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> effectObject = j.GetArrayField("effects");

			for (int i = 0; i < effectObject.Num(); i++)
			{
				HapticEffect tempEffect;
				tempEffect.from_json(*effectObject[i]->AsObject());
				Effects.push_back(tempEffect);
			}
		}
	};

	struct LayoutObject
	{
		int Index;
		float X;
		float Y;

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("Index", Index);
			j.SetStringField("X", FString::SanitizeFloat(X));
			j.SetStringField("Y", FString::SanitizeFloat(Y));
		}

		void from_json(FJsonObject& j)
		{
			Index = j.GetIntegerField("index");
			X = j.GetNumberField("x");
			Y = j.GetNumberField("y");
		}
		
	};

	struct Layout
	{
		string Type;
		std::map<std::string, std::vector<LayoutObject>> Layouts;

		void to_json(FJsonObject& j)
		{
			TSharedPtr<FJsonObject> layout = MakeShareable(new FJsonObject);
			
			for (auto& l : Layouts)
			{
				TArray<TSharedPtr<FJsonValue>> jVec;

				for (size_t i = 0; i < l.second.size(); i++)
				{
					TSharedPtr<FJsonObject> layoutTemp = MakeShareable(new FJsonObject);
					l.second[i].to_json(*layoutTemp);
					TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(layoutTemp));
					jVec.Add(JsonValue);
				}

				layout->SetArrayField(l.first.c_str(), jVec);
			}
			j.SetStringField("Type", FString(Type.c_str()));
			j.SetObjectField("Layouts", layout);
		}

		void from_json(FJsonObject& j)
		{

			TMap<FString, TSharedPtr<FJsonValue>> layoutsObj = j.GetObjectField("layouts")->Values;
			Type = TCHAR_TO_UTF8(*j.GetStringField(TEXT("type")));

			for (auto& kv : layoutsObj)
			{
				string key = TCHAR_TO_UTF8(*kv.Key);
				TArray<TSharedPtr<FJsonValue>> layoutObjectValues = kv.Value->AsArray();
				vector<LayoutObject> tempLayoutObject;

				for (int i = 0; i < layoutObjectValues.Num(); i++)
				{
					LayoutObject tempLayout;
					tempLayout.from_json(*layoutObjectValues[i]->AsObject());
					tempLayoutObject.push_back(tempLayout);
				}
				Layouts[key] = tempLayoutObject;
			}

		}
	};

	struct HapticProject
	{
		std::vector<Track> Tracks;
		Layout Layout;

		void to_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> trackValues;
			TSharedPtr<FJsonObject> layoutObject = MakeShareable(new FJsonObject);

			for (size_t i = 0; i < Tracks.size(); i++)
			{
				TSharedPtr<FJsonObject> trackObject = MakeShareable(new FJsonObject);
				Tracks[i].to_json(*trackObject);
				TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(trackObject));
				trackValues.Add(JsonValue);
			}

			Layout.to_json(*layoutObject);

			j.SetArrayField("Tracks", trackValues);
			j.SetObjectField("Layout", layoutObject);
		}

		void from_json(FJsonObject& j)
		{
			TArray<TSharedPtr<FJsonValue>> trackObjs = j.GetArrayField(TEXT("Tracks"));

			for (int i = 0; i < trackObjs.Num(); i++)
			{
				Track tempTrack;
				tempTrack.from_json(*trackObjs[i]->AsObject());
				Tracks.push_back(tempTrack);
			}
			Layout.from_json(*j.GetObjectField("layout"));
		}
	};

	struct HapticFile
	{
		int intervalMillis;
		int size;
		int durationMillis;
		map<int, vector<HapticFeedback>> feedback;
		HapticProject project;

		void from_json (FJsonObject& j)
		{
			intervalMillis = j.GetIntegerField(TEXT("intervalMillis"));
			size = j.GetIntegerField(TEXT("size"));
			durationMillis = j.GetIntegerField(TEXT("durationMillis"));
			project.from_json(*j.GetObjectField("project"));
		}
	};

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

	struct TransformOption
	{
		double DeltaX;
		double DeltaY;
		bool IsValueRotate;

		//TransformOption(double deltaX, double deltaY, bool isValueRotate)
		//{
		//	DeltaX = deltaX;
		//	DeltaY = deltaY;
		//	IsValueRotate = isValueRotate;
		//}

		void to_json(FJsonObject& j)
		{
			j.SetNumberField("DeltaX", DeltaX);
			j.SetNumberField("DeltaY", DeltaY);
			j.SetBoolField("IsValueRotate", IsValueRotate);
		}

		string to_string()
		{
			string valRot = "";
			if (IsValueRotate)
			{
				valRot = "true";
			}
			else
			{
				valRot = "false";
			}
			string ret = "{ \"DeltaX\" : " + std::to_string(DeltaX) + ", \"DeltaY\" : "+ std::to_string(DeltaY)+ ", \"IsValueRotate\" : "+ valRot +"}";
			return ret;
		}
	};

	struct RegisterRequest
	{
		string Key;
		HapticProject Project;

		void to_json(FJsonObject& j)
		{
			TSharedPtr<FJsonObject> projectObject = MakeShareable(new FJsonObject);

			Project.to_json(*projectObject);
			j.SetStringField("Key", Key.c_str());
			j.SetObjectField("Project", projectObject);
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

	class BufferedHapticFeedback
	{
	public:
		BufferedHapticFeedback() {}
		map<int, vector<HapticFeedbackFrame>> feedbackMap;
		int StartTime;
		int EndTime;

		BufferedHapticFeedback(Position position, vector<DotPoint> points, int durationMillis, int interval)
		{
			int i;

			for (i = 0; i < durationMillis / interval; i++)
			{
				if (position == All)
				{

					HapticFeedbackFrame left(Left, points);
					HapticFeedbackFrame right(Right, points);
					HapticFeedbackFrame vBack(VestBack, points);
					HapticFeedbackFrame vFront(VestFront, points);
					HapticFeedbackFrame head(Head, points);
					int time = i * interval;
					feedbackMap[time] = { left, right, vBack, vFront, head };
				}
				else
				{
					feedbackMap[i * interval] = { HapticFeedbackFrame(position,points) };
				}


			}
			StartTime = -1;
			vector<HapticFeedbackFrame> f;
			vector<DotPoint> emptyPoints;
			if (position == All)
			{
				HapticFeedbackFrame left(Left, emptyPoints);
				HapticFeedbackFrame right(Right, emptyPoints);
				HapticFeedbackFrame vBack(VestBack, emptyPoints);
				HapticFeedbackFrame vFront(VestFront, emptyPoints);
				HapticFeedbackFrame head(Head, emptyPoints);
				f = { left, right, vBack, vFront, head };
			}
			else
			{
				f = { HapticFeedbackFrame(position,emptyPoints) };

			}

			feedbackMap[i * interval] = f;
			EndTime = i * interval;
		}

		BufferedHapticFeedback(Position position, vector<PathPoint> points, int durationMillis, int interval)
		{
			int i;

			for (i = 0; i < durationMillis / interval; i++)
			{
				if (position == All)
				{

					HapticFeedbackFrame left(Left, points);
					HapticFeedbackFrame right(Right, points);
					HapticFeedbackFrame vBack(VestBack, points);
					HapticFeedbackFrame vFront(VestFront, points);
					HapticFeedbackFrame head(Head, points);
					int time = i * interval;
					feedbackMap[time] = { left, right, vBack, vFront, head };
				}
				else
				{
					feedbackMap[i * interval] = { HapticFeedbackFrame(position, points) };
				}


			}
			StartTime = -1;
			vector<HapticFeedbackFrame> f;
			vector<DotPoint> emptyPoints;
			if (position == All)
			{
				HapticFeedbackFrame left(Left, emptyPoints);
				HapticFeedbackFrame right(Right, emptyPoints);
				HapticFeedbackFrame vBack(VestBack, emptyPoints);
				HapticFeedbackFrame vFront(VestFront, emptyPoints);
				HapticFeedbackFrame head(Head, emptyPoints);
				f = { left, right, vBack, vFront, head };
			}
			else
			{
				f = { HapticFeedbackFrame(position,emptyPoints) };

			}

			feedbackMap[i * interval] = f;
			EndTime = i * interval;
		}

		BufferedHapticFeedback(HapticFile hapticFile)
		{
			Initialise(hapticFile);
		}

		void Initialise(HapticFile hapticFeedbackFile)
		{
			StartTime = -1;
			EndTime = hapticFeedbackFile.durationMillis;
			auto feedback = hapticFeedbackFile.feedback;
			feedbackMap = {};

			for (auto keyValuePair = feedback.begin(); keyValuePair != feedback.end(); ++keyValuePair)
			{
				auto time = keyValuePair->first;
				auto feed = keyValuePair->second;

				vector<HapticFeedbackFrame> feedbackFrames(feed.size());

				for (size_t i = 0; i < feed.size(); i++)
				{
					if (feed[i].mode == DOT_MODE)
					{
						vector<DotPoint> points = vector<DotPoint>();
						auto values = feed[i].values;
						for (size_t index = 0; index < values.size(); index++)
						{
							if (values[index] > 0)
							{
								points.push_back(DotPoint(index,values[index]));
							}
						}

						feedbackFrames[i] = HapticFeedbackFrame(feed[i].position, points);
					}
					else if (feed[i].mode == PATH_MODE)
					{
						vector<PathPoint> points = vector<PathPoint>();
						auto values = feed[i].values;
						auto size = values[0];
						for (int index = 0; index < size; index++)
						{
							float x = values[3 * index + 1] / 40.0f;
							float y = values[3 * index + 2] / 30.0f;
							int intensity = values[3 * index + 3];

							points.push_back(PathPoint(x, y, intensity));
						}

						feedbackFrames[i] = HapticFeedbackFrame(feed[i].position, points);
					}
				}

				feedbackMap[time] = feedbackFrames;
			}
		}

		static BufferedHapticFeedback Copy(const BufferedHapticFeedback &signal, int interval, float intensityRatio, float durationRatio)
		{
			BufferedHapticFeedback feedbackSignal;
			feedbackSignal.EndTime = static_cast<int>(signal.EndTime * durationRatio / interval * interval) + interval;
			feedbackSignal.StartTime = -1;

			int time;
			for (time = 0; time < feedbackSignal.EndTime; time += interval)
			{
				int keyTime = static_cast<int>(time / durationRatio) / interval*interval;

				if (Common::containsKey(keyTime, signal.feedbackMap)) // contains
				{
					vector<HapticFeedbackFrame> hapticFeedbacks = signal.feedbackMap.at(keyTime);

					vector<HapticFeedbackFrame> copiedFeedbacks(hapticFeedbacks.size());

					for (size_t i = 0; i < hapticFeedbacks.size(); i++)
					{
						HapticFeedbackFrame hapticFeedback = hapticFeedbacks[i];
						vector<DotPoint> emptyDots;
						HapticFeedbackFrame feedback = HapticFeedbackFrame(hapticFeedback.position,emptyDots);

						for (auto point = hapticFeedback.dotPoints.begin(); point != hapticFeedback.dotPoints.end(); point++)
						{
							int val = (int)(point->intensity*intensityRatio);
							if (val > 100)
							{
								val = 100;
							}
							else if (val < 0)
							{
								val = 0;
							}
							DotPoint pt(point->index, val);
							feedback.dotPoints.push_back(pt);
						}

						for (auto point = hapticFeedback.pathPoints.begin(); point != hapticFeedback.pathPoints.end(); point++)
						{
							int val = (int)(point->intensity*intensityRatio);
							if (val > 100)
							{
								val = 100;
							}
							else if (val < 0)
							{
								val = 0;
							}
							PathPoint pt(point->x, point->y, val);
							feedback.pathPoints.push_back(pt);
						}

						copiedFeedbacks[i] = feedback;
						feedbackSignal.feedbackMap[time] = copiedFeedbacks;
					}
				}
			}

			return feedbackSignal;
		}

	};
		
}

#endif