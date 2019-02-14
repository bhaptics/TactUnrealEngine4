# bHaptics haptic devices Unreal Engine C++ plugin
This project helps to utilize haptic devices in Unreal Engine 4
Current version is 1.4.2

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found at
   bHaptics webpage: [http://www.bhaptics.com](http://bhaptics.com/app.html)

## Changes
* Updated UI for new devices.
* Added ForearmL and ForearmR for latest version of Tactosy
  * Replaces Left and Right positions for the latest Tactosy device.
  * Previous versions are still supported through the Left and Right positions.

## How to integrate the plugin into existing projects
* If you have installed from the UE4 Marketplace, you can skip this section and go to the ['How to use the plugin'](#how-to-use-the-plugin) section.
* Copy the Plugins folder of the bHapticsManger project and paste it into either your project folder or into the Engine/Plugins folder.
* Navigate to the Source/ThirdParty/HapticsManagerLibrary folder in the plugin, and build the HapticLibrary.sln found there.
    * This will generate the library and DLL files necessary to work with the C++ library in UE4.
* You should now have the plugin source code in either Plugins folder, as well as blueprints in the Plugins/HapticsManager/Content folder.
* For blueprint-only projects, in the case of any compiling errors, you may need to create a C++ class to generate the solution files for your project and rebuild the plugin.

## How to use the plugin
* For C++ projects, include the HapticsManager Module in the projects Build file.
```
PublicDependencyModuleNames.AddRange(
	new string[]
	{
	...,
	"HapticsManager"
	});
```
* On the actor from which you wish to call the feedback, add the Haptic Manager Component to the actor.
* You can now call any of the SDK's functions by referencing the component.
* The Haptic Manager has 3 different ways of submitting feedback: using preset haptic files, specifying which motors or 'dots' to play, or specifying a point on the device to play the feedback.

### Presets
* Presets can be constructed through using the bHaptics designer at https://designer.bhaptics.com
* After signing in, you can create a new project and design a feedback effect for use in the game.
* In order to use the feedback effect in Unreal, export the feedback file from the Designer, and simply drag and drop the file into your Unreal project, or you can import the files using the import function in the Content Browser.
* The files will be automatically converted into a UAsset, and you can view the feedback effect's identifying Key, the device it will be played on, and the duration of the effect
* Example feedback files are given in the Feedback folder in the Plugin's contents directory, to help get you started.

#### Playing Feedback Files
* There are three functions provided for playing the feedback from haptic feedback files: Submit Feedback, Submit Feedback with Intensity and Duration, and Submit Feedback with Transform.
* To play a given Feedback File, simply call the Submit Feedback function and select the Feedback File to be played.
* The additional two functions add additional options to the alter the feedback file for any dynamic changes needed.
	* Submitting with Transform adds a RotationOption, with Offset-Angle and Offset-Y, which will transform any Tactot feedback files. The Offset Angle rotates the file around the body counter-clockwise, horizontally, by the given degrees, switching from front to the back when leaving the boundary of the front part of the vest, or vice versa. Similarly, OffsetY transforms the feedback up or down the body. This allows for a single impact feedback to be used anywhere on the body by rotating and transforming the horizontal and vertical components.
	* Submitting with Intensity and Duration adds a ScaleOption to the previous function, which adds scale variables to alter the original feedback's intensity and duration. For instance an intensity and/or duration scale(s) of 0.5 would halve the original intensity and duration of the feedback, while a value of 2.0 would double it, although the intensity will be capped at a value of 100, and neither value can be less than 0. This can be applied to any device's feedback file.
     * Both of the above methods also include an AltKey, or Alternative Key, in the case when a submitted feedback file key is already being played. If left blank, the submission call will interrupt the playing feedback and restart it; however if an AltKey is given, the two feedback files will be played together, with the second under the AltKey. This is useful in cases such as when a character is hit twice requiring the same feedback file to be played simultaneously.
* These functions allow feedback files to be dynamically altered while in-game, while not affecting the file itself, allowing for a single file to be adapted to multiple situations.

### Dots
* You can also access each individual motor using the SubmitFeedbackUsingDots function, which takes in an array of DotPoints as an input.
* A DotPoint represents an individual motor in the array, specifying which index the motor is and at what intensity it should vibrate.
* The index ranges from 0 to 19 for the Tactosy and Tactot devices, 0 to 6 for the tactal, and 0 to 9 for other devices, and moves from left to right.
* Since the input for this function is an array, you can send multiple dots as a feedback effect at once.

### Paths
* To give more of a location-specific effect, SubmitFeedbackUsingPaths can be called using the PathPoints structure.
* PathPoints give the illusion that the feedback is coming from a point between two motors and can be used to make the feedback effect seem more continuous.
* The structure consists of an X and Y coordinate on the device plane, as well as the intensity of the feedback.
* The coordinates range from the top-left of the device as (0,0) to the bottom-right as (1,1).
* Multiple points can be submitted together with the feedback effect.

### Other Functionality
* Other functions are provided to help give more freedom in developing different kinds of feedback effects.
* For example, checks for whether a specified feedback effect is playing are provided as well as functions to cease the feedback effect.
* These can be used to add effects such as interrupting feedback effects, or even to loop a certain feedback effect.
* The ProjectToVest function is provided to simplify collision, taking a location and component and converting it to Cylindrical coordinates and returning as a RotationOption. You can use this and the customisable CustomProjectToVest to quickly give feedback to the vest.
* All the functions for the plugin can be found in the bHaptics category in blueprints, or in the HapticsManagerComponent.h file in the plugin.

## Testing
* In the HapticsManager Blueprint, some functionality has been implemented to help test and experiment with the functions outlined above.
* To view the feedback in-game, with or without the devices connected, add the HapticsManager blueprint in the plugin's Content/Blueprints folder, which functions as both a UI to view the feedback, as well as test simple functionality.
* To test, run the game in Simulate mode and click on the HapticsManager blueprint actor in the game world. The functions and their variables can be found under the Default category or under the Blutilities category (for UE4.14/15) in the details panel. While simulate is running you can call the haptic feedback functions and provide variables to alter each call.
* The blueprint allows you to select any feedback files in the project, and to play and alter the feedback using the Play Feedback, Play Feedback With Intensity Duration and Submit Feedback With Transform functions, altering their respective variables under the default category in the details panel.
* You can also test out simple DotPoint and PathPoint patterns, by creating the points using the variables in the details panel, and setting the duration and device to play the feedback on, and then calling their respective functions to play the feedback.
* Using these functions you can get a good idea of how the haptic feedback will feel, as well as to test new ideas, especially testing any changes to existing feedback files.

## Samples 
* You can reference the blueprints in this project for how to implement different haptic feedback effects, as well as how to use the Feedback Files.
* While no C++ examples are provided, the logic is the same as with the blueprints by calling the functions provided by the Haptics Manager Component.

## Tips
* The bHaptics designer is a great tool for testing effects and trying out ideas, and can help with planning effects you want to create in-game.
* PathPoints will automatically interpolate between motors, and the intensity will be shared between the two. If this feedback feels too weak for your situation, you can add more points clustered together to increase the feedback effect when interpolated between points.
* Be careful of naming the feedback effects, as 2 feedback effects with the same name will interrupt each other. Use the IsPlaying function to check if a specifc feedback effect is finished before sending the same feedback effect again, or submit with an AltKey so both feedback files play together.
* Be aware of the duration of your effects. You may need to add delays to ensure the submitted feedback has finished playing before submitting the next, in the case of a series of feedback calls to create the effect (ie. explosion, moving feedback).
* For the Submit Key With Transform, you can consider the torso as a cylinder and project the haptic feedback file onto that. You can then change the horizontal offset by finding the angle offset from the forward vector, and the OffsetY from the height on the cylinder to play. This will help the feedback feel more continuous as it moves from the front to the back or vice versa. You can see the KeyDummy blueprint for an example of how to do this.
* Some simple examples are provided in the Plugin's Content/Blueprints folder, with Dummies for collision detection examples, as well some simple macro effects in the Macro Effect Library .
* The ForLoops in blueprints can cause unexpected results with feedback, as it does not natively support delays. A ForLoopWithDelay macro is provided; however, if you are using loops and delays heavily, it is recommended to work in C++ for these functions, or use the designer for the feedback.
* For further references, you can find our tutorial series at our youtube channel [here](https://www.youtube.com/watch?v=Dy2D4Jnx-Io&t=2s&list=PLfaa78_N6dlvd0Ha0s0Y_LT62-Oqp8N2A&index=3).

## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Feb. 14th, 2019.


###### Copyright (c) 2017-2019 bHaptics Inc. All rights reserved.
