# ReSpeed

This mod allows you to <cy>customise</c> the speed of each speed portal. You can fine tune the speed of each portal to your liking, or even randomise the speeds of each portal if you prefer chaos in your life, featuring dedicated <cg>minimum</c> and <cb>maximum</c> speeds! If you're a psychopath, you can even change speeds for Players 1 <cr>AND</c> 2 separately! Or you could just... <co>not</c> do any of this, and disable the entire mod in its settings without having to restart.

## NOTE:
These are the values used by the game's logic to determine the speed of the player at each portal. More technically, these are the floats used in the *p0* argument for the function PlayerObject::updateTimeMod(float p0, bool p1), which controls the speed of the player at each portal:
- 0.5x portal: 0.7f
- 1.0x portal: 0.9f
- 2.0x portal: 1.1f
- 3.0x portal: 1.3f
- 4.0x portal: 1.6f
