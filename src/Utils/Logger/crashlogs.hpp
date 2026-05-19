//CrashLogs
//by Tyler Glaiel, 2023
//simple C++ crashlog output library based on C++23's <stacktrace> header
//call glaiel::crashlogs::begin_monitoring() at the start of your program
//and it will save a stack trace to a crash log if the program crashes!
//a decent amount of this was copied & modified from backward.cpp (https://github.com/bombela/backward-cpp)
//but with <stacktrace> it prints nice stack traces without needing to include pdbs and is also significantly simpler

#pragma once
#include <string>

namespace glaiel::crashlogs {
    //Register OS-level exception filters as early as possible from the init thread.
    //Does NOT spawn the writer thread or initialize telemetry. Idempotent.
    //If a crash occurs before begin_monitoring(), the handler will fall back to a
    //synchronous write on the crashing thread, and a paired last-chance crashlog
    //is always emitted from the exception path.
    void early_register();

    //Begins crash monitoring. Spawns the writer thread, initializes telemetry, and
    //ensures early_register() has run. Crash logs will not be generated until either
    //early_register() or begin_monitoring() has been called.
    void begin_monitoring();

    //set the folder path that crashlogs will be saved in.
    //if the folder doesn't exist, it will be created when the program crashes
    void set_crashlog_folder(std::string folderpath);

    void end_session();
}
