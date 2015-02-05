PegasusFeather
==============

Game Framework "Pegasus Feather" built to support the game "Shades"

This project is intended to help me extend my skills by creating a fully functional 2D game and engine, with the hope of being able to flex my OpenGL skills with the help of the latest edition of the red book.

This project is also intended to be as cross-platform for anything that fully supports C++ and SDL2 as I can manage, sometimes using some #ifdef magic.

The intention of all coding is a Write Once-Compile Everywhere structure, only using seperate makefiles as a last-resort.

All OpenGL code is intended to target OpenGL 3.3 capable hardware with optional fallbacks to OpenGL1.

DirectX 11 code should be grafted in at some point in anticipation of X1 release.

Dependancies:

Win32: SDL2, SDL2_image, SDL2_image, SDL2_mixer, SDL2_ttf, SDL2_net, GLEW.

Linux: libsdl2-dev, libsdl2-image-dev, libsdl2-mixer-dev, libsdl2-ttf-dev, libsdl2-net-dev, libglew-dev