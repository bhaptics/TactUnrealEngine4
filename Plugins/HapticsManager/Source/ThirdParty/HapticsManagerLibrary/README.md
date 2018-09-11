# bHaptics haptic devices C++ SDK
This project helps to utilize haptic devices in C++ engines
Current version is 1.4.0

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found at
   bHaptics webpage: [http://www.bhaptics.com](http://bhaptics.com/app.html)

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

## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Sept. 11th, 2018.


###### Copyright (c) 2018 bHaptics Inc. All rights reserved.
