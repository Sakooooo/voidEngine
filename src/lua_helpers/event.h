#ifndef VOID_EVENT_H
#define VOID_EVENT_H
#include <SDL3/SDL_events.h>
#include <lua.h>

namespace voidEngine {
static int poll_sdl_event(lua_State* L);
};

#endif
