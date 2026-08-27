#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "engine.h"
#include <cstdio>
#include <SDL3/SDL.h>
#include <vector>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

extern "C"
{
#include <lua.h>
}

int main()
{
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Failed to initalize SDL Video! %s\n", SDL_GetError());
        return 1;
    }

    Engine engine;

    if (!engine.initalized)
        return 1;

    MyTestGui my_test_gui = MyTestGui(engine.gui_manager);

    engine.gui_manager->AddPanel(&my_test_gui);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool running = true;

    const std::vector<SDL_Vertex> verts = {
        {
            SDL_FPoint{400, 150},
            SDL_FColor{255, 0, 0, 255},
            SDL_FPoint{0, 0},
        },
        {
            SDL_FPoint{200, 450},
            SDL_FColor{0, 0, 255, 255},
            SDL_FPoint{0, 0},
        },
        {
            SDL_FPoint{600, 450},
            SDL_FColor{0, 255, 0, 255},
            SDL_FPoint{0, 0},
        },
    };

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
        }

        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);

        // if (show_another_window)
        // {
        //     ImGui::Begin("Another window", &show_another_window);
        //     ImGui::Text("I am another window");
        //     if (ImGui::Button("Close me"))
        //         show_another_window = false;
        //     ImGui::End();
        // }

        engine.gui_manager->RenderPanels();
        SDL_SetRenderScale(engine.renderer, engine.gui_manager->io->DisplayFramebufferScale.x, engine.gui_manager->io->DisplayFramebufferScale.y);
        // SDL_SetRenderDrawColorFloat(engine.renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(engine.renderer);

        SDL_RenderGeometry(engine.renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), engine.renderer);

        SDL_RenderPresent(engine.renderer);
    }

    SDL_Quit();
    return 0;
}
