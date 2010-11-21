liblub
======

An OpenGL 3.3 Core Framework.

Dependencies
------------

You need the following packages on Ubuntu. (other distros similar)


`$ sudo aptitude install build-essential cmake libqt4-dev libfreeimage-dev libboost-dev`

You also need assimp (i.e. from git).

`$ git clone git://github.com/assimp/assimp.git`

Window and Context Creation
---------------------------

You can choose between SDL and XCB window creation. XCB will only work with X11 :)
SDL is optional, found in the CMake options.

For XCB:

`$ sudo aptitude install libx11-xcb-dev libxcb-keysyms1-dev`

For SDL:

SDL 1.3 from mercurial

`$ hg clone http://hg.libsdl.org/SDL`

Headers
-------

The CMake script will download following headers from the [OpenGL Registry](http://www.opengl.org/registry/), if you dont have them in `/usr/include/GL3`,
and put them into the local liblub include directory.

* gl3.h
* glext.h
* glxext.h

Compile
-------

`$ cmake .`

`$ make`


Running the scenes
--------------------

Run the scenes from the root directory.

`$ ./bin/sponge`

tl;dr Ubuntu
------------

To quickly build liblub on Ubuntu (assuming 4 cores)

`sudo aptitude install git build-essential cmake libqt4-dev libfreeimage-dev libboost-dev libx11-xcb-dev libxcb-keysyms1-dev

git clone git://github.com/assimp/assimp.git

cd assimp

cmake .

make -j5

sudo make install

cd ..

git clone git://github.com/lubosz/liblub.git

cd liblub

cmake .

make -j5`