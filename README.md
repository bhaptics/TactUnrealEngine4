# bHaptics Haptic Manager UE4 Project Template
This template project demonstrates how to integrate the bHaptics plugin into a project. Contains blueprints to get started with integrating haptics into your UE4 project.

## Prerequisite
* bHaptics Player has to be installed (Windows)
   * The app can be found in
   bHaptics webpage: [http://www.bhaptics.com](http://bhaptics.com/app.html)
* Tested on UE 4.16 and 4.17.

## Contents
### Blueprints
* HapticsManager_BP - Blueprint to access the functionality of the bHaptics plugin.
* FPSHapticsPawn - A simple FPS pawn with raycasting to 'shoot' objects, and is an example on how to map haptic feedback to user input.
* 3 Dummy blueprints - Examples of how to incorporate feedback into objects using collision detection.
   * BP_Dummy - Demonstrates the use of the DotPoint structure in making haptic feedback, using separate colliders for each motor.
   * BP_DummySimple - Demonstrates the use of the PathPoint structure is making haptic feedback, and uses collision detection to generate the feedback.
   * BP_DummyExplosion - Demonstated the implementation of more advanced effects beyond single motor calls, by creating a fading 

### Feedbacks
* Folder for storing preset haptic feedback effects created in the bHaptics Designer.
* All feedback files stored here are loaded automatically by the bHapticsManager_BP to be called used during gameplay.

### Maps
* DemoMap - Example map using the blueprints in a simple level where you can test various haptic feedback.

## Using the Example Content
* Open the DemoMap level and click play. You will spawn in the world with a red cross-hair in the centre of the screen.
* Pressing the left mouse button will 'fire' at the object in the cross-hair, as well as play a feedback on the tactosy devices.
* Hitting the Dummies or clicking on the devices in the UI will cause the 'dots' to light up and the respective device to vibrate.
* Pressing the Numerical keys from 0-9 will play the first 10 feedback effects loaded by the HapticsManager alphabetically.
* Additional effects are also mapped to the 'U' and 'T' keys to demonstrate how to change the duration and intensity of a feedback.
* The 'P' key will toggle the haptic feedback on and off, while the '-' and '=' keys set it off or on respectively.
* You can check and alter these key events in the FPSHapticsPawn blueprint.

## Notes
* The dummies are facing forwards by default, and so hits will trigger the front vest to vibrate. To test the back motors, rotate the dummies, so you can still see the visualisation during the demo.
* This is merely a demo template demonstrating the use of the HapticManager blueprint and actor in a game level. To implement this into your existing levels, please read the Plugin README in the plugins folder of this project for more details on the plugin itself.
* For tutorials into creating your own haptic feedback and implementing the functions shown in this template, see our tutorial playlist [here](https://www.youtube.com/playlist?list=PLfaa78_N6dlvd0Ha0s0Y_LT62-Oqp8N2A).

## Contact
* Official Website: http://www.bhaptics.com/
* E-mail: contact@bhaptics.com

Last update of README.md: Sept 26th, 2017.


###### Copyright (c) 2017 bHaptics Inc. All rights reserved.
