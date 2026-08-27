#ifndef ENGINE_H
#define ENGINE_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
// alright i seriously don't understand why clang won't compile this WITHOUT me putting this
class GuiManager;
#include "gui.h" // but i've included the header for it here?
// so WHY can't you fucking find it?!?!

class Engine
{
public:
    // I wonder if this could be an array later
    SDL_Window *window;
    SDL_Renderer *renderer;

    GuiManager *gui_manager;

    bool initalized;
    Engine();
    ~Engine();
};

#endif
