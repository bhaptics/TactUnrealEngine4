//Copyright bHaptics Inc. 2017
/*
#ifndef BHAPTICS_PROJECT
#define BHAPTICS_PROJECT

#include <map>
//#include "model.hpp"
namespace bhaptics
{
	using namespace std;

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
	};

	struct DotModeObjectCollection
	{
		int StartTime;
		int EndTime;
		PlaybackType PlaybackType = PlaybackType::NONE;
		vector<DotModeObject> PointList;
	};

	struct DotMode
	{
		bool DotConnected;
		vector<DotModeObjectCollection> Feedback;
	};

	struct PathModeObject
	{
		float X;
		float Y;
		float Intensity;
		int Time;
	};

	struct PathModeObjectCollection
	{
		PlaybackType PlaybackType = PlaybackType::NONE;
		PathMovingPattern MovingPattern = PathMovingPattern::CONST_TDM;
		vector<PathModeObject> PointList;
	};

	struct PathMode
	{
		vector<PathModeObjectCollection> Feedback;
	};

	struct HapticEffectMode
	{
		int Texture;
		FeedbackMode Mode;
		DotMode DotMode;
		PathMode PathMode;
	};

	struct HapticEffect
	{
		int StartTime;
		int OffsetTime;
		map<std::string, HapticEffectMode> Modes;

	};

	struct Track
	{
		vector<HapticEffect> Effects;

	};

	struct LayoutObject
	{
		int Index;
		float X;
		float Y;
	};

	struct Layout
	{
		std::string Type;
		map<std::string, vector<LayoutObject>> Layouts;
	};

	struct HapticProject
	{
		vector<Track> Tracks;
		Layout Layout;

	};

	class Frame
	{
	public:
		int DurationMillis;
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
			return frame;
		}

		static Frame AsDotPointFrame(vector<DotPoint> points, bhaptics::Position position, int durationMillis, int texture = 0)
		{
			Frame frame;
			frame.Position = position;
			frame.DotPoints = points;
			frame.Texture = texture;
			return frame;
		}
	};

	struct RegisterRequest
	{
		std::string Key;
		HapticProject Project;
	};

	struct SubmitRequest
	{
		std::string Type;
		std::string Key;
		Frame Frame;
	};

	struct PlayerRequest
	{
		vector<RegisterRequest> Register;
		vector<SubmitRequest> Submit;
	};

	struct PlayerResponse
	{
		std::vector<std::string> RegisteredKeys;
		std::vector<std::string> ActiveKeys;
		int ConnectedDeviceCount;
		map<string, vector<int>> Status;
	};

	
}

#endif
/*/