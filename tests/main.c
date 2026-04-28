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
#include "Utils/Macros.h"
#include "Window.h"

#include "Types/RingBuffer.h"
#include "Types/StaticArena.h"
#include "Types/DynamicList.h"
#include "Types/Array.h"

#include <signal.h>

//================================================================================ Segfault Handler ================================================================================//

static void
    OnSegfault(int fp_Signal)
{
    fprintf
    (
        stderr,
        STARLIGHT_COL_BRIGHT_RED
        "\n[SEGMENTATION FAULT]: uh oh stinky uwu, signal: %d\n"
        STARLIGHT_COL_RESET,
        fp_Signal
    );

    exit(fp_Signal);
}

static void
    OnTerminalInterrupt(int fp_Signal)
{
    fprintf
    (
        stderr,
        STARLIGHT_COL_MAGENTA
        "\n[PROCESS TERMINATED]: OwO, signal: %d\n"
        STARLIGHT_COL_RESET,
        fp_Signal
    );

    exit(fp_Signal);
}

//================================================================================ Test Helpers ================================================================================//

static int gs_TestsPassed = 0;
static int gs_TestsFailed = 0;

#define STARLIGHT_TEST(fp_Name, fp_Condition)                                                   \
do                                                                                               \
{                                                                                                \
    if (fp_Condition)                                                                            \
    {                                                                                            \
        STARLIGHT_PRINT_FMT(STARLIGHT_COL_BRIGHT_GREEN, "  [PASS] %s", fp_Name);               \
        gs_TestsPassed++;                                                                        \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        STARLIGHT_PRINT_FMT(STARLIGHT_COL_BRIGHT_RED,                                           \
            "  [FAIL] %s  (line %d)", fp_Name, __LINE__);                                       \
        gs_TestsFailed++;                                                                        \
    }                                                                                            \
} while(0)


//================================================================================ RingBuffer Tests ================================================================================//

static void
    TestRingBuffer(void)
{
    STARLIGHT_PRINT("\n=== RingBuffer Tests ===", STARLIGHT_COL_BRIGHT_CYAN);

    ////////////////////// Create //////////////////////

    STARLIGHT_RingBuffer f_Buffer = STARLIGHT_RING_BUFFER_CREATE(8u, sizeof(int));

    STARLIGHT_TEST("create: Data is non-null",         f_Buffer.Data != NULL);
    STARLIGHT_TEST("create: Capacity is 8",            f_Buffer.Capacity == 8);
    STARLIGHT_TEST("create: ElementSize is sizeof int", f_Buffer.ElementSize == sizeof(int));
    STARLIGHT_TEST("create: Head starts at 0",         f_Buffer.Head == 0);

    ////////////////////// Write + GetValidCount //////////////////////

    // write 3 elements — buffer not full yet
    for (int lv_I = 0; lv_I < 3; lv_I++)
    {
        int* fv_Slot = STARLIGHT_RING_BUFFER_WRITE(&f_Buffer, int);
        *fv_Slot = lv_I * 10;   // 0, 10, 20
    }

    STARLIGHT_TEST("write 3: Head == 3",               f_Buffer.Head == 3);
    STARLIGHT_TEST("write 3: ValidCount == 3",         STARLIGHT_RING_BUFFER_GET_VALID_ELEMENT_COUNT(f_Buffer) == 3);

    ////////////////////// PeekLatest //////////////////////

    int* f_Latest = STARLIGHT_RING_BUFFER_PEEK_LATEST(f_Buffer, int);
    STARLIGHT_TEST("peek latest: value is 20",         *f_Latest == 20);

    ////////////////////// Fill to capacity + wraparound //////////////////////

    // write 8 more — wraps around, stomps old entries
    for (int lv_I = 0; lv_I < 8; lv_I++)
    {
        int* fv_Slot = STARLIGHT_RING_BUFFER_WRITE(&f_Buffer, int);
        *fv_Slot = 100 + lv_I;   // 100..107
    }

    STARLIGHT_TEST("wrap: ValidCount == 8 (full)",     STARLIGHT_RING_BUFFER_GET_VALID_ELEMENT_COUNT(f_Buffer) == 8);
    STARLIGHT_TEST("wrap: Head == 11",                 f_Buffer.Head == 11);

    // newest is the last written: 107
    f_Latest = STARLIGHT_RING_BUFFER_PEEK_LATEST(f_Buffer, int);
    STARLIGHT_TEST("wrap: latest == 107",              *f_Latest == 107);

    ////////////////////// Peek oldest-to-newest order //////////////////////

    // after writing 11 total into a cap-8 buffer:
    // oldest slot = Head - 8 = 3, so values 100,101,102,103,104,105,106,107
    bool f_OrderCorrect = true;
    for (size_t lv_I = 0; lv_I < 8; lv_I++)
    {
        int* fv_Entry = STARLIGHT_RING_BUFFER_PEEK(f_Buffer, int, lv_I);
        if (*fv_Entry != (int)(100 + lv_I))
        {
            f_OrderCorrect = false;
            break;
        }
    }
    STARLIGHT_TEST("peek order: oldest-to-newest correct", f_OrderCorrect);

    ////////////////////// Destroy //////////////////////

    STARLIGHT_RING_BUFFER_DESTROY(&f_Buffer);
    STARLIGHT_TEST("destroy: Data is NULL",            f_Buffer.Data == NULL);
    STARLIGHT_TEST("destroy: Capacity is 0",           f_Buffer.Capacity == 0);
    STARLIGHT_TEST("destroy: Head is 0",               f_Buffer.Head == 0);
}

