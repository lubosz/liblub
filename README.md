liblub
======

An OpenGL 3.3 Core Framework.

Dependencies
------------

You need the following packages on Ubuntu. (other distros simmilar)


`$ sudo aptitude install build-essential cmake libqt4-dev libfreeimage-dev libboost-dev`

For XCB:

`$ sudo aptitude install libx11-xcb-dev libxcb-keysyms1-dev`

For SDL:

SDL 1.3 from mercurial

`$ hg clone http://hg.libsdl.org/SDL`


You also need assimp (i.e. form git).

`$ git clone git://github.com/assimp/assimp.git`

Headers
-------

The CMake script will download following headers from the [OpenGL Registry](http://www.opengl.org/registry/), if you dont have them in `/usr/include/GL3`

* gl3.h
* glext.h
* glxext.h

Compile
-------

`$ cmake .`

`$ make`


Running the examples
--------------------

Run the examples from the root directory.

`$ ./build/sponge`
