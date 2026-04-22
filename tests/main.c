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
#include "Graphics.h"
#include "Input.h"

int main(int argc, char** argv)
{
    SDL_Window* f_MainWindow = NULL;
    CreateSDLWindow(&f_MainWindow, "Hello World!", 800, 600);

    bool f_IsRunning = true;

    while(f_IsRunning)
    {
        STESTS_PollEvents(&f_IsRunning);
    }

    return 0;
}
