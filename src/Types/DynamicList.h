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
#ifndef STARLIGHT_PHYSICS_DYNAMIC_LIST_HG
#define STARLIGHT_PHYSICS_DYNAMIC_LIST_HG

///Starlight Physics uwu
#include "Utils/Macros.h" //idk should move this to a better dir ngl owo
#include "Utils/MemoryAllocator.h"

typedef struct{
    void* Data;
    size_t CurrentSize;
    size_t MaxCapacity;
    size_t ElementSize; //sizeof(elem)
} STARLIGHT_DynamicList;


STARLIGHT_FORCEINLINE STARLIGHT_DynamicList
    STARLIGHT_DynamicListCreate
    (
        size_t fp_InitialCapacity,
        size_t fp_ElementSize //no templates owo
    )
{
    STARLIGHT_DynamicList f_List;

    f_List.MaxCapacity = fp_InitialCapacity;
    f_List.ElementSize = fp_ElementSize;
    f_List.Data = malloc(fp_ElementSize * fp_InitialCapacity);
    f_List.CurrentSize = 0; //ye no constructors ;w;
    
    return f_List;
}

STARLIGHT_NODISCARD_FORCEINLINE void*
    STARLIGHT_DynamicListPushBack(STARLIGHT_DynamicList* fp_List)
{
    if(not fp_List)
    {

        return NULL; //fuck i hate not having null guarantees in C thats the one thing I hate fuck
    }

    if(fp_List->CurrentSize == fp_List->MaxCapacity)
    {
        size_t f_NewCapacity = fp_List->MaxCapacity * 2; //scale geometrically using 2^N for now apparently theres a better way but w/e
        void* f_NewDataChunk = realloc(fp_List->Data, f_NewCapacity * fp_List->ElementSize);

        if(not f_NewDataChunk)
        {
            //log?
            return NULL;
        }

        fp_List->Data = f_NewDataChunk;
        fp_List->MaxCapacity = f_NewCapacity;
    }

    void* f_Slot = (uint8_t*)fp_List->Data + fp_List->CurrentSize*fp_List->ElementSize;
    fp_List->CurrentSize++;

    return f_Slot;
}

STARLIGHT_NODISCARD_FORCEINLINE void* //WARNING: not sure if i should assert or return NULL idk
    STARLIGHT_DynamicListGet
    (
        const STARLIGHT_DynamicList* fp_List,
        const size_t fp_Index
    )
{
    STARLIGHT_ASSERT(fp_List and fp_Index < fp_List->CurrentSize);
    return (uint8_t*)fp_List->Data + fp_Index*fp_List->ElementSize;
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_DynamicListPop(STARLIGHT_DynamicList* fp_List)
{
    if(fp_List and fp_List->CurrentSize > 0) //NULL check first then do stuff owo
    {
        fp_List->CurrentSize--;
    }
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_DynamicListDestroy(STARLIGHT_DynamicList* fp_List)
{
    if(not fp_List)
    {
        return;
    }

    free(fp_List->Data);
    fp_List->Data = NULL;
    fp_List->CurrentSize = 0;
    fp_List->MaxCapacity = 0;
    fp_List->ElementSize = 0; //idk reset ig owo
}

#endif /*STARLIGHT_PHYSICS_DYNAMIC_LIST_HG*/
