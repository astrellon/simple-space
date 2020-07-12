# Design Scratchpad

## Sprites

- Built in animations
  - Animations are key value pairs

### Animation

- definition
- currentAnimation
- currentTime;
- nextAnimation;
- timeScale;

### AnimationDefinition

- key: string
- value: AnimationTimeline

### AnimationTimeline

Look to CSS for basic inspiration.

#### TextureAnimationLayer

Basically a list of indices into a texture atlas with some timing info

- texture
- default key frame times
- list of key frames (index and timing on how long to stay on frame)

## Teleportation effect

Dissolve effect for other character.
Dissolve the world for the player, so the player stays the same maybe with a glowing outside and then use two cameras to show the source and destination dissolving between the two locations.