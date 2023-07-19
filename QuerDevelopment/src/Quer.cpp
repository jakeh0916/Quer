// Quer [Source]
// 
// |==========================================================================|
// | Date          Person          Description                                |
// |==========================================================================|
// | xx-xx-2023    Jake Huseman    Initial quer.bat                           |
// | 06-23-2023    Aidan Sommer    Added execution time                       |
// | 07-06-2023    Jake Huseman    Added 'quit' and better display            |
// | 07-19-2023    Group effort    Moved to github and C++ project            |
// |==========================================================================|

#include <iostream>
#include <string>

#define QUER_VERSION "Version 1.0"

namespace Quer
{
    enum class RunMode
    {
        RUN,    // Queue some commands and run them.
        RECORD, // Record some commands and save them to a 'querscript' file.
        PLAY,   // Plays back a recorded 'querscript' file.
    };

    // Usage: "quer" or "quer -r" or "quer -p"
    void Run(RunMode runMode, const char* querScriptName = nullptr)
    {
        printf("Running in run mode = %s.\n", 
            runMode == RunMode::RUN    ? "Run" :
            runMode == RunMode::RECORD ? "Record" :
            runMode == RunMode::PLAY   ? "Play" : "Unknown");
        if (querScriptName) printf("Script name = %s", querScriptName);
    }

    // Usage: "quer -h" or "quer -help"
    void Help()
    {
        printf("  +----------------------- Quer Help Menu --------------- Info -+\n");
        printf("  | -h, -help                                                   |\n");
        printf("  |                  Opens the help menu.                       |\n");
        printf("  | -v, -version                                                |\n");
        printf("  |                  Shows the current quer version.            |\n");
        printf("  | -u, -update                                                 |\n");
        printf("  |                  Updates quer to the latest version.        |\n");
        printf("  | -where                                                      |\n");
        printf("  |                  Shows installation directory.              |\n");
        printf("  |--------------------------------------------------- Actions -|\n");
        printf("  | -c, -clean                                                  |\n");
        printf("  |                  Removes all saved 'querscript' files.      |\n");
        printf("  | -r <name>, -record <name>                                   |\n");
        printf("  |                  Allows you to record a querscript file     |\n");
        printf("  |                  which can be played back using 'quer -p'.  |\n");
        printf("  | -p <name>, -play <name>                                     |\n");
        printf("  |                  Plays a saved querscript file.             |\n");
        printf("  | No arguments                                                |\n");
        printf("  |                  Runs quer normally to queue/run commands.  |\n");
        printf("  +-------------------------------------------------------------+\n");
    }

    // Usage: "quer -v" or "quer -version"
    void Version()
    {
        printf("[quer] The current version of quer is %s.\n", QUER_VERSION);
    }

    // TODO
    // Usage: "quer -u" or "quer -update"
    void Update()
    {
        printf("[quer] This feature is not supported yet.\n");
    }

    // Usage: "quer -where"
    void Where(const char* localPath)
    {
        printf("[quer] The quer installation is located in: %s\n", localPath);
    }

    // TODO
    // Usage: "quer -c" or "quer -clean"
    void Clean()
    {
        printf("[quer] This feature is not supported yet.\n");
    }

    void InvalidSyntax(const char* message)
    {
        printf("[quer] Invalid syntax: %s\n", message);
    }
}

int main(const int argc, const char* const* argv)
{
    const char* localPath = *argv;

    if (argc == 1) // No command line arguments.
    {
        Quer::Run(Quer::RunMode::RUN);
    }
    else if (argc == 2) // One command line argument.
    {
        if (strcmp(*(argv + 1), "-v") == 0 || strcmp(*(argv + 1), "-version") == 0)
        {
            Quer::Version();
        }
        else if (strcmp(*(argv + 1), "-h") == 0 || strcmp(*(argv + 1), "-help") == 0)
        {
            Quer::Help();
        }
        else if (strcmp(*(argv + 1), "-u") == 0 || strcmp(*(argv + 1), "-update") == 0)
        {
            Quer::Update();
        }
        else if (strcmp(*(argv + 1), "-where") == 0)
        {
            Quer::Where(localPath);
        }
        else if (strcmp(*(argv + 1), "-c") == 0 || strcmp(*(argv + 1), "-clean") == 0)
        {
            Quer::Clean();
        }
        // (Handle case where user tries to run -p or -r without a recording name)
        else if (strcmp(*(argv + 1), "-r") == 0 || strcmp(*(argv + 1), "-record") == 0 ||
            strcmp(*(argv + 1), "-p") == 0 || strcmp(*(argv + 1), "-play") == 0)
        {
            Quer::InvalidSyntax("'-play' and '-record' need a recording name to read from/write to. See 'quer -help'.");
        }
        else
        {
            Quer::InvalidSyntax("Unknown command argument encountered.");
        }
    }
    else if (argc == 3) // Two command line arguments.
    {
        if (strcmp(*(argv + 1), "-r") == 0 || strcmp(*(argv + 1), "-record") == 0)
        {
            const char* querScriptName = *(argv + 2);
            Quer::Run(Quer::RunMode::RECORD, querScriptName);
        }
        else if (strcmp(*(argv + 1), "-p") == 0 || strcmp(*(argv + 1), "-play") == 0)
        {
            const char* querScriptName = *(argv + 2);
            Quer::Run(Quer::RunMode::PLAY, querScriptName);
        }
        else
        {
            Quer::InvalidSyntax("Unknown command argument encountered.");
        }
    }
    else
    {
        Quer::InvalidSyntax("Unknown command argument encountered.");
    }
}