//================================================================================ DynamicList Tests ================================================================================//

static void
    TestDynamicList(void)
{
    STARLIGHT_PRINT("\n=== DynamicList Tests ===", STARLIGHT_COL_BRIGHT_CYAN);

    ////////////////////// Create //////////////////////

    STARLIGHT_DynamicList f_List = STARLIGHT_DYNAMIC_LIST_CREATE(4u, sizeof(float));

    STARLIGHT_TEST("create: Data is non-null",         f_List.Data != NULL);
    STARLIGHT_TEST("create: MaxCapacity == 4",         f_List.MaxCapacity == 4);
    STARLIGHT_TEST("create: CurrentSize == 0",         f_List.CurrentSize == 0);

    ////////////////////// PushBack //////////////////////

    float* f_Slot;

    f_Slot  = STARLIGHT_DYNAMIC_LIST_PUSH_BACK(&f_List, float); *f_Slot = 1.0f;
    f_Slot  = STARLIGHT_DYNAMIC_LIST_PUSH_BACK(&f_List, float); *f_Slot = 2.0f;
    f_Slot  = STARLIGHT_DYNAMIC_LIST_PUSH_BACK(&f_List, float); *f_Slot = 3.0f;
    f_Slot  = STARLIGHT_DYNAMIC_LIST_PUSH_BACK(&f_List, float); *f_Slot = 4.0f;

    STARLIGHT_TEST("push 4: CurrentSize == 4",         f_List.CurrentSize == 4);
    STARLIGHT_TEST("push 4: MaxCapacity == 4",         f_List.MaxCapacity == 4);

    ////////////////////// Growth //////////////////////

    // this push should trigger realloc to 8
    f_Slot  = STARLIGHT_DYNAMIC_LIST_PUSH_BACK(&f_List, float); *f_Slot = 5.0f;

    STARLIGHT_TEST("growth: CurrentSize == 5",         f_List.CurrentSize == 5);
    STARLIGHT_TEST("growth: MaxCapacity == 8",         f_List.MaxCapacity == 8);

    ////////////////////// Get //////////////////////

    float* f_First  = STARLIGHT_DYNAMIC_LIST_GET_INDEX(f_List, float, 0u);
    float* f_Third  = STARLIGHT_DYNAMIC_LIST_GET_INDEX(f_List, float, 2u);
    float* f_Fifth  = STARLIGHT_DYNAMIC_LIST_GET_INDEX(f_List, float, 4u);

    STARLIGHT_TEST("get [0] == 1.0",                   *f_First == 1.0f);
    STARLIGHT_TEST("get [2] == 3.0",                   *f_Third == 3.0f);
    STARLIGHT_TEST("get [4] == 5.0",                   *f_Fifth == 5.0f);

    ////////////////////// Pop //////////////////////

    STARLIGHT_DYNAMIC_LIST_POP_BACK(&f_List);
    STARLIGHT_TEST("pop: CurrentSize == 4",            f_List.CurrentSize == 4);

    ////////////////////// Destroy //////////////////////

    STARLIGHT_DYNAMIC_LIST_DESTROY(&f_List);
    STARLIGHT_TEST("destroy: Data is NULL",            f_List.Data == NULL);
    STARLIGHT_TEST("destroy: CurrentSize == 0",        f_List.CurrentSize == 0);
    STARLIGHT_TEST("destroy: MaxCapacity == 0",        f_List.MaxCapacity == 0);
}

