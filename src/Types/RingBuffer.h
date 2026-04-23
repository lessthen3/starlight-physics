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
#ifndef STARLIGHT_PHYSICS_RING_BUFFER_HG
#define STARLIGHT_PHYSICS_RING_BUFFER_HG

///STD
#include <stddef.h>
#include <malloc.h>

///Starlight Physics
#include "Utils/Macros.h"

typedef struct{
    void* Data;
    size_t Capacity;
    size_t ElementSize;
    size_t Head;
    size_t Tail;
} STARLIGHT_RingBuffer;

STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_RingBuffer
    STARLIGHT_RingBufferCreate
    (
        const size_t fp_Capacity,
        const size_t fp_ElementSize
    )
{
    STARLIGHT_RingBuffer f_RingBuffer;

    f_RingBuffer.Capacity = fp_Capacity;
    f_RingBuffer.ElementSize = fp_ElementSize;

    f_RingBuffer.Data = malloc(fp_Capacity*fp_ElementSize);

    f_RingBuffer.Head = 0;
    f_RingBuffer.Tail = 0;

    return f_RingBuffer;
}



#endif /*STARLIGHT_PHYSICS_RING_BUFFER_HG*/\
