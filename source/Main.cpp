// Main [Source]
// 
// |==========================================================================|
// | Date          Person          Description                                |
// |==========================================================================|
// | xx-xx-2023    Jake Huseman    Initial quer.bat                           |
// | 06-23-2023    Aidan Sommer    Added execution time                       |
// | 07-06-2023    Jake Huseman    Added 'quit' and better display            |
// | 07-19-2023    Group effort    Moved to github and C++ project            |
// | 07-20-2023    Jake Huseman    Added basic queueing + running func        |
// | 07-23-2023    Aidan Sommer    Added show and more func. for undo         |
// | 09-20-2023    Jake Huseman    Removed broken things -> v1.2              |
// |==========================================================================|

#include <string>

#include "Quer.h"

int main(const int argc, const char* const* argv)
{
    const char* localPath = *argv;

    if (argc == 1) // No command line arguments.
    {
        Quer::Run(Quer::RunMode::RUN);
    }
    else if (argc == 2) // One command line argument.
    {
        if (std::strcmp(*(argv + 1), "-v") == 0 || std::strcmp(*(argv + 1), "--version") == 0)
        {
            Quer::Version();
        }
        else if (std::strcmp(*(argv + 1), "-h") == 0 || std::strcmp(*(argv + 1), "--help") == 0)
        {
            Quer::Help();
        }
        else if (std::strcmp(*(argv + 1), "-ls") == 0 || std::strcmp(*(argv + 1), "--listscripts") == 0)
        {
            Quer::ListScripts();
        }
        else if (std::strcmp(*(argv + 1), "-fs") == 0 || std::strcmp(*(argv + 1), "--findscripts") == 0)
        {
            Quer::FindScripts();
        }
        else if (std::strcmp(*(argv + 1), "-c") == 0 || std::strcmp(*(argv + 1), "--clean") == 0)
        {
            Quer::Clean();
        }
        // (Handle case where user tries to run -p or -r without a recording name)
        else if (std::strcmp(*(argv + 1), "-r") == 0 || std::strcmp(*(argv + 1), "--record") == 0 ||
            std::strcmp(*(argv + 1), "-p") == 0 || std::strcmp(*(argv + 1), "--play") == 0)
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
        if (std::strcmp(*(argv + 1), "-r") == 0 || std::strcmp(*(argv + 1), "--record") == 0)
        {
            const char* querScriptName = *(argv + 2);
            Quer::Run(Quer::RunMode::RECORD, querScriptName);
        }
        else if (std::strcmp(*(argv + 1), "-p") == 0 || std::strcmp(*(argv + 1), "--play") == 0)
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
