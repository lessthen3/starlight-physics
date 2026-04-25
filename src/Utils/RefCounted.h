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
#ifndef STARLIGHT_PHYSICS_REFERENCE_COUNTED_HG
#define STARLIGHT_PHYSICS_REFERENCE_COUNTED_HG

#include <stdint.h>
#include <iso646.h>
#include <stdlib.h>

typedef void (*DestroyFn)(void*);  // destructor callback

typedef struct{
    uint32_t  RefCount;
    DestroyFn Destroy;   // called when refcount hits 0
} RefHeader;

void 
    RefRetain(void* fp_Obj)
{
    RefHeader* f_Header = (RefHeader*)fp_Obj;  // safe because RefHeader is first member
    f_Header->RefCount++;
}

void 
    RefRelease(void* fp_Obj)
{
    if (not fp_Obj) 
    { 
        return; 
    }

    RefHeader* f_Header = (RefHeader*)fp_Obj;
    f_Header->RefCount--;

    if (f_Header->RefCount == 0)
    {
        f_Header->Destroy(fp_Obj);  // call the type-specific destructor
        free(fp_Obj);
    }
}

#endif /*STARLIGHT_PHYSICS_REFERENCE_COUNTED_HG*/
