#include <iostream>
#include "command_line_interface.hpp"

int main(int argc, char* argv[])
{
    int result = rm::CLI::run(argc, argv);
    
    return result;
}
