# PaperZDForSpine2D

This plugin gives support for running Spine2D sequences on PaperZD, must have both plugins installed for this to work.

This plugin contains:
- An specialized AnimationSource and AnimSequences used to hold the Spine2D render data for use in the PaperZD plugin.
- Internal "Playback Handles" that interface with the Spine2D plugin and direct it on how to render the PaperZD animations.
- Support for "Layered" animations which interface with the Spine2D track system.

Install:
--------

For installing this plugin, copy the folder **PaperZDForSpine2D** into your project plugins folder (creating it if it doesn't exist).
Path should look like this
`MyProject/Plugins/PaperZDForSpine2D`

After that, you should be able to run your project and it will prompt a build which you should accept.
The plugin is now ready to use.

First setup:
--------

After install, you can start creating your first "Animation Source". The plugin will recognize that Spine2D is enabled and will ask whether you want to use Paper2D or Spine2D for this source. 
When opening the newly created Animation Source you simply need to setup the "Atlas" and "SkeletonDataAsset" spine objects so the Animation source knows which skeleton you which to use.

Note: Due to the way Spine2D is coded, if you setup the "Atlas" and "SkeletonDataAsset" in an incorrect manner (they don't match), an error message will trigger from the Spine2D plugin (which can trigger multiple times). If this happens, simply change the assets so they are correctly setup.

Limitations:
--------

Because this plugin connects PaperZD with Spine2D, some compatibility issues might arise as the Spine2D plugin might work in a different manner as PaperZD expects and any changes to the codebase could break the compatibility. 
Some considerations follow:

- The animation blending only happens with the last animation that ran in the layer.
- The animation blending updates at the rate of the world, due to how the PaperZD and Spine2D render pipeline differ.
- While PaperZD allows for negative playback ratio on the AnimSequences, Spine2D does not allow that and some compatibility issues might arise if you attempt that (like blending being reversed).
- An animation source can only render a single skeleton. Hot swapping skeletons is not currently supported.