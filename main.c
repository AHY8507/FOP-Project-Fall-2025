#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "engine/entities/ball.h"
#include "engine/entities/team.h"
#include "engine/graphics/renderer.h"

int main(void) {
    srand((unsigned) time(NULL));

    struct Renderer renderer;
    if (renderer_init(&renderer) != 0)
        return 1;

    Scene scene = {
        .field = {1000, 700},
        .first_team = make_team_ptr(),
        .second_team = make_team_ptr(),
        .ball = make_ball_ptr(0, 0)
    };

    init_scene(&scene);

    bool running = true;
    SDL_Event event;
    Uint32 last = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            // For testing: Move the ball to the click position
            // else if (event.type == SDL_MOUSEBUTTONDOWN) {
            //     int mouse_x = event.button.x;
            //     int mouse_y = event.button.y;
            //     scene.ball->position.x = (float)mouse_x;
            //     scene.ball->position.y = (float)mouse_y;
            // }
        }

        const Uint32 now = SDL_GetTicks();
        const float dt = (now - last) / 1000.0f;
        last = now;

        update_scene(&scene, dt);
        renderer_draw_scene(&renderer, &scene);

        SDL_Delay(16);
    }

    renderer_destroy(&renderer);
    return 0;
}
