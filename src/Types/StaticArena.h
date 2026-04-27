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

#define STARLIGHT_STATIC_ARENA_CREATE(fp_DesiredCapacity)                                                                                       \
(                                                                                                                                               \
    STARLIGHT_ASSERT(fp_DesiredCapacity > 0 && "[Static Arena]: Tried creating a static arena of size 0, wtf?"),                                \
    STARLIGHT_IMPLEMENTATION_StaticArenaCreate(fp_DesiredCapacity)                                                                              \
)

// since we inline, CSE should optimize the double calculation and leave it in the register at the call site
#define STARLIGHT_STATIC_ARENA_ALLOCATE(fp_Arena, T, fp_Size, fp_Alignment)                                                                     \
(                                                                                                                                               \
    STARLIGHT_ASSERT((fp_Arena) && "[Static Arena]: Tried passing NULL static arena to STARLIGHT_STATIC_ARENA_ALLOCATE"),                       \
    STARLIGHT_ASSERT((((fp_Arena)->Offset + fp_Alignment - 1) & ~(fp_Alignment - 1)) + fp_Size <= (fp_Arena)->Capacity && "[Static Arena]: Buffer Overflow! wai :d"), \
    (T*)STARLIGHT_IMPLEMENTATION_StaticArenaAllocate(fp_Arena, fp_Size, (((fp_Arena)->Offset + fp_Alignment - 1) & ~(fp_Alignment - 1)))        \
)

#define STARLIGHT_STATIC_ARENA_RESET(fp_Arena)                                                                                                  \
(                                                                                                                                               \
    STARLIGHT_ASSERT(fp_Arena && "[Static Arena]: Tried passing NULL static arena to STARLIGHT_STATIC_ARENA_RESET"),                            \
    STARLIGHT_IMPLEMENTATION_StaticArenaReset(fp_Arena)                                                                                         \
)

#define STARLIGHT_STATIC_ARENA_DESTROY(fp_Arena)                                                                                                \
(                                                                                                                                               \
    STARLIGHT_ASSERT(fp_Arena && "[Static Arena]: Tried to pass NULL static arena in STARLIGHT_STATIC_ARENA_DESTROY"),                          \
    STARLIGHT_ASSERT(fp_Arena->Base && "[Static Arena]: Tried to pass NULL data inside static arena ;w;, [STARLIGHT_STATIC_ARENA_DESTROY]"),    \
    STARLIGHT_IMPLEMENTATION_StaticArenaDestroy(fp_Arena)                                                                                       \
)

typedef struct{
    uint8_t* Base;
    size_t Offset;
    size_t Capacity;
} STARLIGHT_StaticArena;

STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_StaticArena  
    STARLIGHT_IMPLEMENTATION_StaticArenaCreate(size_t fp_DesiredCapacity)
{
    STARLIGHT_StaticArena f_Arena;

    f_Arena.Base = malloc(fp_DesiredCapacity);

    STARLIGHT_ASSERT(f_Arena.Base && "[Static Arena]: Failed to allocate memory, something really bad must've happened ;w;");

    f_Arena.Offset = 0;
    f_Arena.Capacity = fp_DesiredCapacity;
    
    return f_Arena;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_IMPLEMENTATION_StaticArenaAllocate
    (
        STARLIGHT_StaticArena* fp_Arena, 
        size_t fp_Size, 
        size_t fp_AlignedOffset
    )
{
    void* f_MemoryPtr = fp_Arena->Base + fp_AlignedOffset;
    fp_Arena->Offset = fp_AlignedOffset + fp_Size;
    return f_MemoryPtr;
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_IMPLEMENTATION_StaticArenaReset(STARLIGHT_StaticArena* fp_Arena)
{
    fp_Arena->Offset = 0;
}


STARLIGHT_FORCEINLINE void
    STARLIGHT_IMPLEMENTATION_StaticArenaDestroy(STARLIGHT_StaticArena* fp_Arena)
{
    free(fp_Arena->Base);
    fp_Arena->Base = NULL;
    fp_Arena->Offset = 0;
    fp_Arena->Capacity = 0;
}

#endif /*STARLIGHT_PHYSICS_ARENA_ALLOCATOR_HG*/

