#pragma once

// Quer [Header]
// 
// |==========================================================================|
// | Date          Person          Description                                |
// |==========================================================================|
// | 09-20-2023    Jake Huseman    Moved stuff to header file & added -ls     |
// |==========================================================================|

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <format>
#include <algorithm>

#include <Windows.h>

#define QUER_VERSION_MAJOR "1"
#define QUER_VERSION_MINOR "2"

namespace Quer
{
    constexpr const char* QUER_VERSION = "Version " QUER_VERSION_MAJOR "." QUER_VERSION_MINOR;

    enum class RunMode
    {
        RUN,    // Queue some commands and run them.
        RECORD, // Record some commands and save them to a 'querscript' file.
        PLAY,   // Plays back a recorded 'querscript' file.
    };

    void Run(RunMode runMode, const char* querScriptName = nullptr)
    {
        // Early exit for bad filepath.
        std::string querScriptFilePath = (querScriptName) ? std::format("C:/querscripts/{}.bat", querScriptName) : "";
        bool alreadyExists = std::filesystem::exists(std::filesystem::path(querScriptFilePath));
        if (runMode == RunMode::RECORD && alreadyExists)
        {
            printf("[quer] Quers cript '%s' already exists.\n", querScriptName);
            return;
        }
        else if (runMode == RunMode::PLAY && !alreadyExists)
        {
            printf("[quer] Quer script '%s' does not exist.\n", querScriptName);
            return;
        }

        // Print quer header
        printf(
            (runMode == RunMode::RECORD) ? "+----------  Quer (Record Mode) ---------+\n"
            : (runMode == RunMode::PLAY) ? "+--------- Quer (Playback Mode) ---------+\n"
            : "+----------------- Quer -----------------+\n");

        // Print quer instructions (if we aren't in Playback Mode)
        if (runMode != RunMode::PLAY)
        {
            printf("| Queue some commands.                   |\n");
            printf("|                                        |\n");
            printf("| > done  - Starts the queue.            |\n");
            printf("| > quit  - Cancels the queue.           |\n");
            printf("| (Use 'quer -h' for more commands.)     |\n");
            printf("+----------------------------------------+\n");
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

        size_t index = 1;
        bool abort = false;
        while (!abort)
        {
            // don't ask what this is doing...
            if (querScriptName && runMode == RunMode::PLAY)
            {
                abort = !std::getline(*inputStreamPtr, currentCommand);
                if (abort)
                {
                    break;
                }

                printf("%d. ", static_cast<unsigned int>(index++));
                printf("%s\n", currentCommand.c_str());
            }
            else
            {
                printf("%d. ", static_cast<unsigned int>(index++));
                abort = !std::getline(*inputStreamPtr, currentCommand);
            }

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
            else if (currentCommand == "show")
            {
                printf("Showing Current Commands:\n");
                std::for_each(queuedCommands.cbegin(), queuedCommands.cend(), [=, pos = 0](std::string a) mutable {printf("%d. %s\n", ++pos, a.c_str()); });
            }
            // Special command: "undo"
            else if (currentCommand.substr(0, 4).compare("undo") == 0)
            {
                if (queuedCommands.empty())
                    printf("Enter inputs before deleting\n");
                else {
                    if (currentCommand.size() == 4)
                    {
                        printf("(Command %d will not be performed.)\n", static_cast<unsigned int>(queuedCommands.size() + 1));
                        queuedCommands.pop_back();
                    }
                    else {
                        int remove_at;
                        try {
                            remove_at = std::stoi(currentCommand.substr(5)) - 1; // Users see everything in array at [pos + 1]
                            if (remove_at >= queuedCommands.size())
                                printf("Please enter a number within range of commands %d - %d\n", 1, (unsigned)(queuedCommands.size()));
                            else {
                                printf("(Command %d. %s will not be performed)\n", (remove_at + 1), queuedCommands[remove_at].c_str());
                                queuedCommands.erase(queuedCommands.cbegin() + remove_at); // Remove command at position
                            }
                        }
                        catch (std::invalid_argument& e) {
                            if (currentCommand.substr(5).compare("all") == 0) {
                                queuedCommands.clear();
                                printf("(All commands have been erased)\n");
                            }
                            else printf("Please enter a valid number or to undo last command just use 'undo'\n"); // Error message
                        }
                    }
                }
            }
            else
            {
                queuedCommands.push_back(currentCommand);
            }
        }

        // If we are in record mode, flush queuedCommands to the new querscript file.
        // If we are in run/playback mode, flush them to system.
        if (runMode == RunMode::RECORD)
        {
            std::ofstream outputStream(querScriptFilePath);
            for (const std::string& command : queuedCommands)
            {
                outputStream << command.c_str() << '\n';
            }
        }
        else
        {
            printf("+--------------- Executing --------------+\n");
            for (const std::string& command : queuedCommands)
            {
                system(command.c_str());
            }
        }
        printf("+----------------- Done -----------------+\n");
    }

    /*-------------------------------------------------------------------------*/
    // Usage: "quer -h" or "quer --help"
    void Help()
    {
        printf("+----------------------- Quer Help Menu ----------------------+\n");
        printf("| General usage:   quer [action] [filename]                   |\n");
        printf("+--------------------------------------------------- Actions -+\n");
        printf("| -h, --help                                                  |\n");
        printf("|                  Opens the help menu.                       |\n");
        printf("| -v, --version                                               |\n");
        printf("|                  Shows the current quer version.            |\n");
        printf("| -ls, --listscripts                                          |\n");
        printf("|                  Lists all saved quer scripts               |\n");
        printf("| -fs, --findscripts                                          |\n");
        printf("|                  Reveals quer script directory.             |\n");
        printf("| -c, --clean                                                 |\n");
        printf("|                  Removes all saved quer script .bat files.  |\n");
        printf("| -r <name>, --record <name>                                  |\n");
        printf("|                  Allows you to record a .bat file           |\n");
        printf("|                  which can be played back using 'quer -p'.  |\n");
        printf("| -p <name>, --play <name>                                    |\n");
        printf("|                  Plays a saved quer script .bat file.       |\n");
        printf("| No arguments                                                |\n");
        printf("|                  Runs quer normally to queue/run commands.  |\n");
        printf("|-------------------------------------------------- Commands -|\n");
        printf("| Type 'done' to start the queue.                             |\n");
        printf("| Type 'quit' to cancel immediately.                          |\n");
        printf("| Type 'undo' to undo the prev. command.                      |\n");
        printf("| Type 'show' to show currents commands.                      |\n");
        printf("+-------------------------------------------------------------+\n");
    }

    /*-------------------------------------------------------------------------*/
    // Usage: "quer -v" or "quer --version"
    void Version()
    {
        printf("[quer] Version %s.\n", QUER_VERSION);
    }

    /*-------------------------------------------------------------------------*/
    // Usage: "quer -ls" or "quer --listscripts"
    void ListScripts()
    {
        auto querscripts_path = std::filesystem::path("C:\\querscripts\\");
        for (const auto& entry : std::filesystem::directory_iterator(querscripts_path))
        {
            printf("%s ", entry.path().stem().generic_string().c_str());
        }
    }

    /*-------------------------------------------------------------------------*/
    // Usage: "quer -fs" or "quer --findscripts"
    void FindScripts()
    {
        system("explorer C:\\querscripts");
    }

    /*-------------------------------------------------------------------------*/
    // Usage: "quer -c" or "quer --clean"
    void Clean()
    {
        auto querscripts_path = std::filesystem::path("C:\\querscripts\\");
        for (const auto& entry : std::filesystem::directory_iterator(querscripts_path))
        {
            std::filesystem::remove_all(entry.path());
        }
    }

    /*-------------------------------------------------------------------------*/
    // Prints for any invalid input.
    void InvalidSyntax(const char* message)
    {
        printf("[quer] Invalid syntax: %s\n", message);
    }
}