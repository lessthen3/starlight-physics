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
#ifndef STARLIGHT_PHYSICS_MEMORY_ALLOCATOR_HG
#define STARLIGHT_PHYSICS_MEMORY_ALLOCATOR_HG

#include <stddef.h>
#include <iso646.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifdef STARLIGHT_DEBUG
# define STARLIGHT_PROFILE_MEMORY
#endif

typedef struct{
    size_t CurrentBytes;    // live bytes right now
    size_t PeakBytes;       // highest CurrentBytes ever reached
    size_t TotalAllocs;     // total number of alloc calls
    size_t TotalFrees;      // total number of free calls
    size_t FailedAllocs;    // OOM count
    const char* Tag;        // which allocator/subsystem this tracks
} AllocStats;

#ifdef STARLIGHT_PROFILE_MEMORY

    static AllocStats gs_GlobalStats = { 0, 0, 0, 0, 0, "Global" };

    void* 
        STARLIGHT_Malloc
        (
            size_t fp_Size, 
            AllocStats* fp_Stats
        )
    {
        // store size just before the user pointer so we can recover it on free
        void* f_Raw = malloc(fp_Size + sizeof(size_t));

        if (not f_Raw)
        {
            fp_Stats->FailedAllocs++;
            return NULL;
        }

        // write the size into the hidden header
        *(size_t*)f_Raw = fp_Size;

        fp_Stats->CurrentBytes += fp_Size;
        fp_Stats->TotalAllocs++;

        if (fp_Stats->CurrentBytes > fp_Stats->PeakBytes)
        {
            fp_Stats->PeakBytes = fp_Stats->CurrentBytes;
        }

        return (uint8_t*)f_Raw + sizeof(size_t); // return pointer past the header
    }

    void*
        STARLIGHT_Realloc
        (
            size_t fp_Size, 
            AllocStats* fp_Stats
        )
    {

        return NULL;
    }

    void 
        STARLIGHT_Free
        (
            void* fp_Ptr, 
            AllocStats* fp_Stats
        )
    {
        if (not fp_Ptr) 
        {
             return;
        }

        // recover the hidden header to get the size back
        void*  f_Raw  = (uint8_t*)fp_Ptr - sizeof(size_t);
        size_t f_Size = *(size_t*)f_Raw;

        fp_Stats->CurrentBytes -= f_Size;
        fp_Stats->TotalFrees++;

        free(f_Raw);
    }

    void 
        STARLIGHT_PrintAllocStats(const AllocStats* fp_Stats)
    {
        printf("[%s] current: %zu bytes | peak: %zu bytes | allocs: %zu | frees: %zu | failed: %zu\n",
            fp_Stats->Tag,
            fp_Stats->CurrentBytes,
            fp_Stats->PeakBytes,
            fp_Stats->TotalAllocs,
            fp_Stats->TotalFrees,
            fp_Stats->FailedAllocs
        );
    }

    #define STARLIGHT_MALLOC(fp_Size)     STARLIGHT_Malloc(fp_Size, gs_GlobalStats)
    #define STARLIGHT_FREE(fp_Ptr)        STARLIGHT_Free(fp_Ptr, gs_GlobalStats)

#else

    #define STARLIGHT_MALLOC(fp_Size, fp_Stats)     malloc(fp_Size)
    #define STARLIGHT_FREE(fp_Ptr, fp_Stats)        free(fp_Ptr)

#endif /*STARLIGHT_PROFILE_MEMORY*/



#endif /*STARLIGHT_PHYSICS_MEMORY_ALLOCATOR_HG*/
