# liblub

An OpenGL 4 and 3.3 Core Framework.

## Dependencies

You need the following packages on Ubuntu.

`$ sudo apt-get install build-essential cmake libqt4-dev libqt4-opengl-dev libgrantlee-dev libboost-dev libx11-xcb-dev libxcb-keysyms1-dev`

You also need assimp (i.e. from git).

`$ git clone git://github.com/assimp/assimp.git`

### Window and Context Creation

You can choose between Qt and XCB window creation. XCB will only work with X11 :)

#### For Qt:

Qt window creation is optional on XCB Systems, but can be set in the CMake options.

`$ cmake . -DUSE_QT_WINDOWS=1`

### Headers

The CMake script will download glcorearb.h from the [OpenGL Registry](http://www.opengl.org/registry/), if you dont have it in `/usr/include/GL`,
and put them into the local liblub include directory.

## Compile

### Linux

`$ cmake .`

`$ make`

### Windows

You can download the required headers and libraries for MinGW here:
https://github.com/downloads/lubosz/liblub/liblub-i486-mingw32.tar.7z

To build:


`$ cmake . -DLIBLUB_WINDOWS=1 -DCMAKE_TOOLCHAIN_FILE=scripts/mingw-toolchain.cmake`

`$ make`

If you have run CMake before, do

`$ rm CMakeCache.txt`

## Run

Run all all apps from bin and scenes from media/scenes
`$ bin/liblub-examples -a`

### Apps

Or just a specific bin
`$ bin/liblub-load animation`

### Scenes

View XML nice.xml scene file from media/scenes/.
`$ bin/liblub-load nice.xml`

### Interesting config options

You can set a few options in in the ~/.liblub/config.xml file.

The OpenGL context version can be set
`<Int name="GLcontext" value="4,1" />`

VSync can be turned off
`<Int name="Vsync" value="0" />`

### Fullscreen Mode with XCB in GNOME

`$ gnome-keybinding-properties`

Set `Window Management` > `Toggle fullscreen Mode` to a hotkey. I chose Super + F12.

SDL Fullscreen works at init (toggle does not).

## tl;dr Ubuntu

To quickly build liblub on Ubuntu (assuming 4 cores)

`sudo apt-get install build-essential cmake libqt4-dev libqt4-opengl-dev libgrantlee-dev libboost-dev libx11-xcb-dev libxcb-keysyms1-dev`

In Quantal (12.10) you can use Assimp 3 from the repository

`sudo apt-get install assimp`

If you have 12.04 or lower you need to build Assimp 3

`git clone git://github.com/assimp/assimp.git`

`cd assimp`

`cmake .`

`make -j5`

`sudo make install`

`cd ..`

Build liblub

`git clone git://github.com/lubosz/liblub.git`

`cd liblub`

`cmake .`

`make -j5`

## tl;dr Arch Linux

You can install the [AUR Package](https://aur.archlinux.org/packages.php?ID=62678)

`packer -S liblub-git`
