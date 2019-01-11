# PaperZDForSpine2D

This plugin gives support for running Spine2D sequences on PaperZD, must have both plugins installed for this to work.

This plugin contains:
- An specialized UPaperZDAnimSequence_Spine2D that interfaces with Spine2D and knows how to updates its playback.
- A ready to use APaperZDCharacter_Spine2D that pre-configures PaperZD's AnimPlayer, registering the Spine2D sequences to the intended RenderComponent.

Install:
--------

For installing this plugin, copy the folder **PaperZDForSpine2D** into your project plugins folder (creating it if it doesn't exist).
Path should look like this
`MyProject/Plugins/PaperZDForSpine2D`

After that, you should be able to run your project and it will prompt a build which you should accept.
After that you can start freely using this plugin.

Advanced Instructions:
---------------------

For use of external animation sources for PaperZD first you need to have an inherited class from UPaperZDAnimSequence, this fork already gives one already configured class.

After creating the class, you must link the AnimSequence class with its render component, so the AnimPlayer (PaperZD class responsible of AnimSequence Playback) knows which child component to use when updating the playback.

For this, APaperZDCharacter inherits from IPaperZDAnimInstanceManager and provides the method ConfigurePlayer, which handles the created AnimInstance's player and expects the user to configure it.

In order to link Render+Sequence you must call:

Player->RegisterRenderComponent(TSubclassOf<UPaperZDAnimInstance>,UPrimitiveComponent*)

This is a BlueprintNativeEvent, so you can implement it on blueprint or in c++. Default implementation links Paper2D Flipbook component to the Flipbook sequences.

**IMPORTANT: This plugin already contains a ready to use APaperZDCharacter_Spine2D class that registers its component**
