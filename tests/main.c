/*******************************************************************
 *                  Starlight Physics v0.0.1
 *        Created by Ranyodh Singh Mandur - 👾 2026-2026
 *
 *             Licensed under the MIT License (MIT).
 *        For more details, see the LICENSE file or visit:
 *              https://opensource.org/licenses/MIT
 *
 *      Starlight Physics is a free open source physics engine
********************************************************************/
#include "Window.h"

#include <signal.h>

static void
    OnSegfault(int fp_Signature)
{
    printf("[SEGMENTATION FAULT]: uh oh stinky uwu %d", fp_Signature);
    exit(fp_Signature);
}

int main(int argc, char** argv)
{
    signal(SIGSEGV, OnSegfault);

    SDL_Window* f_MainWindow = SDL_CreateWindow
    (
        "Hello World!",
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (not f_MainWindow)
    {
        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    bool f_IsRunning = true;

    while(f_IsRunning)
    {
        STESTS_PollEvents(&f_IsRunning);
    }

    return EXIT_SUCCESS;
}
