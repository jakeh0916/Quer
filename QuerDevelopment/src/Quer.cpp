// Quer [Source]
// 
// |==========================================================================|
// | Date          Person          Description                                |
// |==========================================================================|
// | xx-xx-2023    Jake Huseman    Initial quer.bat                           |
// | 06-23-2023    Aidan Sommer    Added execution time                       |
// | 07-06-2023    Jake Huseman    Added 'quit' and better display            |
// | 07-19-2023    Group effort    Moved to github and C++ project            |
// | 07-20-2023    Jake Huseman    Added basic queueing + running func        |
// |==========================================================================|

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <format>

#include <Windows.h>

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
        // Early exit for bad filepath.
        std::string querScriptFilePath = (querScriptName) ? std::format("C:/querscripts/{}.querscript", querScriptName) : "";
        bool alreadyExists = std::filesystem::exists(std::filesystem::path(querScriptFilePath));
        if (runMode == RunMode::RECORD && alreadyExists)
        {
            printf("[quer] Querscript '%s' already exists.\n", querScriptName);
            return;
        }
        else if (runMode == RunMode::PLAY && !alreadyExists)
        {
            printf("[quer] Querscript '%s' does not exist.\n", querScriptName);
            return;
        }

        // Print quer header
        printf(
            (runMode == RunMode::RECORD) ? "  +----------  Quer (Record Mode) ---------+\n"
            : (runMode == RunMode::PLAY) ? "  +--------- Quer (Playback Mode) ---------+\n"
                                         : "  +----------------- Quer -----------------+\n");

        // Print quer instructions (if we aren't in Playback Mode)
        if (runMode != RunMode::PLAY)
        {
            printf("  | Queue some commands                    |\n");
            printf("  | (Use 'quer -h' for more commands.)     |\n");
            printf("  |                                        |\n");
            printf("  | Type 'done' to start the queue.        |\n");
            printf("  | Type 'quit' to cancel immediately.     |\n");
            printf("  | Type 'undo' to undo the prev. command. |\n");
            printf("  +----------------------------------------+\n");
        }

        // If we are in run/record mode, this pointer -> std::cin
        // If we are in playback mode, this pointer -> std::ifstream (which must be freed)
        auto inputStreamPtr =
            std::unique_ptr<std::istream, void(*)(std::istream*)>(
                (runMode == RunMode::PLAY)
                    ? new std::ifstream(querScriptFilePath)
                    : &std::cin,
            /* custom deleter */ [](std::istream* doomedStream)
            {
                if (doomedStream && doomedStream != &std::cin) delete doomedStream;
            }
        );

        // Reject bad istreams.
        // (Probably the user passed in an invalid filepath.)
        if (!inputStreamPtr->good())
        {
            printf("[quer] Could not read file.\n");
            return;
        }

        std::vector<std::string> queuedCommands{};
        std::string currentCommand{};

        printf("1. ");
        size_t index = 1;
        while (std::getline(*inputStreamPtr, currentCommand))
        {
            // Special command: "done"
            if (currentCommand == "done")
            {
                break;
            }
            // Special command: "quit"
            else if (currentCommand == "quit")
            {
                return;
            }
            // Special command: "undo"
            else if (currentCommand == "undo" && !queuedCommands.empty())
            {
                queuedCommands.pop_back();
                printf("(Command %d will not be performed.)\n", static_cast<unsigned int>(queuedCommands.size()));
            }
            else
            {
                queuedCommands.push_back(currentCommand);
            }

            printf("%d. ", static_cast<unsigned int>(++index));
        }

        // If we are in record mode, flush queuedCommands to the new querscript file.
        // If we are in run/playback mode, flush them to system.
        if (runMode == RunMode::RECORD)
        {
            printf("  +-------------- Recording... ------------+\n");
            std::ofstream outputStream(querScriptFilePath);
            for (const std::string& command : queuedCommands)
            {
                outputStream << command.c_str() << '\n';
            }
        }
        else
        {
            printf("  +--------------- Executing --------------+\n");
            for (const std::string& command : queuedCommands)
            {
                system(command.c_str());
            }
        }
        printf("  +----------------- Done -----------------+\n");
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
        printf("  |                  Reveals querscript directory.              |\n");
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
        printf("[quer] The quer installation is located in: C:/querscripts/\n");
        system("explorer C:\\querscripts");
    }

    // TODO
    // Usage: "quer -c" or "quer -clean"
    void Clean()
    {
        printf("[quer] This feature is not supported yet.\n");
    }

    // Prints for any invalid input.
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
        if (std::strcmp(*(argv + 1), "-v") == 0 || std::strcmp(*(argv + 1), "-version") == 0)
        {
            Quer::Version();
        }
        else if (std::strcmp(*(argv + 1), "-h") == 0 || std::strcmp(*(argv + 1), "-help") == 0)
        {
            Quer::Help();
        }
        else if (std::strcmp(*(argv + 1), "-u") == 0 || std::strcmp(*(argv + 1), "-update") == 0)
        {
            Quer::Update();
        }
        else if (std::strcmp(*(argv + 1), "-where") == 0)
        {
            Quer::Where(localPath);
        }
        else if (std::strcmp(*(argv + 1), "-c") == 0 || std::strcmp(*(argv + 1), "-clean") == 0)
        {
            Quer::Clean();
        }
        // (Handle case where user tries to run -p or -r without a recording name)
        else if (std::strcmp(*(argv + 1), "-r") == 0 || std::strcmp(*(argv + 1), "-record") == 0 ||
            std::strcmp(*(argv + 1), "-p") == 0 || std::strcmp(*(argv + 1), "-play") == 0)
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
        if (std::strcmp(*(argv + 1), "-r") == 0 || std::strcmp(*(argv + 1), "-record") == 0)
        {
            const char* querScriptName = *(argv + 2);
            Quer::Run(Quer::RunMode::RECORD, querScriptName);
        }
        else if (std::strcmp(*(argv + 1), "-p") == 0 || std::strcmp(*(argv + 1), "-play") == 0)
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
