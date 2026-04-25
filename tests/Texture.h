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
#ifndef STARLIGHT_TESTS_TEXTURE_HG
#define STARLIGHT_TESTS_TEXTURE_HG

#include <stdint.h>

// embed at the TOP of any ref-counted struct so the pointer cast is safe
typedef struct
{
    // RefHeader Ref;       // MUST be first member
    int       Width;
    int       Height;
    uint8_t*  Pixels;
} Texture;

#endif /*STARLIGHT_TESTS_TEXTURE_HG*/
