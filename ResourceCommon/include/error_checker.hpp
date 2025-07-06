#ifndef RESOURCE_MANAGEMENT_CPP_ERROR_CHECKER_H
#define RESOURCE_MANAGEMENT_CPP_ERROR_CHECKER_H
#include <string>
#include <unordered_set>

// rm -> ResourceManagement
namespace rm::err::errFlags
{
    enum class Flags{ SUCCESS, FAILURE};
}

namespace rm::err::EncryptionFlag
{
    extern bool apply_encryption; // _is_valid_pack_config_json function in validation changes this.
}

namespace rm::err::Private
{
    inline const std::string input_targets = "input_targets";
    inline const std::string output_dir_debug = "output_dir_debug";
    inline const std::string output_dir_release = "output_dir_release";
    inline const std::string encryption_key = "encryption_key";
}

namespace rm::err::ErrorTypes
{
    inline const std::string FILE = "[FILE]";
    inline const std::string JSON = "[JSON]";
    inline const std::string VALIDATION = "[VALIDATION]";
    inline const std::string PATH = "[PATH]";
    inline const std::string CONFIG = "[CONFIG]";
    inline const std::string SYSTEM = "[SYSTEM]";
}

namespace rm::err::SuccessTypes
{
    inline const std::string VALIDATION = "[VALIDATION]";
    inline const std::string FILE       = "[FILE]";
    inline const std::string CONFIG     = "[CONFIG]";
    inline const std::string SYSTEM     = "[SYSTEM]";
    inline const std::string JSON       = "[JSON]";
    inline const std::string PATH       = "[PATH]";
}

namespace rm::err::Utils
{
    void _unixify_path(std::string &path);

    void _begin_log_buffer();
    void _log_info(const std::string &message);
    void _log_error(const std::string& type, const std::string& message);
    void _log_success(const std::string& type, const std::string& message);
    void _end_log_bugger();

}

namespace rm::err::Environment
{
    bool _is_json_path_supplied(const std::string& config_path);
    bool _is_path_json_file(std::string &path);
    bool _is_json_file_openable(const std::string& path);
    bool _is_valid_pack_config_json(const std::string& path);
}


namespace rm::err::Packing
{
    void _log_packing(errFlags::Flags error_flag, std::string message);
}

#endif // RESOURCE_MANAGEMENT_ERROR_CHECKER_H