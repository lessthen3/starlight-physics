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
#include <stdio.h>

//================================================================================ Compiler Dependent ================================================================================//

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


//================================================================================ Printing + Colours ================================================================================//

// ANSI colour codes
#define STARLIGHT_COL_RESET      "\033[0m"

#define STARLIGHT_COL_RED        "\x1B[31m"
#define STARLIGHT_COL_GREEN      "\x1B[32m"
#define STARLIGHT_COL_YELLOW     "\x1B[33m"
#define STARLIGHT_COL_BLUE       "\x1B[34m"
#define STARLIGHT_COL_MAGENTA    "\x1B[35m"
#define STARLIGHT_COL_CYAN       "\x1B[36m"
#define STARLIGHT_COL_WHITE      "\x1B[37m"
#define STARLIGHT_COL_BRIGHT_RED       "\x1B[91m"
#define STARLIGHT_COL_BRIGHT_GREEN     "\x1B[92m"
#define STARLIGHT_COL_BRIGHT_YELLOW    "\x1B[93m"
#define STARLIGHT_COL_BRIGHT_BLUE      "\x1B[94m"
#define STARLIGHT_COL_BRIGHT_MAGENTA   "\x1B[95m"
#define STARLIGHT_COL_BRIGHT_CYAN      "\x1B[96m"
#define STARLIGHT_COL_BRIGHT_WHITE     "\x1B[97m"

// wrap a string literal in a colour, zero runtime cost, pure compile-time concat
#define STARLIGHT_COLOURED(fp_Colour, fp_String) fp_Colour fp_String STARLIGHT_COL_RESET

#ifdef STARLIGHT_DEBUG
# define STARLIGHT_ENABLE_TERMINAL
#endif /*STARLIGHT_DEBUG*/

// print macros, gets thrown out for release builds owo
#ifdef STARLIGHT_ENABLE_TERMINAL
    #define STARLIGHT_PRINT(fp_String, fp_Colour) \
        fprintf(stdout, "%s\n", STARLIGHT_COLOURED(fp_Colour, fp_String))

    #define STARLIGHT_PRINT_ERROR(fp_String) \
        fprintf(stderr, STARLIGHT_COL_BRED "%s" STARLIGHT_COL_RESET "\n", fp_String)

    #define STARLIGHT_PRINT_FMT(fp_Colour, fp_FormattedString, ...) \
        fprintf(stdout, fp_Colour fp_FormattedString STARLIGHT_COL_RESET "\n", __VA_ARGS__)

    #define STARLIGHT_PRINT_ERROR_FMT(fp_Colour, fp_FormattedString, ...) \
        fprintf(stderr, fp_Colour fp_FormattedString STARLIGHT_COL_RESET "\n", __VA_ARGS__)
#else
    #define STARLIGHT_PRINT(fp_String, fp_Colour)          ((void)0)
    #define STARLIGHT_PRINT_ERROR(fp_String)              ((void)0)
    #define STARLIGHT_PRINT_FMT(fp_Colour, fp_FormattedString, ...) ((void)0)
#endif /*STARLIGHT_ENABLE_TERMINAL*/

//================================================================================ Assert Wrapper ================================================================================//

#define STARLIGHT_SAFE_CALL(fp_Result, ...) (__VA_ARGS__, fp_Result)

//================================================================================ Likely/Unlikely owo ================================================================================//

#if defined(__GNUC__) || defined(__clang__)
#   define STARLIGHT_LIKELY(fp_Condition)   __builtin_expect(!!(fp_Condition), 1)
#   define STARLIGHT_UNLIKELY(fp_Condition) __builtin_expect(!!(fp_Condition), 0)
#else
#   define STARLIGHT_LIKELY(fp_Condition)   (fp_Condition)
#   define STARLIGHT_UNLIKELY(fp_Condition) (fp_Condition)
#endif


//================================================================================ Assert ================================================================================//

#define STARLIGHT_ASSERT(fp_Condition)                                   \
    (STARLIGHT_LIKELY(fp_Condition)                                      \
        ? (void)0                                                        \
        : (fprintf(stderr,                                               \
            STARLIGHT_COL_BRIGHT_RED                                     \
            "\n[STARLIGHT ASSERT FAILED]\n"                              \
            "  condition : %s\n"                                         \
            "  file      : %s\n"                                         \
            "  line      : %d\n"                                         \
            "  func      : %s\n"                                         \
            STARLIGHT_COL_RESET "\n",                                    \
            #fp_Condition, __FILE__, __LINE__, __func__                  \
            ), abort(), (void)0)                                         \
    )

#ifdef STARLIGHT_DEBUG
# define STARLIGHT_DEBUG_ASSERT(fp_Condition) STARLIGHT_ASSERT(fp_Condition)
#else
# define STARLIGHT_DEBUG_ASSERT(fp_Condition) ((void)0)
#endif /*STARLIGHT_DEBUG_ASSERT*/

#endif /*__STARLIGHT_PHYSICS_MACROS_HG__*/
