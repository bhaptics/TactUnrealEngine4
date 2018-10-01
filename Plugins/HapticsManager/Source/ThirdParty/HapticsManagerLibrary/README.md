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
