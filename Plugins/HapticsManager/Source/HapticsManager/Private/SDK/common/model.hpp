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
        VestFront = 201, VestBack = 202,
        GloveLeft = 203, GloveRight = 204,
        Custom1 = 251, Custom2 = 252, Custom3 = 253, Custom4 = 254
    };

    enum FeeddbackMode {
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
            intensity = _intensity;
        }
    };

    struct PathPoint
    {
        float x;
        float y;
        int intensity;

        PathPoint(float _x, float _y, int _intensity)
        {
            x = _x;
            y = _y;
            intensity = _intensity;
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
        FeeddbackMode mode;
        vector<uint8_t> values;

        HapticFeedback(Position _pos, const int _val[], FeeddbackMode _mod)
        {
            position = _pos;
            mode = _mod;
            values.assign(20, 0);
            for (int i = 0; i < 20; i++)
            {
                values[i] = _val[i];
            }
        }

        HapticFeedback(Position _pos, const vector<uint8_t> &_val, FeeddbackMode _mod)
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

    struct HapticFile
    {
        int intervalMillis;
        int size;
        int durationMillis;
        map<int, vector<HapticFeedback>> feedback;

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

    class BufferedHapticFeedback
    {
    public:
        BufferedHapticFeedback() {}
        map<int, vector<HapticFeedbackFrame>> feedbackMap;
        int StartTime;
        int EndTime;

		/**
        BufferedHapticFeedback(HapticFeedback feedback, int durationMillis, int interval)
        {
            int i;

            for (i = 0; i < durationMillis / interval; i++)
            {
                if (feedback.position == All)
                {

                    HapticFeedback left(Left, feedback.values, feedback.mode);
                    HapticFeedback right(Right, feedback.values, feedback.mode);
                    HapticFeedback vBack(VestBack, feedback.values, feedback.mode);
                    HapticFeedback vFront(VestFront, feedback.values, feedback.mode);
                    HapticFeedback head(Head, feedback.values, feedback.mode);
                    int time = i * interval;
                    feedbackMap[time] = { left, right, vBack, vFront, head };
                }
                else
                {
                    feedbackMap[i * interval] = { feedback };
                }


            }
            StartTime = -1;
            vector<HapticFeedback> f;
            int bytes[20] = { 0 };
            if (feedback.position == All)
            {
                HapticFeedback left(Left, bytes, feedback.mode);
                HapticFeedback right(Right, bytes, feedback.mode);
                HapticFeedback vBack(VestBack, bytes, feedback.mode);
                HapticFeedback vFront(VestFront, bytes, feedback.mode);
                HapticFeedback head(Head, bytes, feedback.mode);
                f = { left, right, vBack, vFront, head };
            }
            else
            {
                f = { feedback };

            }

            feedbackMap[i * interval] = f;
            EndTime = i * interval;
        }*/

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
			//int bytes[20] = { 0 };
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
			/*
            StartTime = -1;
            EndTime = hapticFile.durationMillis;
            feedbackMap = hapticFile.feedback;
			//*/
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

				for (int i = 0; i < feed.size(); i++)
				{
					if (feed[i].mode == DOT_MODE)
					{
						vector<DotPoint> points = vector<DotPoint>();
						auto values = feed[i].values;
						for (int index = 0; index < values.size(); index++)
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

	void to_json(FJsonObject& j, const PathPoint& point)
	{
		j.SetNumberField("X", point.x);
		j.SetNumberField("Y", point.y);
		j.SetNumberField("Intensity", point.intensity);
	}

	void to_json(FJsonObject& j, const DotPoint& point)
	{
		j.SetNumberField("Index", point.index);
		j.SetNumberField("Intensity", point.intensity);
	}

	void to_json(FJsonObject& jsonObject, const HapticFeedbackFrame& feedback)
	{
		vector<std::string> tempVec1, tempVec2;

		//vector<json> jVec1, jVec2;
		TArray<TSharedPtr<FJsonValue>> jVec1, jVec2;

		for (size_t i = 0; i < feedback.pathPoints.size(); i++)
		{
			TSharedPtr<FJsonObject> jTemp = MakeShareable(new FJsonObject);
			to_json(*jTemp, feedback.pathPoints[i]);
			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(jTemp));
			jVec1.Add(JsonValue);

		}

		for (size_t i = 0; i < feedback.dotPoints.size(); i++)
		{
			//json jTemp;
			TSharedPtr<FJsonObject> jTemp = MakeShareable(new FJsonObject);
			to_json(*jTemp, feedback.dotPoints[i]);
			//jVec2.push_back(jTemp);
			TSharedRef<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject(jTemp));
			jVec2.Add(JsonValue);
			//tempVec2.push_back(jTemp.dump());
		}

		jsonObject.SetNumberField("Position", feedback.position);
		jsonObject.SetNumberField("Texture", feedback.texture);
		jsonObject.SetArrayField("PathPoints", jVec1);
		jsonObject.SetArrayField("DotPoints", jVec2);
		//j = json{ { "Position",feedback.position },{ "Texture",feedback.texture },{ "PathPoints",jVec1 } ,{ "DotPoints", jVec2 } };
	}
		
}

#endif