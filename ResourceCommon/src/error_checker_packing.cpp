#include "error_checker.hpp"
#include <iostream>

namespace rm::err::Packing
{
    void _log_packing(errFlags::Flags error_flag, std::string message)
    {
        std::string prefix = "[PACKING]";

        if (error_flag == errFlags::Flags::FAILURE)
        {
            std::cout << prefix << " ERROR: " << message << "\n";
        }
        else
        {
            std::cout << "\r\033[K" << prefix << " SUCCESS: " << message << std::flush;
            
        }
    }
}
