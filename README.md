# Pyyrolysis

Author: Sebastien Alaiwan

Demo
----

An online demo is available here:

http://code.alaiwan.org/games/pyyrolysis

<p align="center"><img src="doc/screenshot.jpg" width="50%"></p>

Description
-----------

This is a demo of a platform-independent C++14 game project.
It uses SDL2, SDL2_image, and the OpenGL core profile.

It's meant as an example of a first-person exploration game
which can be compiled to native code, or, using Emscripten, to Javascript,
and maybe one day to WebAssembly.

The code doesn't contain any reference/dependency to Emscripten, except in the
entry-point file, where the main loop function gets passed to Emscripten.

This code also shows how to isolate your game logic code (doors, switches,
powerups, bullets, ..) from your I/O code (display, audio, input).

Directory structure
-------------------

```
bin:            output directory for binaries.
./check:        main check script. Call this to build native and asmjs versions.
engine/src:     I/O code (game-agnostic)
engine/include: representation of the outside world, as seen by the game logic.
src:            game logic (only depends on 'engine/include')
tests:          game logic tests
res:            pre-rendered game resources (sounds, sprites, tiles)
```


Build
-----

Requirements:
```
* libsdl2-dev
* libsdl2-image-dev
* libogg-dev
* libvorbis-dev
```

It can be compiled to native code using your native compiler (gcc or clang):

```
$ make
```

The binaries will be generated to a 'bin' directory
(This can be overriden using the BIN makefile variable).

It can also be compiled to Javascript, using Emscripten.
In theory, it's as simple as:

```
$ CXX=emcc make
```

However, in practice, more options need to be injected to the makefile,
like setting EXT (program extension) to ".html", etc.
There's a make wrapper that does exactly that:

```
$ ./scripts/asmjs-make
```

Run the game
------------

Just run the following command:

```
$ bin/rel/game.exe
```

