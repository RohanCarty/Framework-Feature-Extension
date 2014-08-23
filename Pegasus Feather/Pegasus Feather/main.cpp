#include <iostream>

#include <SDL2/SDL.h>

int main(int argc, char* arg[])
{
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout<<"ERROR SDL_Init"<<std::endl;
        
        return -1;
    }
    
    // create a window
    SDL_Window* window = SDL_CreateWindow("SDL 2 window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640,
                                          480,
                                          SDL_WINDOW_OPENGL
                                          );
    
    // wait 5 secs
    SDL_Delay(5000);
    
    // clean up
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}