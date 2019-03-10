#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>

/**
 * stomp - Simple utility to truncate file(s)
 *
 * A useful utility for files such as logs that need to exist but be made empty.
 *
 * Generally ignores exceptions.
 *
 * 1.0 Initial implementation
 */

char *arg;
bool isDelete = false;
bool quiet = false;
bool argIs(const char *cmp);
void parseOptions(int argc, char **argv);
void stompArguments(int argc, char **argv);
void usage();

int main(int argc, char* argv[])
{
    parseOptions(argc, argv);
    stompArguments(argc, argv);
    return 0;
}

bool argIs(const char *cmp)
{
    return strcmp(arg, cmp) == 0;
}

void parseOptions(int argc, char **argv)
{
    // if there are no arguments show usage
    if (argc < 2) // program path is argv[0]
    {
        usage();
        exit(0);
    }

    for (int i = 1; i < argc; ++i) // skip program path argv[0]
    {
        arg = argv[i];
        if (arg[0] == '-') // skip non-options
        {
            if (argIs("-d"))
            {
                isDelete = true;
            }
            else if (argIs("-q"))
            {
                quiet = true;
            }
            else if (argIs("-?") || argIs("-h") || argIs("--help"))
            {
                usage();
                exit(0);
            }
        }
    }
}

void stompArguments(int argc, char **argv)
{
    int count = 0;
    for (int i = 1; i < argc; ++i)
    {
        char *arg = argv[i];
        if (arg[0] != '-') // skip options
        {
            try
            {
                if (isDelete) {
                    unlink(arg);
                }
                else {
                    std::fstream fs(arg, std::fstream::out | std::fstream::trunc);
                }
                ++count;
            }
            catch(const std::runtime_error& re)
            {
                //std::cerr << "Runtime error: " << re.what() << std::endl;
            }
            catch(const std::exception& ex)
            {
                //std::cerr << "Error occurred: " << ex.what() << std::endl;
            }
            if ( ! quiet)
            {
                std::cout << (isDelete ? "deleted " : "stomped ") << argv[i] << std::endl;
            }
        }
    }
    if ( ! quiet)
    {
        std::cout << (isDelete ? "deleted " : "stomped ") << count << " files" << std::endl;
    }
}

void usage()
{
    std::cout << "" << std::endl;
    std::cout << "stomp v1.1.0 - Truncate one or more files" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Usage: stomp [-q] file1 file2 ..." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -d  Delete the file instead of truncating" << std::endl;
    std::cout << "  -q  Quiet, do not output anything" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "If a path or filename contains spaces enclose it in double-quotes." << std::endl;
    std::cout << "" << std::endl;
}