//================================================================================ StaticArena Tests ================================================================================//

typedef struct { float X; float Y; float Z; } TestVec3;

static void
    TestStaticArena(void)
{
    STARLIGHT_PRINT("\n=== StaticArena Tests ===", STARLIGHT_COL_BRIGHT_CYAN);

    ////////////////////// Create //////////////////////

    STARLIGHT_StaticArena f_Arena = STARLIGHT_STATIC_ARENA_CREATE(1024);

    STARLIGHT_TEST("create: Base is non-null",          f_Arena.Base != NULL);
    STARLIGHT_TEST("create: Capacity == 1024",          f_Arena.Capacity == 1024);
    STARLIGHT_TEST("create: Offset == 0",               f_Arena.Offset == 0);

    ////////////////////// Alloc //////////////////////

    uint32_t* f_Int  = STARLIGHT_STATIC_ARENA_ALLOCATE(&f_Arena, uint32_t, sizeof(uint32_t), _Alignof(uint32_t));
    *f_Int = 42;

    STARLIGHT_TEST("alloc uint32: non-null",            f_Int != NULL);
    STARLIGHT_TEST("alloc uint32: value roundtrip",     *f_Int == 42);
    STARLIGHT_TEST("alloc uint32: Offset advanced",     f_Arena.Offset >= sizeof(uint32_t));

    TestVec3* f_Vec = STARLIGHT_STATIC_ARENA_ALLOCATE(&f_Arena, TestVec3, sizeof(TestVec3), _Alignof(TestVec3));
    f_Vec->X = 1.0f; f_Vec->Y = 2.0f; f_Vec->Z = 3.0f;

    STARLIGHT_TEST("alloc vec3: non-null",              f_Vec != NULL);
    STARLIGHT_TEST("alloc vec3: X == 1.0",              f_Vec->X == 1.0f);
    STARLIGHT_TEST("alloc vec3: Y == 2.0",              f_Vec->Y == 2.0f);
    STARLIGHT_TEST("alloc vec3: Z == 3.0",              f_Vec->Z == 3.0f);

    // alignment check — pointer should be aligned to _Alignof(TestVec3)
    STARLIGHT_TEST("alloc vec3: alignment correct", ((uintptr_t)f_Vec % _Alignof(TestVec3)) == 0);

    // alloc a bunch and make sure offset tracks
    size_t f_OffsetBefore = f_Arena.Offset;
    uint8_t* f_Bytes = STARLIGHT_STATIC_ARENA_ALLOCATE(&f_Arena, uint8_t, 64, 1);
    STARLIGHT_TEST("alloc 64 bytes: offset grew by 64", f_Arena.Offset == f_OffsetBefore + 64);
    STARLIGHT_TEST("alloc 64 bytes: non-null",          f_Bytes != NULL);

    ////////////////////// Reset //////////////////////

    STARLIGHT_IMPLEMENTATION_StaticArenaReset(&f_Arena);
    STARLIGHT_TEST("reset: Offset == 0",                f_Arena.Offset == 0);
    STARLIGHT_TEST("reset: Base still valid",           f_Arena.Base != NULL);

    // can alloc again after reset
    uint32_t* f_After = STARLIGHT_STATIC_ARENA_ALLOCATE(&f_Arena, uint32_t, sizeof(uint32_t), _Alignof(uint32_t));
    *f_After = 99;
    STARLIGHT_TEST("alloc after reset: value correct",  *f_After == 99);

    ////////////////////// Destroy //////////////////////

    STARLIGHT_IMPLEMENTATION_StaticArenaDestroy(&f_Arena);
    STARLIGHT_TEST("destroy: Base is NULL",             f_Arena.Base == NULL);
    STARLIGHT_TEST("destroy: Offset == 0",              f_Arena.Offset == 0);
    STARLIGHT_TEST("destroy: Capacity == 0",            f_Arena.Capacity == 0);
}

