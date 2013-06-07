Massacre Proto
==============

[![Build Status](https://travis-ci.org/Dark-Confidant/Massacre.png)](https://travis-ci.org/Dark-Confidant/Massacre)

Crossplatform despair simulator

Installation
------------

We now support installation from gamerlay in **Gentoo** linux.

```bash
layman -a gamerlay
emerge games-engines/massacre games-util/massacre-data
```

Building
--------

Linux:

```bash
mkdir build && cd build
cmake ..
make
```

Dependencies
------------

- Gfx: Boost 1.46+, GLEW 1.6+
- Samples: GLFW 2.7.2+
