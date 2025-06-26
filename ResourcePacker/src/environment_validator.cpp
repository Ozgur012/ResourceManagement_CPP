#include "environment_validator.hpp"
#include "error_checker.hpp"


namespace ResourceManagement::Validator
{
    bool is_valid_environment(const std::string& config_path)
    {
        bool result = true;
        
        if (!ErrorChecker::Environment::_is_json_path_supplied(config_path))
        {
            result = false;
        }

        std::string unix_path = config_path;
        ErrorChecker::Utils::_unixify_path(unix_path);

        if (!ErrorChecker::Environment::_is_path_json_file(unix_path))
        {
            result = false;
        }

        if (!ErrorChecker::Environment::_is_json_file_openable(unix_path))
        {
            result = false;
        }

        if (!ErrorChecker::Environment::_is_valid_pack_config_json(unix_path))
        {
            result = false;
        }

        ErrorChecker::Utils::_flush_logs();


        return result;
    }

}
