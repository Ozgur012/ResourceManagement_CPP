#include "error_checker.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

namespace rm::err::Utils
{
    inline std::vector<std::string> m_error_logs;
    inline std::vector<std::string> m_success_logs;

    void _begin_log_buffer()
    {
        std::cout << "\n========RESOURCE_PACKER_START===========================================\n";
    }

    void _log_info(const std::string &message)
    {
        if (!message.empty())
            std::cout << '\n' << "[INFO]" << message;
    }

    void _log_error(const std::string& type, const std::string& message)
    {
        if (!message.empty())
            std::cout << type << ": ERROR: " << message << "\n";
            //m_error_logs.push_back(type + " ERROR: " + message);
    }

    void _log_success(const std::string& type, const std::string& message)
    {
        if (!message.empty())
            std::cout << "\n" << type << ": SUCCESS: " << message << "\n";
            //m_success_logs.push_back(type + " SUCCESS: " + message);
    }

    
    void _end_log_bugger()
    {
        std::cout << "========RESOURCE_PACKER_END=======================================\n\n";
    }


    void _unixify_path(std::string &path)
    {
        std::string _initial_path = path;
        bool result = false;
        for (auto &i : path)
        {
            if (i == '\\')
            {
                i = '/';
                result = true;
            }
        }

        if (result)
        {
            _log_success(SuccessTypes::SYSTEM, " Output directory path normalized (Unixified): " + path);
        }
    }
}
