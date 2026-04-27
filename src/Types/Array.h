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
#ifndef STARLIGHT_PHYSICS_ARRAY_HG
#define STARLIGHT_PHYSICS_ARRAY_HG

#include <stdint.h>
#include <stddef.h>

#include "Utils/Macros.h"

#define STARLIGHT_ARRAY_CREATE(fp_Size, fp_ElementSize)                                                                                \
(                                                                                                                                      \
    STARLIGHT_STATIC_ASSERT_UNSIGNED(fp_Size, "[Array]: STARLIGHT_ARRAY_CREATE only accepts unsigned values for size"),                \
    STARLIGHT_STATIC_ASSERT_UNSIGNED(fp_ElementSize, "[Array]: STARLIGHT_ARRAY_CREATE only accepts unsigned values for element size"), \
    STARLIGHT_ASSERT(fp_Size > 0 && "Tried to pass size of 0 to array creation"),                                                      \
    STARLIGHT_ASSERT(fp_ElementSize > 0 && "Element size must be greater than 0"),                                                     \
    STARLIGHT_IMPLEMENTATION_ArrayCreate(fp_Size, fp_ElementSize)                                                                      \
)

#define STARLIGHT_ARRAY_ACCESS_INDEX(fp_Array, T, fp_Index)                                                                            \
(                                                                                                                                      \
    STARLIGHT_STATIC_ASSERT_UNSIGNED(fp_Index, "[Array]: only unsigned values are valid for list indices"),                            \
    STARLIGHT_ASSERT(fp_Index < fp_Array.Size && "Index out of bounds!"),                                                              \
    (T*)STARLIGHT_IMPLEMENTATION_ArrayAccessIndex(fp_Array, fp_Index)                                                                  \
)

typedef struct{
    void* Data;
    size_t ElementSize;
    size_t Size;
} STARLIGHT_Array;

STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_Array
    STARLIGHT_IMPLEMENTATION_ArrayCreate 
    (
        const size_t fp_Size,
        const size_t fp_ElementSize
    )
{
    STARLIGHT_Array f_Array;
    
    f_Array.Size = fp_Size;
    f_Array.ElementSize = fp_ElementSize;

    f_Array.Data = malloc(fp_Size*fp_ElementSize);
    STARLIGHT_ASSERT(f_Array.Data && "[Array]: Failed to allocate memory, something really bad must've happened ;w;");

    return f_Array;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_IMPLEMENTATION_ArrayAccessIndex
    (
        const STARLIGHT_Array fp_Array,
        size_t fp_Index
    )
{
    return (uint8_t*)fp_Array.Data + fp_Array.ElementSize*fp_Index;
}

#endif /*STARLIGHT_PHYSICS_ARRAY_HG*/
