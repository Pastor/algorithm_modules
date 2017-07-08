
#include <gtest/gtest.h>
#include <windows.h>
#include <psapi.h>

int main(int argc, char **argv) {
    PROCESS_MEMORY_COUNTERS pmc_start;
    PROCESS_MEMORY_COUNTERS pmc_complete;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc_start, sizeof(pmc_start));
    ::testing::InitGoogleTest(&argc, argv);
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemState _checkpoint_start;
    _CrtMemState _checkpoint_end;
    _CrtMemState _checkpoint_diff;

    fprintf(stderr, "Memory leak detection enabled\n");
     _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
     _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
    _CrtMemCheckpoint(&_checkpoint_start);

#endif
    int ret = RUN_ALL_TESTS();
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemCheckpoint(&_checkpoint_end);
    if (_CrtMemDifference(&_checkpoint_diff, &_checkpoint_start, &_checkpoint_end))
        _CrtMemDumpStatistics( &_checkpoint_diff );
#endif
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc_complete, sizeof(pmc_complete));
    fprintf(stdout, "[Memory] Start   : %dMb\n",
            (int) (pmc_start.WorkingSetSize / 1024 / 1024));
    fprintf(stdout, "[Memory] Complete: %dMb\n",
            (int) (pmc_complete.WorkingSetSize / 1024 / 1024));
    fprintf(stdout, "[Memory] Diff    : %dMb\n",
            (int) ((pmc_complete.WorkingSetSize / 1024 / 1024) - (pmc_start.WorkingSetSize / 1024 / 1024)));
    return ret;
}