//================================================================================ Summary ================================================================================//

static void
    PrintSummary(void)
{
    STARLIGHT_PRINT("\n=== Test Summary ===", STARLIGHT_COL_BRIGHT_CYAN);

    if (gs_TestsFailed == 0)
    {
        STARLIGHT_PRINT_FMT(STARLIGHT_COL_BRIGHT_GREEN,
            "All %d tests passed! owo", gs_TestsPassed);
    }
    else
    {
        STARLIGHT_PRINT_FMT(STARLIGHT_COL_BRIGHT_RED,
            "%d passed, %d FAILED >:(",
            gs_TestsPassed, gs_TestsFailed);
    }
}

//================================================================================ Main ================================================================================//

int main(void)
{
    signal(SIGSEGV, OnSegfault);
    signal(SIGINT, OnTerminalInterrupt);


    STARLIGHT_PRINT("Starlight Physics — Data Structure Tests >w<", STARLIGHT_COL_BRIGHT_MAGENTA);

    TestRingBuffer();
    TestDynamicList();
    TestStaticArena();
    PrintSummary();

    STARLIGHT_Array f_Array = STARLIGHT_ARRAY_CREATE(2u, 2u);

    // char f_StackOverflow[60000000u];

    // f_StackOverflow[0] = 'f';

    // uint64_t* f_HeapViolation = 0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000;

    // *f_HeapViolation = 0xDEADBEEF;

    // uint8_t* f_HeapOverflow = malloc(59595959595959);

    // f_HeapOverflow[0] = 0xFF;

    #ifdef PEACH_PLATFORM_LINUX
      while(1) free(NULL);
    #endif

    #ifdef PEACH_PLATFORM_WINDOWS
        STARLIGHT_PRINT("hi me win owo", STARLIGHT_COL_BRIGHT_CYAN);
    #endif

    // f_StackOverflow[20000] = 'c';

    return gs_TestsFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    // STARLIGHT_PRINT("Hello World >w<", STARLIGHT_COL_BRIGHT_MAGENTA);

    // STARLIGHT_RingBuffer f_TestRingBuffer = STARLIGHT_RING_BUFFER_CREATE(100, 1);

    // SDL_Window* f_MainWindow = SDL_CreateWindow //implicitly calls SDL_Init for the video subsystem owo
    // (
    //     "Hello World!",
    //     800,
    //     600,
    //     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    // );

    // if (not f_MainWindow)
    // {
    //     printf("Window could not be created! SDL_Error: %s", SDL_GetError());
    //     return EXIT_FAILURE;
    // }

    // bool f_IsRunning = true;

    // while(f_IsRunning)
    // {
    //     STESTS_PollEvents(&f_IsRunning);
    // }

    // return EXIT_SUCCESS;
}
