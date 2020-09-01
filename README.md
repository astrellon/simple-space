# Simple Space (Game)
A simple space game written in C++ and SFML.

## Dependencies

Ubuntu
```sh
sudo apt install g++ build-essential cmake libudev-dev libxrandr-dev libopenal-dev libvorbis-dev libfreetype-dev libglew-dev libbox2d-dev doctest-dev
```

You will also need to pull the git submodules.
The only dependencies at the moment is `tmxlite` which is a [Tiled](https://www.mapeditor.org/) map parser, which is used for planet surfaces.

```sh
git submodule init
git submodule update
```

To run just execute `run.sh`.
