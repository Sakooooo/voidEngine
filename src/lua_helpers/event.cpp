#include "event.h"

static int voidEngine::poll_sdl_event(lua_State* L) {
  SDL_Event e;

  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
      lua_pushstring(L, "keydown");
    } else {
      lua_pushstring(L, "not implemented");
    }
  } else {
    lua_pushstring(L, "nothing");
  };

  return 1;
}
