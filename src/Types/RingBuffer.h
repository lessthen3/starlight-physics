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
#include <stdlib.h>
#include <stdint.h>

///Starlight Physics
#include "Utils/Macros.h"

#define STARLIGHT_RING_BUFFER_CREATE(fp_Capacity, fp_ElementSize)                                           \
    STARLIGHT_SAFE_CALL(                                                                                    \
        STARLIGHT_IMPLEMENTATION_RingBufferCreate(fp_Capacity, fp_ElementSize),                             \
        STARLIGHT_ASSERT(fp_ElementSize > 0 && "Tried to create a ring buffer with a 0 element size ;w;"),  \
        STARLIGHT_ASSERT(fp_Capacity > 0     && "capacity must be > 0"),                                    \
        STARLIGHT_ASSERT((fp_Capacity & (fp_Capacity - 1)) == 0 && "capacity must be power of 2")           \
    )

#define STARLIGHT_RING_BUFFER_WRITE(fp_RingBuffer, T)                                        \
    STARLIGHT_SAFE_CALL(                                                                     \
        ((T*)STARLIGHT_IMPLEMENTATION_RingBufferWrite(fp_RingBuffer)),                       \
        STARLIGHT_ASSERT(fp_RingBuffer),                                                     \
        STARLIGHT_ASSERT(fp_RingBuffer->Data)                                                \
    )

#define STARLIGHT_RING_BUFFER_PEEK(fp_RingBuffer, T, fp_Offset)                               \
    STARLIGHT_SAFE_CALL(                                                                      \
        ((T*)STARLIGHT_IMPLEMENTATION_RingBufferPeek(fp_RingBuffer, fp_Offset)),              \
        STARLIGHT_ASSERT(fp_RingBuffer && "Tried to pass NULL ring buffer"),                  \
        STARLIGHT_ASSERT(fp_Offset < fp_RingBuffer->Capacity && "Index out of bounds!")       \
    )

    //STARLIGHT_ASSERT(fp_Buffer->Head > 0 && "Can't peek an empty")  WARNING: unsure how to manage invalid elements whatever
#define STARLIGHT_RING_BUFFER_PEEK_LATEST(fp_RingBuffer, T)                         \
    STARLIGHT_SAFE_CALL(                                                            \
        ((T*)STARLIGHT_IMPLEMENTATION_RingBufferPeekLatest(fp_RingBuffer)),         \
        STARLIGHT_ASSERT(fp_RingBuffer && "Tried to pass NULL ring buffer")         \
    )

#define STARLIGHT_RING_BUFFER_GET_VALID_ELEMENT_COUNT(fp_RingBuffer)                \
    STARLIGHT_SAFE_CALL(                                                            \
        STARLIGHT_IMPLEMENTATION_RingBufferGetValidElementCount(fp_RingBuffer),     \
        STARLIGHT_ASSERT(fp_RingBuffer && "Tried to pass NULL ring buffer")         \
    )

#define STARLIGHT_RING_BUFFER_DESTROY(fp_RingBuffer)                                \
    STARLIGHT_SAFE_CALL(                                                            \
        STARLIGHT_IMPLEMENTATION_RingBufferDestroy(fp_RingBuffer),                  \
        STARLIGHT_ASSERT(fp_RingBuffer && "Tried to pass NULL ring buffer")         \
    )

typedef struct{
    void* Data;
    size_t Capacity;
    size_t ElementSize;
    size_t Head;
} STARLIGHT_RingBuffer;

STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_RingBuffer
    STARLIGHT_IMPLEMENTATION_RingBufferCreate
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

    return f_RingBuffer;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_IMPLEMENTATION_RingBufferWrite
    (
        STARLIGHT_RingBuffer* fp_Buffer
    )
{
    void* f_Slot = (uint8_t*)fp_Buffer->Data + (fp_Buffer->Head & (fp_Buffer->Capacity - 1))*fp_Buffer->ElementSize; // bitmask wrapping
    fp_Buffer->Head++;
    return f_Slot; // caller writes into this directly
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_IMPLEMENTATION_RingBufferPeek
    (
        const STARLIGHT_RingBuffer* fp_Buffer,
        const size_t                fp_OffsetFromOldest
    )
{
    // oldest element lives at Head - Capacity when buffer is full, offset 0 = oldest, offset Capacity-1 = newest
    size_t f_Index = (fp_Buffer->Head - fp_Buffer->Capacity + fp_OffsetFromOldest) & (fp_Buffer->Capacity - 1);
    return (uint8_t*)fp_Buffer->Data + f_Index*fp_Buffer->ElementSize;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_IMPLEMENTATION_RingBufferPeekLatest
    (
        const STARLIGHT_RingBuffer* fp_Buffer
    )
{
    size_t f_Index = (fp_Buffer->Head - 1) & (fp_Buffer->Capacity - 1);
    return (uint8_t*)fp_Buffer->Data + f_Index * fp_Buffer->ElementSize;
}

STARLIGHT_NODISCARD_FORCEINLINE size_t //kinda just intended if ur unsure or as a safety check for reading data on first write cycle owo 
    STARLIGHT_IMPLEMENTATION_RingBufferGetValidElementCount
    (
        const STARLIGHT_RingBuffer* fp_Buffer
    )
{
    return fp_Buffer->Head < fp_Buffer->Capacity
        ? fp_Buffer->Head
        : fp_Buffer->Capacity;
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_IMPLEMENTATION_RingBufferDestroy
    (
        STARLIGHT_RingBuffer* fp_Buffer
    )
{
    free(fp_Buffer->Data);
    fp_Buffer->Data      = NULL;
    fp_Buffer->Head      = 0;
    fp_Buffer->Capacity  = 0;
}

#endif /*STARLIGHT_PHYSICS_RING_BUFFER_HG*/
