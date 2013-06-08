Massacre Proto
==============

[![Build Status](https://travis-ci.org/Dark-Confidant/Massacre.png)](https://travis-ci.org/Dark-Confidant/Massacre)

Cross-platform despair simulator.

Installation
------------

We now support installation from gamerlay on **Gentoo** linux:

```bash
layman -a gamerlay
emerge games-engines/massacre games-util/massacre-data
```

Building on Linux
-----------------

Install the [dependencies](#dependencies), then run:

```bash
mkdir build && cd build
cmake ..
make
```

Building on Windows
-------------------

Install [CMake](http://www.cmake.org/cmake/resources/software.html).
Get [Boost](http://boost.org/users/download/),
    [GLEW](http://glew.sourceforge.net/),
    [GLFW](http://glfw.org/download.html).
Run:

```cmd
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="<path to boost>;<path to glew>;<path to glfw>" ..
```

Then you can either run *Visual Studio Command Prompt (2010)*:

```cmd
msbuild /v:m Massacre.sln
msbuild INSTALL.vcxproj
```

or simply open Massacre.sln in *Visual Studio 2010* or later and build everything,  
then build INSTALL manually in order that the outputs are placed nicely.

Dependencies
------------

- Gfx: Boost 1.46+, GLEW 1.6+
- Samples: GLFW 2.7.2+
