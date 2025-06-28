#include "error_checker.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

namespace ResourceManagement::ErrorChecker::Utils
{
    inline std::vector<std::string> m_error_logs;
    inline std::vector<std::string> m_success_logs;

    void _log_error(const std::string& type, const std::string& message)
    {
        if (!message.empty())
            m_error_logs.push_back(type + " ERROR: " + message);
    }

    void _log_success(const std::string& type, const std::string& message)
    {
        if (!message.empty())
            m_success_logs.push_back(type + " SUCCESS: " + message);
    }

    void _flush_logs()
    {
        if (!m_success_logs.empty())
        {
            std::cout << "\n========RESOURCE_PACKER_SUCCESS_LOG===========================================\n";
            for (size_t i = 0; i < m_success_logs.size(); ++i)
                std::cout << " " << std::setw(2) << (i + 1) << ". " << m_success_logs[i] << "\n";
            std::cout << "=============================================================================\n\n";
        }

        if (!m_error_logs.empty())
        {
            std::cout << "\n========RESOURCE_PACKER_ERROR_LOG=============================================\n";
            for (size_t i = 0; i < m_error_logs.size(); ++i)
                std::cout << " " << std::setw(2) << (i + 1) << ". " << m_error_logs[i] << "\n";
            std::cout << "=============================================================================\n\n";
        }

        m_success_logs.clear();
        m_error_logs.clear();
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
            _log_success(SuccessTypes::SYSTEM, " Path normalized (Unixified):\n"
                         "      Input: " + _initial_path + "\n"
                         "      Normalized: " + path + "\n");
        }
    }
}
