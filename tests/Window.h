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

#include <Utils/Macros.h>

#include <stdint.h>

#include <SDL3/SDL.h>
#include <stddef.h>
#include <iso646.h>
#include <stdbool.h>


void 
    STESTS_PollEvents(bool* fp_IsRunning)
{

    SDL_Event wv_Event;

    while (SDL_PollEvent(&wv_Event))
    {
        switch (wv_Event.type)
        {
        //////////////////// Keyboard Input Stuff ////////////////////

        case SDL_EVENT_KEY_DOWN:
            break;
        case SDL_EVENT_KEY_UP:
            break;

        //////////////////// Mouse Input Handling ////////////////////

        case SDL_EVENT_MOUSE_MOTION:
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            break;

      
        //////////////////// Audio Device Stuff ////////////////////

        case SDL_EVENT_AUDIO_DEVICE_ADDED:
            break;
        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
            break;
        case SDL_EVENT_AUDIO_DEVICE_REMOVED:
            break;

        //////////////////// Touch Inputs ////////////////////

        case SDL_EVENT_FINGER_MOTION:
            break;
        case SDL_EVENT_FINGER_DOWN:
            break;
        case SDL_EVENT_FINGER_UP:
            break;
        case SDL_EVENT_FINGER_CANCELED:
            break;

        case SDL_EVENT_PINCH_BEGIN:
            break;
        case SDL_EVENT_PINCH_END:
            break;
        case SDL_EVENT_PINCH_UPDATE:
            break;

        //////////////////// Camera Stuff ig idk ////////////////////

        case SDL_EVENT_CAMERA_DEVICE_ADDED:
            break;

        //////////////////// Window Stuff ////////////////////

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            *fp_IsRunning = false;
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            break;
        case SDL_EVENT_WINDOW_SHOWN:
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            break;
        case SDL_EVENT_WINDOW_OCCLUDED:
            break;
        case SDL_EVENT_WINDOW_EXPOSED: // window became visible again
            break;
        ///NOT SURE IF THESE ARE NEEDED SINCE THERE IS A WINDOW FLAG FOR MOUSE FOCUS ALREADY
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            break;
        }
    }
}

#endif /*STARLIGHT_TESTS_GRAPHICS_HG*/
