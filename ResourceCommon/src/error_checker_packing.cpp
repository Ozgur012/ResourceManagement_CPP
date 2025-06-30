#include "error_checker.hpp"
#include <iostream>

namespace ResourceManagement::ErrorChecker::Packing
{
    void _log_packing(ErrorFlags::Flags error_flag, std::string message)
    {
        std::string prefix = "[PACKING]";

        if (error_flag == ErrorFlags::Flags::FAILURE)
        {
            std::cout << prefix << " ERROR: " << message << "\n";
        }
        else
        {
            std::cout << "\r\033[K" << prefix << " SUCCESS: " << message << std::flush;
            
        }
    }
}
