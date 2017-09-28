# bHaptics haptic devices Unreal Engine C++ plugin
This project helps to utilize haptic devices in Unreal Engine 4
Current version is 1.0.0

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found in
   bHaptics webpage: [http://www.bhaptics.com](http://bhaptics.com/app.html)

## How to integrate the plugin into existing projects
* Copy the plugin and content folders of the bHapticsManger project and paste them into your project folder.
* You should now have the plugin source code in your plugins folder, and the bHapticsManager folder in the contents folder.
* For C++ projects, in the build configuration file found in the source folder (<YourProjectName>.Build.cs) add "HapticsManager"
  to the list of public dependency module names, and generate the solution files again to ensure the plugins folder is included.
* For blueprint-only projects, there may be compiling errors, in which case you will need to create a c++ class to generate the solution files for
  your project.

## How to use the plugin
* From the editor place the HapticsManager_BP into your game world. This blueprint actor will connect to the bHaptics Player and send the haptic
  feedback back to the devices.
* The Haptics Manager controls all the method calls to submit the feedback, as well as providing a handy debug UI to visualise the feedback as well,
  which can be hidden in game when not needed.
* The Haptic Manager has 3 different ways of submitting feedback: using preset haptic files, specifying specifc motors or 'dots' to play,
  or specifying a point on the device.

** Presets
* Presets can be constructed through using the bHaptics designer at https://designer.bhaptics.com
* After signing in, you can create a new project and design a certain feedback for use in the game.
* In order to use the feedback in Unreal, export the feedback from the Designer and place the new file into the bHapticsManager/Feedbacks
  folder in the Contents directory.
* The files are automatically loaded by the HapticsManager blueprint on play, and are stored using the file name as a key.
* To play the feedback, merely call the SubmitFeedback function from the Haptics Manager, using the specified feedback as a key.

** Dots
* You can also access each individual motor using the SubmitFeedbackUsingDots function, which takes in an array of DotPoints as an input.
* A DotPoint represents an individual motor in the array, specifying which index the motor is and what intensity it should vibrate.
* The index ranges from 0 to 19 for the tactosy and tactot devices, and 0 to 6 for the tactal, and moves from left to right.
* Since the input for this function is an array, you can send multiple dots as a feedback effect at once.

** Paths
* To give a better sense of location on the devices, SubmitFeedbackUsingPaths can be called using the PathPoints structure.
* PathPoints give the illusion that the feedback is coming from a point between two motors and can be used to make the feedback seem more continuous.
* The structure consists of an X and Y coordinate on the device plane, as well as the intensity of the feedback.
* The coordinates range from the top-left of the device as (0,0) to the bottom-right as (1,1).
* Multiple points can be submitted together with the feedback effect.

**Other Functionality
* Other functions are provided to help give more freedom in developing different kinds of feedback.
* For example, checks for whether a specified feedback is playing are provided as well as functions to cease the feedback.
* These can be used to add effects such as interrupt feedback, or even to loop a certain feedback.

##Packaging
* As of yet, there are still some issues with the plugin when packaging a project, and some additional steps are needed to work around them.
* Some source code requires exceptions, and so exceptions must be enabled by adding 'UEBuildConfiguration.bForceEnableExceptions = true;' to the project's
  TargetRules.
* Before packaging ensure that the Feedbacks folder in the Contents/bHapticsManager folder is also copied with the packaging. You can control this in the
  Project Settings -> Packaging (advanced options) -> Additional Non-Asset Directories To Copy.
* The plugin also uses a dll (bHapticUtility64.dll) to run the bHaptics Player if it is installed; however, this dll is not copied when packaging and must
  be done manually into Plugins/HapticsManager/ directory. However, this functionality is optional and will not affect the haptic feedback.
* The above issues are currently still being addressed and should be resolved in future updates to make packaging easier for developers.

## Samples 
* Sample feedback effect files are provided in the bHapticsManager/Feedbacks folder as well as some example blueprints.
* There are 3 dummies provided, showing how to implement feedback for DotPoints, PathPoints, and how to create new effects such as an explosion.
* There is also a FPSHapticsPawn exmple blueprint as an example implementation for using the preset feedback, where the number keys are used to play
  feedback files.
* You can also check what presets are loaded by checking the list of keys in the HapticsManager actor in the editor, after placing the actor into the game world.
* While no C++ examples are provided, the logic the same as with the blueprints: create a reference to the HapticManager which is instanced in the game world
  and then call the functions to play the feedback effects.

## Tips
* The bHaptics designer is a great tool for testing effects and trying out ideas, and can help with planning effects you want to create in-game.
* For the PathPoint structure, it is difficult to hit the edge of objects to fully activate the outer motors. To make the outer feedback stronger,
  one way is to multiply the points by 1.2, so the outer edge is easier to hit. You can experiment and see what makes your feedback feel better.
* PathPoints will automatically interpolate between motors, and the intensity will be shared between the two. If this feedback feels too weak for
  your situation, you can add more points clustered together to increase the feedback effect when interpolated between points.
* Be careful of naming the feedback effects, as 2 feedback effects with the same name will interrupt each other. Use the IsPlaying function to check if a 
  specifc feedback effect is finished before sending the same feedback effect again.
* Be aware of the duration of your effects. You may need to add delays to ensure the submitted feedback has finished playing before submitting the next,
  in the case of a series of feedback calls to create the effect (ie explosion, moving feedback)


## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Sept 26th, 2017.


###### Copyright (c) 2017 bHaptics Inc. All rights reserved.
