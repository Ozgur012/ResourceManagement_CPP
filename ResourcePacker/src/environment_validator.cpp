#include "environment_validator.hpp"
#include "error_checker.hpp"


namespace rm::Validator
{
    bool is_valid_environment(const std::string& config_path)
    {
        bool result = true;
        
        if (!err::Environment::_is_json_path_supplied(config_path))
        {
            result = false;
        }

        std::string unix_path = config_path;
        err::Utils::_unixify_path(unix_path);

        if (!err::Environment::_is_path_json_file(unix_path))
        {
            result = false;
        }

        if (!err::Environment::_is_json_file_openable(unix_path))
        {
            result = false;
        }

        if (!err::Environment::_is_valid_pack_config_json(unix_path))
        {
            result = false;
        }

        return result;
    }

}
