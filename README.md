---
layout: default
page_type: index
permalink: /
---

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

#### Prerequisites

* [CMake](http://www.cmake.org/cmake/resources/software.html);
* [Boost](http://boost.org/users/download/);
* [GLEW](http://glew.sourceforge.net/);
* [GLFW](http://sourceforge.net/projects/glfw/files/glfw/2.7.9/), if you want samples.


#### The Easy Way

1. Extract prerequisites to `./extras/`.
2. Configure and Generate via CMake GUI.
3. Build `Massacre.sln` with Visual Studio 2010 or later.
4. Build the `INSTALL` project inside the solution.

#### The Right Way

Run *Visual Studio Command Prompt (2010)*:

```cmd
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="<path to boost>;<path to glew>;<path to glfw>" ..
msbuild /v:m Massacre.sln
msbuild INSTALL.vcxproj
```

Dependencies
------------

- Gfx: Boost 1.46+, GLEW 1.6+
- Samples: GLFW 2.7.2, up to 2.7.9
