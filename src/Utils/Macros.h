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
#ifndef __STARLIGHT_PHYSICS_MACROS_HG__
#define __STARLIGHT_PHYSICS_MACROS_HG__

#include <stdlib.h>

#if defined(_MSC_VER)
#   define STARLIGHT_FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#   define STARLIGHT_FORCEINLINE inline __attribute__((always_inline))
#else
#   define STARLIGHT_FORCEINLINE inline // Fallback for anything else
#endif /*STARLIGHT_FORCEINLINE*/


/* No Discard (C23, GCC, Clang, MSVC) */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#   define STARLIGHT_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
#   define STARLIGHT_NODISCARD __attribute__((__warn_unused_result__))
#elif defined(_MSC_VER)
#   include <sal.h>
#   define STARLIGHT_NODISCARD _Check_return_
#else
#   define STARLIGHT_NODISCARD
#endif /*STARLIGHT_NODISCARD*/


#define STARLIGHT_NODISCARD_FORCEINLINE STARLIGHT_NODISCARD STARLIGHT_FORCEINLINE

#ifdef STARLIGHT_DEBUG
#   define STARLIGHT_ASSERT(fp_Condition)                               \
        do{                                                             \
            if(!(fp_Condition))                                           \
            {                                                           \
                fprintf                                                 \
                (                                                       \
                    stderr,                                             \
                    "STARLIGHT_ASSERT FAILED: (%s)\n"                   \
                    "  file: %s\n"                                      \
                    "  line: %d\n"                                      \
                    "  func: %s\n",                                     \
                    #fp_Condition, __FILE__, __LINE__, __func__         \
                );                                                      \
                                                                        \
                abort();                                                \
            }                                                           \
        }while(!69)
#else
#   define STARLIGHT_ASSERT(fp_Condition) ((void)(fp_Condition))
#endif /*STARLIGHT_ASSERT*/

#endif /*__STARLIGHT_PHYSICS_MACROS_HG__*/
