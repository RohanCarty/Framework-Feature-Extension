PegasusFeather
==============

Game Framework "Pegasus Feather" built to support the game "Shades"

This project is intended to help me extend my skills by creating a fully functional 2D game and engine, with the hope of being able to flex my OpenGL/Vulkan/DX11 skills with the help of the latest edition of the red books.

This project is also intended to be as cross-platform for anything that fully supports C++ and SDL2 as I can manage, sometimes using some #ifdef magic.

The intention of all coding is a Write Once-Compile Everywhere structure, only using seperate makefiles as a last-resort.

Modern OpenGL code has been removed pending re-implementation and possible replacement with a Vulkan renderer as the new target, OpenGL 1 is still supported just in case for the long term.

DirectX 11 is in a very basic state, similar to the OpenGL1 renderer, but will either be re-written or abandoned pending Vulkan/OpenGL 4.5 updates depending on if I want to continue support for UWP.

Dependancies:

Win32: SDL2, SDL2_image, SDL2_image, SDL2_mixer, SDL2_ttf, SDL2_net, GLEW.

Debian/Ubuntu Linux: libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-net-dev libglew-dev libxml2-dev