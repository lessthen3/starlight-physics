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
#ifndef STARLIGHT_TESTS_GRAPHICS_HG
#define STARLIGHT_TESTS_GRAPHICS_HG

#include <stdio.h>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <stddef.h>
#include <iso646.h>
#include <Macros.h>

STARLIGHT_NODISCARD static bool
    CreateSDLWindow
    (
        SDL_Window** fp_SDLWindow,
        const char* fp_WindowTitle,
        const unsigned int fp_WindowWidth,
        const unsigned int fp_WindowHeight
    )
{
    if (*fp_SDLWindow)
    {
        printf("Tried passing a valid SDL_Window* handle for window creation, please cleanup original SDL window or dereference pointer before attempting to create a new SDL window");
        return false;
    }

    uint64_t f_WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    *fp_SDLWindow = SDL_CreateWindow
    (
        fp_WindowTitle,
        fp_WindowWidth,
        fp_WindowHeight,
        f_WindowFlags
    );

    if (not *fp_SDLWindow)
    {
        printf("Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    return true;
}

#endif /*STARLIGHT_TESTS_GRAPHICS_HG*/
