# liblub

An OpenGL 4 and 3.3 Core Framework.

## Dependencies

You need the following packages on Ubuntu.

`$ sudo apt-get install build-essential cmake libqt4-dev libqt4-opengl-dev libgrantlee-dev libboost-dev libx11-xcb-dev libxcb-keysyms1-dev`

You also need assimp (i.e. from git).

`$ git clone git://github.com/assimp/assimp.git`

### Window and Context Creation

You can choose between SDL and XCB window creation. XCB will only work with X11 :)
SDL is optional, found in the CMake options.

#### For SDL:

SDL 1.3 from mercurial

`$ hg clone http://hg.libsdl.org/SDL`

### Headers

The CMake script will download following headers from the [OpenGL Registry](http://www.opengl.org/registry/), if you dont have them in `/usr/include/GL3`,
and put them into the local liblub include directory.

* gl3.h
* glext.h
* glxext.h

## Compile

`$ cmake .`

`$ make`

## Run

Run all all apps from bin and scenes from media/scenes
`$ scripts/runAllScenes.sh`

### Apps
 
Running the applications from the bin directory.
`$ scripts/runAll.sh`

Or just a specific one
`$ ./bin/sponge`

### Scenes

View XML nice.xml scene file from media/scenes/.
`$ ./bin/load nice`

### Interesting config options

You can set a few options in in the config.xml file.

The OpenGL context version can be set
<Int name="GLcontext" value="4,1" />

VSync can be turned off
<Int name="Vsync" value="0" />

### Fullscreen Mode with XCB in GNOME

`$ gnome-keybinding-properties`

Set `Window Management` > `Toggle fullscreen Mode` to a hotkey. I chose Super + F12.

SDL Fullscreen works at init (toggle does not).

## tl;dr Ubuntu

To quickly build liblub on Ubuntu (assuming 4 cores)

`sudo aptitude install git build-essential cmake libqt4-dev libfreeimage-dev libx11-xcb-dev libxcb-keysyms1-dev`

`git clone git://github.com/assimp/assimp.git`

`cd assimp`

`cmake .`

`make -j5`

`sudo make install`

`cd ..`

`git clone git://github.com/lubosz/liblub.git`

`cd liblub`

`cmake .`

`make -j5`
