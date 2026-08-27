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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplSDL3_InitForSDLRenderer(engine.window, engine.renderer);
    ImGui_ImplSDLRenderer3_Init(engine.renderer);

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

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // imgui closure
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello world!");

            ImGui::Text("I am some text");
            ImGui::Checkbox("Open demo window", &show_demo_window);
            ImGui::Checkbox("Open Another window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float *)&clear_color);

            if (ImGui::Button("Button"))
                counter++;

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        if (show_another_window)
        {
            ImGui::Begin("Another window", &show_another_window);
            ImGui::Text("I am another window");
            if (ImGui::Button("Close me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::Render();
        SDL_SetRenderScale(engine.renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(engine.renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        // SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(engine.renderer);

        SDL_RenderGeometry(engine.renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), engine.renderer);

        SDL_RenderPresent(engine.renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();
    return 0;
}
