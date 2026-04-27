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

#define STARLIGHT_DYNAMIC_LIST_CREATE(fp_InitialCapacity, fp_ElementSize)                                                                       \
(                                                                                                                                               \
    STARLIGHT_ASSERT(fp_InitialCapacity > 0 && "[Dynamic List]: Initial capacity must be greater than 0!"),                                     \
    STARLIGHT_ASSERT(fp_ElementSize > 0 && "[Dynamic List]: Element size must be greater than 0!"),                                             \
    STARLIGHT_IMPLEMENTATION_DynamicListCreate(fp_InitialCapacity, fp_ElementSize)                                                              \
)

#define STARLIGHT_DYNAMIC_LIST_PUSH_BACK(fp_List, T)                                                                                            \
(                                                                                                                                               \
    STARLIGHT_ASSERT((fp_List) && "[Dynamic List]: Tried passing NULL reference to STARLIGHT_DYNAMIC_LIST_PUSH_BACK"),                            \
    (T*)STARLIGHT_IMPLEMENTATION_DynamicListPushBack(fp_List)                                                                                   \
)

#define STARLIGHT_DYNAMIC_LIST_GET_INDEX(fp_List, T, fp_Index)                                                                                  \
(                                                                                                                                               \
    STARLIGHT_ASSERT(fp_Index < fp_List.CurrentSize && "[Dynamic List]:Index out of bounds! invalid call to STARLIGHT_DYNAMIC_LIST_GET_INDEX"), \
    (T*)STARLIGHT_IMPLEMENTATION_DynamicListGet(fp_List, fp_Index)                                                                              \
)

#define STARLIGHT_DYNAMIC_LIST_POP_BACK(fp_List)                                                                                                \
(                                                                                                                                               \
    STARLIGHT_ASSERT((fp_List) && "[Dynamic List]: Tried passing NULL reference to STARLIGHT_DYNAMIC_LIST_GET_INDEX"),                            \
    STARLIGHT_ASSERT((fp_List)->CurrentSize > 0 && "[Dynamic List]: Size UNDERFLOW! invalid call to STARLIGHT_DYNAMIC_LIST_POP_BACK"),            \
    STARLIGHT_IMPLEMENTATION_DynamicListPopBack(fp_List)                                                                                        \
)

#define STARLIGHT_DYNAMIC_LIST_DESTROY(fp_List)                                                                                                 \
(                                                                                                                                               \
    STARLIGHT_ASSERT((fp_List) && "[Dynamic List]: Tried to pass NULL list in STARLIGHT_DYNAMIC_LIST_DESTROY"),                                   \
    STARLIGHT_ASSERT((fp_List)->Data && "[Dynamic List]: Tried to pass NULL data inside list(wai) ;w;, [STARLIGHT_DYNAMIC_LIST_DESTROY]"),        \
    STARLIGHT_IMPLEMENTATION_DynamicListDestroy(fp_List)                                                                                        \
)

typedef struct{
    void* Data;
    size_t CurrentSize;
    size_t MaxCapacity;
    size_t ElementSize; //sizeof(elem)
} STARLIGHT_DynamicList;


STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_DynamicList
    STARLIGHT_IMPLEMENTATION_DynamicListCreate
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
    STARLIGHT_IMPLEMENTATION_DynamicListPushBack(STARLIGHT_DynamicList* fp_List)
{
    if(fp_List->CurrentSize == fp_List->MaxCapacity)
    {
        size_t f_NewCapacity = fp_List->MaxCapacity * 2; //scale geometrically using 2^N for now apparently theres a better way but w/e
        void* f_NewDataChunk = realloc(fp_List->Data, f_NewCapacity * fp_List->ElementSize);

        STARLIGHT_ASSERT(f_NewDataChunk && "Unable to reallocate memory for dynamic list, something really bad must've happened OwO");

        fp_List->Data = f_NewDataChunk;
        fp_List->MaxCapacity = f_NewCapacity;
    }

    void* f_Slot = (uint8_t*)fp_List->Data + fp_List->CurrentSize*fp_List->ElementSize;
    fp_List->CurrentSize++;

    return f_Slot;
}

STARLIGHT_NODISCARD_FORCEINLINE void* //WARNING: not sure if i should assert or return NULL idk
    STARLIGHT_IMPLEMENTATION_DynamicListGet
    (
        const STARLIGHT_DynamicList fp_List,
        const size_t fp_Index
    )
{
    return (uint8_t*)fp_List.Data + fp_Index*fp_List.ElementSize;
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_IMPLEMENTATION_DynamicListPopBack(STARLIGHT_DynamicList* fp_List)
{
    fp_List->CurrentSize--;   
}

STARLIGHT_FORCEINLINE void
    STARLIGHT_IMPLEMENTATION_DynamicListDestroy(STARLIGHT_DynamicList* fp_List)
{
    free(fp_List->Data);
    fp_List->Data = NULL;
    fp_List->CurrentSize = 0;
    fp_List->MaxCapacity = 0;
    fp_List->ElementSize = 0; //idk reset ig owo
}

#endif /*STARLIGHT_PHYSICS_DYNAMIC_LIST_HG*/
