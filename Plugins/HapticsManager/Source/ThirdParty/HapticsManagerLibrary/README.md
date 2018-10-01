# bHaptics haptic devices C++ SDK
This project helps to utilize haptic devices in C++ engines
Current version is 1.4.0

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found at
   bHaptics webpage: [http://www.bhaptics.com](https://www.bhaptics.com/download/)

## Haptic Player
* To simplify device management and feedback calls, this SDK connects to the bHaptics Player, which will manage the devices and send the Haptic signals to each device.
* This SDK uses a simple web socket to connect to the Player, and so this connection needs to be initialised when the game begins.
* You can check if the connection is initialised by navigating to the settings page on the Player and checking the "Number of Clients" value. If this increments when the game begins and decrements when it closes then the connection is working.
* A JSON structure of 'PlayerRequest' consisting of a RegisterRequest to register a Haptic File for later playback, or SubmitRequest to make an actual feedback call. You can find these structures in the model.h file.
# Examples of the JSON strcutures are found below.

## Haptic Library
* Refer to the HapticLibrary files for the functions for Haptic Feedback.
* You can use the built DLL and LIB files to integrate the haptic feedback into the Engine, or re-implement the HapticLibrary files in your engine to access the functionality.

## Preset Feedback Files
* For .tact feedback files downloaded from the bHaptics Designer, the files must be parsed and registered.
* The file stores a JSON structure which describes the feedback effect.
* You can either use your own JSON Parser and pass the Project attribute to the SDK, or pass the file path and let the SDK load and parse the file itself.
* See the util files for the implementation of this parser.

## Example Integration
* This project is a part of our UE4 Plugin.
* You can refer to the HapticsManager/Private/HapticManagerComponent.cpp file and its associated header file for how to integrate the library into an engine.

## JSON Examples
* Submit Path Point Example
```
{ 
  "Register" : 
    [],
  "Submit" : 
  [
    { 
      "Type" : "frame",
       "Key" : "BodyHit",
       "Frame" : 
        { 
          "DurationMillis":200,
          "Position": 201,
          "Texture": 0,
          "DotPoints": [],
          "PathPoints": 
          [
            { "X":0.746000, "Y": 0.210000, "Intensity": 100, "MotorCount": 3}
          ]
        }
      }
    ]
}
```

*Register File Example
```
{
  "Register" :
    [
      { 
        "Key" : "ShotgunImpact",
        "Project" : {"id":458,"name":"ShotgunImpact","column":0,"row":0,"mediaFileUrl":null,"thumbnailUrl":null,"mediaFileDuration":0.20000000000000001,"tracks":[{"id":915,"enable":true,"projectId":458,"effects":[{"id":1769,"trackId":915,"name":"path","startTime":0,"offsetTime":200,"priority":0,"modes":{"VestFront":{"texture":0,"mode":"PATH_MODE","dotMode":{"dotConnected":false,"feedback":[{"startTime":0,"endTime":200,"playbackType":"NONE","pointList":[]}]},"pathMode":{"feedback":[{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[{"x":"0.500","y":"0.500","intensity":"1.000","time":0}]},{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[{"x":"0.623","y":"0.374","intensity":"1.000","time":0}]},{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[{"x":"0.623","y":"0.626","intensity":"1.000","time":0}]},{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[{"x":"0.382","y":"0.638","intensity":"1.000","time":0}]},{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[{"x":"0.379","y":"0.378","intensity":"1.000","time":0}]}]}},"VestBack":{"texture":0,"mode":"PATH_MODE","dotMode":{"dotConnected":false,"feedback":[{"startTime":0,"endTime":200,"playbackType":"NONE","pointList":[]}]},"pathMode":{"feedback":[{"playbackType":"NONE","movingPattern":"CONST_SPEED","pointList":[]}]}}}}]},{"id":916,"enable":true,"projectId":458,"effects":[]}],"hapticEffectSize":0,"parentId":82,"creationTime":null,"updateTime":"2018-01-31T04:48:52.000Z","peaks":null,"layout":{"id":3,"name":"Tactot","layouts":{"VestFront":[{"index":0,"x":"0.000","y":"0.000","distance":0},{"index":1,"x":"0.333","y":"0.000","distance":0},{"index":2,"x":"0.667","y":"0.000","distance":0},{"index":3,"x":"1.000","y":"0.000","distance":0},{"index":4,"x":"0.000","y":"0.250","distance":0},{"index":5,"x":"0.333","y":"0.250","distance":0},{"index":6,"x":"0.667","y":"0.250","distance":0},{"index":7,"x":"1.000","y":"0.250","distance":0},{"index":8,"x":"0.000","y":"0.500","distance":0},{"index":9,"x":"0.333","y":"0.500","distance":0},{"index":10,"x":"0.667","y":"0.500","distance":0},{"index":11,"x":"1.000","y":"0.500","distance":0},{"index":12,"x":"0.000","y":"0.750","distance":0},{"index":13,"x":"0.333","y":"0.750","distance":0},{"index":14,"x":"0.667","y":"0.750","distance":0},{"index":15,"x":"1.000","y":"0.750","distance":0},{"index":16,"x":"0.000","y":"1.000","distance":0},{"index":17,"x":"0.333","y":"1.000","distance":0},{"index":18,"x":"0.667","y":"1.000","distance":0},{"index":19,"x":"1.000","y":"1.000","distance":0}],"VestBack":[{"index":0,"x":"0.000","y":"0.000","distance":0},{"index":1,"x":"0.333","y":"0.000","distance":0},{"index":2,"x":"0.667","y":"0.000","distance":0},{"index":3,"x":"1.000","y":"0.000","distance":0},{"index":4,"x":"0.000","y":"0.250","distance":0},{"index":5,"x":"0.333","y":"0.250","distance":0},{"index":6,"x":"0.667","y":"0.250","distance":0},{"index":7,"x":"1.000","y":"0.250","distance":0},{"index":8,"x":"0.000","y":"0.500","distance":0},{"index":9,"x":"0.333","y":"0.500","distance":0},{"index":10,"x":"0.667","y":"0.500","distance":0},{"index":11,"x":"1.000","y":"0.500","distance":0},{"index":12,"x":"0.000","y":"0.750","distance":0},{"index":13,"x":"0.333","y":"0.750","distance":0},{"index":14,"x":"0.667","y":"0.750","distance":0},{"index":15,"x":"1.000","y":"0.750","distance":0},{"index":16,"x":"0.000","y":"1.000","distance":0},{"index":17,"x":"0.333","y":"1.000","distance":0},{"index":18,"x":"0.667","y":"1.000","distance":0},{"index":19,"x":"1.000","y":"1.000","distance":0}]},"type":"Vest","public":true}}
      }
    ],
  "Submit" : []
}
```

*Submit File Example
```
{
  "Register" :
    [],
  "Submit" :
    [
      {
        "Type" : "key",
        "Key" : "ShotgunImpact",
        "Parameters": 
          {
            "altKey": "GunShot1",
            "rotationOption": "{ \"offsetAngleX\" : -11.664018, \"offsetY\" : 0.237115}","scaleOption": "{ \"intensity\" : 1.000000, \"duration\" : 1.000000}"
          },
        "Frame" : { "DurationMillis":0, "Position": 53995, "Texture": 0, "DotPoints": [], "PathPoints": []}
      }
    ]
}
```

## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Oct. 1st, 2018.


###### Copyright (c) 2018 bHaptics Inc. All rights reserved.
