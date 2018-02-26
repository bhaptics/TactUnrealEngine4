# bHaptics haptic devices Unreal Engine C++ plugin
This project helps to utilize haptic devices in Unreal Engine 4
Current version is 1.3.0

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found in
   bHaptics webpage: [http://www.bhaptics.com](http://bhaptics.com/app.html)

## How to integrate the plugin into existing projects
* If you have installed from the UE4 Marketplace, you can skip this section and go to the ['How to use the plugin'](#how-to-use-the-plugin) section.
* Copy the Plugins folder of the bHapticsManger project and paste it into either your project folder or into the Engine/Plugins folder.
* You should now have the plugin source code in either Plugins folder, as well as blueprints in the Plugins/HapticsManager/Content folder.
* For blueprint-only projects, in the case of any compiling errors, you will need to create a C++ class to generate the solution files for your project and rebuild the plugin.

## How to use the plugin
* From the editor, make sure that 'Show Plugin Content' is checked under View Options, on the bottom right of the Content Browser.
* Browse to 'HapticsManager/Content' and open the blueprints folder.
* Place the HapticsManager_BP into your game world. This blueprint actor will connect to the bHaptics Player and send the haptic feedback back to the devices.
* The Haptics Manager controls all the method calls to submit the feedback, as well as providing a handy debug UI to visualise the feedback as well, which can be hidden in game when not needed.
* The Haptic Manager has 3 different ways of submitting feedback: using preset haptic files, specifying specifc motors or 'dots' to play, or specifying a point on the device.

### Presets
* Presets can be constructed through using the bHaptics designer at https://designer.bhaptics.com
* After signing in, you can create a new project and design a certain feedback effect for use in the game.
* In order to use the feedback effect in Unreal, export the feedback file from the Designer, and create and place the new file into folder in the Contents directory in which to store all your feedback folders.
* In the HapticsManager_BP object, set the ProjectFeedbackFolder to the folder directory where you store all the feedback files, relative to the Contents directory.
* The files in this directory will be automatically loaded by the HapticsManager blueprint on play, and are stored using the file name as a key.
* To play the feedback effect, merely call the Submit Key function from the Haptics Manager, using the specified feedback file's name as a key.
* Example feedback files are given in the HapticsManager/Feedback folder in the Plugins directory. You can choose whether to access the feedback files from either the Plugins directory or the Project's contents directory through the UseProjectFeedbackFolder boolean variable. If true, it will use the files in the <Project>/Contents/<ProjectFeedbackFolder>, otherwise it will read from the Plugin's folder. It is set to true by default.
* If you wish to use feedback files outside of these folders, you can call the RegisterFeedback function to register a specific feedback file with its own Key; however, the path to the file must be manually inputted.
* Any feedback files used will also need to be copied when packaging the game, so it is recommended to keep any feedback files relative to the Content directory, and remember to include any folders containing feedback files when packaging, otherwise the files must be manually copied after packaging. See the [Packaging](#packaging) section for how to do this.

#### Playing Feedback Files
* There are three functions provided for playing the feedback from haptic feedback files: Submit Key, Submit Key with Intensity and Duration, and Submit Key with Transform.
* For haptic files in the HapticsManager/Feedback folder in either the plugin's folder, or the proejct's contents folder, keys are automatically generated using the file name, for ease of use. If you wish to change the key used, call the Register feedback function and provide key and feedback file to be assigned.
* The additional two functions add additional options to the alter the feedback file.
	* Submitting with Transform adds a RotationOption consisting of an Offset AngleX and an OffsetY, and the changes will only apply to feedback files for the Tactot haptic vest. The Offset Angle rotates the file around the body counter-clockwise, horizontally, by the given degrees, switching from front to the back when leaving the boundary of the front part of the vest, or vice versa. Similarly, OffsetY transforms the feedback up or down the body. This allows for a single impact feedback to be used anywhere on the body by rotating and transforming the horizontal and vertical components.
	* Submitting with Intensity and Duration adds a ScaleOption to the previous function, which adds scale variables to alter the original feedback's intensity and duration. For instance an intensity and/or duration scale(s) of 0.5 would halve the original intensity and duration of the feedback, while a value of 2.0 would double it, although the intensity will be capped at a value of 100, and neither value can be less than 0.
     * Both of the above methods also include an AltKey, or Alternative Key, in the case when a submitted feedback file key is already being played. If left blank, the submission call will interrupt the playing feedback and restart it; however if an AltKey is given, the two feedback files will be played together, with the second under the AltKey. This is useful in cases such as when a character is hit twice requiring the same feedback file to be played simultaneously.
* The feedback file itself is not altered by either of the above two calls, so all changes to the intensity or position of the feedback effects in the file are with regards to the original feedback. This is so the same feedback file can be used for situations such as impact effects, or effects where the duration and intensity change based on other variables or conditions in the game.

### Dots
* You can also access each individual motor using the SubmitFeedbackUsingDots function, which takes in an array of DotPoints as an input.
* A DotPoint represents an individual motor in the array, specifying which index the motor is and what intensity it should vibrate.
* The index ranges from 0 to 19 for the Tactosy and Tactot devices, and 0 to 6 for the tactal, and moves from left to right.
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
* All the functions for the plugin can be found in the bHaptics category in blueprints, or in the HapticsManagerActor.h file in the plugin.

## Testing
* In the HapticsManager Blueprint, some functionality has been implemented to help test and experiment with the functions outlined above.
* To test some basic functionality, run the game in Simulate mode and click on the HapticsManager blueprint actor in the game world. The functions and their variables can be found under the Default category or under the Blutilities category (for UE4.14/15) in the details panel. While simulate is running you can call the haptic feedback functions and provide variables to alter each call.
* The blueprint allows you to navigate any feedback files loaded already by the manager, and to play and alter the feedback using the SubmitKey, SubmitKeyWithIntensityDuration and SubmitKeyWithTransform functions, altering their respective variables under the default category in the details panel.
* You can also test out simple DotPoint and PathPoint patterns, by creating the points using the variables in the details panel, and setting the duration and device to play the feedback on, and then calling their respective functions to play the feedback.
* Using these functions you can get a good idea of how the haptic feedback will feel, as well as to test new ideas, especially testing any changes to existing feedback files.

## Packaging
* Before packaging ensure that the Feedback folder in the Contents/HapticsManager folder is also copied with the packaging. You can control this in the `Project Settings -> Packaging (advanced options) -> Additional Non-Asset Directories To Copy` and including the folder containing the feedback files there.
* If haptic feedback from the files are not playing in the packaged version, ensure that any additional file paths are all relative to the project directory, and that the folders are also correct in the packaged directory. 
* The plugin also uses a dll (bHapticUtility64.dll) to run the bHaptics Player if it is installed; however, this dll is not copied when packaging and must be done manually into the Plugins/HapticsManager/DLL directory. This functionality is optional and will not affect the haptic feedback.

## Samples 
* You can reference the blueprints in this project for how to implement different haptic feedback effects, as well as use the provided .tact feedback files.
* You can also check what presets are loaded by checking the list of keys in the HapticsManager actor in the editor, but only after placing the actor into the game world.
* While no C++ examples are provided, the logic is the same as with the blueprints: create a reference to the HapticManager in the game world, and then call the functions to play the feedback effects.

## Tips
* The bHaptics designer is a great tool for testing effects and trying out ideas, and can help with planning effects you want to create in-game.
* For the PathPoint structure, it is difficult to hit the edge of objects to fully activate the outer motors. To make the outer feedback stronger, one way is to multiply the points by 1.2, so the outer edge is easier to hit. You can experiment and see what makes your feedback feel better.
* PathPoints will automatically interpolate between motors, and the intensity will be shared between the two. If this feedback feels too weak for your situation, you can add more points clustered together to increase the feedback effect when interpolated between points.
* Be careful of naming the feedback effects, as 2 feedback effects with the same name will interrupt each other. Use the IsPlaying function to check if a specifc feedback effect is finished before sending the same feedback effect again, or submit with an AltKey so both feedback files play together.
* Be aware of the duration of your effects. You may need to add delays to ensure the submitted feedback has finished playing before submitting the next, in the case of a series of feedback calls to create the effect (ie. explosion, moving feedback).
* For the Submit Key With Transform, you can consider the torso as a cylinder and project the haptic feedback file onto that. You can then change the horizontal offset by finding the angle offset from the forward vector, and the OffsetY from the height on the cylinder to play. This will help the feedback feel more continuous as it moves from the front to the back or vice versa.
* Some simple examples are provided in a Feedback Effect Macro Library blueprint for reference, and you can use and adapt these functions to suit your needs.
* The ForLoops in blueprints can cause unexpected results with feedback, as it does not natively support delays. A ForLoopWithDelay macro is provided; however, if you are using loops and delays heavily, it is recommended to work in C++ for these functions.
* For further references, you can find our tutorial series at our youtube channel [here](https://www.youtube.com/channel/UCGyV7l7iZz9tUtEg4wvsOpA/playlists?shelf_id=0&sort=dd&view=1).

## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Jan. 9th, 2018.


###### Copyright (c) 2017 bHaptics Inc. All rights reserved.
