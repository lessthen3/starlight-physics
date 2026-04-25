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
#ifndef STARLIGHT_PHYSICS_STRING_IMPL_HG
#define STARLIGHT_PHYSICS_STRING_IMPL_HG

#include <stdint.h>
#include <stddef.h>
#include <iso646.h>

#include "Utils/Macros.h"

typedef struct{
    char* Data;
    size_t CurrentSize;
    size_t MaxCapacity;
} STARLIGHT_String;


STARLIGHT_FORCEINLINE STARLIGHT_String
    STARLIGHT_StringCreate
    (
        size_t fp_InitialCapacity
    )
{
    STARLIGHT_String f_String;

    f_String.Data = malloc(sizeof(char) * fp_InitialCapacity);
    f_String.MaxCapacity = fp_InitialCapacity;
    f_String.CurrentSize = 0; //ye no constructors ;w;
    
    return f_String;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_StringPushBack(STARLIGHT_String* fp_String)
{
    STARLIGHT_ASSERT(fp_String); //fuck i hate not having null guarantees in C thats the one thing I hate fuck

    if(fp_String->CurrentSize == fp_String->MaxCapacity)
    {
        size_t f_NewCapacity = fp_String->MaxCapacity * 2; //scale geometrically using 2^N for now apparently theres a better way but w/e
        void* f_NewDataChunk = realloc(fp_String->Data, f_NewCapacity * sizeof(char));

        if(not f_NewDataChunk)
        {
            //log?
            return NULL;
        }

        fp_String->Data = f_NewDataChunk;
        fp_String->MaxCapacity = f_NewCapacity;
    }

    void* f_Slot = (uint8_t*)fp_String->Data + fp_String->CurrentSize*sizeof(char);
    fp_String->CurrentSize++;

    return f_Slot;
}

STARLIGHT_NODISCARD_FORCEINLINE char* //WARNING: not sure if i should assert or return NULL idk
    STARLIGHT_StringGet
    (
        const STARLIGHT_String* fp_String,
        const size_t fp_Index
    )
{
    STARLIGHT_ASSERT(fp_String && "Tried to pass null reference to STARLIGHT_String");
    STARLIGHT_ASSERT(fp_Index < fp_String->CurrentSize && "Index out of bounds on string!");

    return (char*)fp_String->Data + fp_Index*sizeof(char);
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_StringPop(STARLIGHT_String* fp_String)
{
    if(fp_String and fp_String->CurrentSize > 0) //NULL check first then do stuff owo
    {
        fp_String->CurrentSize--;
    }
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_StringDestroy(STARLIGHT_String* fp_String)
{
    if(not fp_String) //idk silent failure seems fine here since it'd already destroyed idk owo
    {
        return;
    }

    free(fp_String->Data);
    fp_String->Data = NULL;
    fp_String->CurrentSize = 0;
    fp_String->MaxCapacity = 0;
}

#endif /*STARLIGHT_PHYSICS_STRING_IMPL_HG*/
