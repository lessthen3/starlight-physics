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
#include "Utils/Macros.h"
#include "Window.h"

#include "Types/RingBuffer.h"

#include <signal.h>

static void
    OnSegfault(int fp_Signature)
{
    printf("[SEGMENTATION FAULT]: uh oh stinky uwu %d\n", fp_Signature);
    exit(fp_Signature);
}

int main(void)
{
    signal(SIGSEGV, OnSegfault);

    STARLIGHT_PRINT("Hello World >w<", STARLIGHT_COL_BRIGHT_MAGENTA);

    STARLIGHT_RingBuffer f_TestRingBuffer = STARLIGHT_RING_BUFFER_CREATE(100, 1);

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
