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
#ifndef STARLIGHT_PHYSICS_ARENA_ALLOCATOR_HG
#define STARLIGHT_PHYSICS_ARENA_ALLOCATOR_HG

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <iso646.h>

#include "Utils/Macros.h"

typedef struct
{
    uint8_t* Base;
    size_t Offset;
    size_t Capacity;
} STARLIGHT_StaticArena;

STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_StaticArena  
    ArenaCreate(size_t fp_DesiredCapacity)
{
    STARLIGHT_StaticArena f_Arena;

    f_Arena.Base = malloc(fp_DesiredCapacity);
    f_Arena.Offset = 0;
    f_Arena.Capacity = fp_DesiredCapacity;
    
    return f_Arena;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    ArenaAlloc
    (
        STARLIGHT_StaticArena* fp_Arena, 
        size_t fp_Size, 
        size_t fp_Align
    )
{
    if(not fp_Arena) //null check owo
    {

        return NULL;
    }

    size_t f_AlignedOffset = (fp_Arena->Offset + fp_Align - 1) & ~(fp_Align - 1);

    if(f_AlignedOffset + fp_Size > fp_Arena->Capacity)
    {
        return NULL;
    }

    void* f_MemoryPtr = fp_Arena->Base + f_AlignedOffset;
    fp_Arena->Offset = f_AlignedOffset + fp_Size;

    return f_MemoryPtr;
}

STARLIGHT_FORCEINLINE void
    ArenaReset(STARLIGHT_StaticArena* fp_Arena)
{
    fp_Arena->Offset = 0;
}


STARLIGHT_FORCEINLINE void
    ArenaDestroy(STARLIGHT_StaticArena* fp_Arena)
{
    if(not fp_Arena)
    {

        return;
    }

    free(fp_Arena->Base);
    fp_Arena->Base = NULL;
    fp_Arena->Offset = 0;
}

#endif /*STARLIGHT_PHYSICS_ARENA_ALLOCATOR_HG*/

