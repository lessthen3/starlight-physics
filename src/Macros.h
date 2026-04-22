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
#ifndef __STARLIGHT_MACROS_HG__
#define __STARLIGHT_MACROS_HG__

#if defined(_MSC_VER)
    #define STARLIGHT_FORCEINLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define STARLIGHT_FORCEINLINE inline __attribute__((always_inline))
#else
    #define STARLIGHT_FORCEINLINE inline // Fallback for anything else
#endif


/* No Discard (C23, GCC, Clang, MSVC) */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    /* Official C23 Standard */
#define STARLIGHT_NODISCARD [[nodiscard]]
#elif defined(__GNUC__) || defined(__clang__)
    /* GCC and Clang extension */
#define STARLIGHT_NODISCARD __attribute__((__warn_unused_result__))
#elif defined(_MSC_VER)
    /* MSVC / SAL annotation (requires <sal.h>) */
#include <sal.h>
#define STARLIGHT_NODISCARD _Check_return_
#else
#define STARLIGHT_NODISCARD
#endif

#endif /*__STARLIGHT_MACROS_HG__*/
