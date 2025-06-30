#ifndef RESOURCE_MANAGEMENT_CPP_ERROR_CHECKER_H
#define RESOURCE_MANAGEMENT_CPP_ERROR_CHECKER_H
#include <string>
#include <unordered_set>

namespace ResourceManagement::ErrorChecker::ErrorFlags
{
    enum class Flags{ SUCCESS, FAILURE};
}

namespace ResourceManagement::ErrorChecker::EncryptionFlag
{
    extern bool apply_encryption; // _is_valid_pack_config_json function in validation changes this.
}

namespace ResourceManagement::ErrorChecker::Private
{
    inline const std::string input_dir = "input_dir";
    inline const std::string output_dir_debug = "output_dir_debug";
    inline const std::string output_dir_release = "output_dir_release";
    inline const std::string resource_pack_file_name = "resource_pack_file_name";
    inline const std::string encryption_key = "encryption_key";
}

namespace ResourceManagement::ErrorChecker::ErrorTypes
{
    inline const std::string FILE = "[FILE]";
    inline const std::string JSON = "[JSON]";
    inline const std::string VALIDATION = "[VALIDATION]";
    inline const std::string PATH = "[PATH]";
    inline const std::string CONFIG = "[CONFIG]";
    inline const std::string SYSTEM = "[SYSTEM]";
}

namespace ResourceManagement::ErrorChecker::SuccessTypes
{
    inline const std::string VALIDATION = "[VALIDATION]";
    inline const std::string FILE       = "[FILE]";
    inline const std::string CONFIG     = "[CONFIG]";
    inline const std::string SYSTEM     = "[SYSTEM]";
    inline const std::string JSON       = "[JSON]";
    inline const std::string PATH       = "[PATH]";
}

namespace ResourceManagement::ErrorChecker::Utils
{
    void _unixify_path(std::string &path);

    void _begin_log_buffer();
    void _log_error(const std::string& type, const std::string& message);
    void _log_success(const std::string& type, const std::string& message);
    void _end_log_bugger();

}

namespace ResourceManagement::ErrorChecker::Environment
{
    bool _is_json_path_supplied(const std::string& config_path);
    bool _is_path_json_file(std::string &path);
    bool _is_json_file_openable(const std::string& path);
    bool _is_valid_pack_config_json(const std::string& path);
}


namespace ResourceManagement::ErrorChecker::Packing
{
    static const std::unordered_set<std::string> valid_extentions = {
        ".png", ".jpg", ".jpeg", ".ogg", ".wav", ".mp3"
    };
    void _log_packing(ErrorFlags::Flags error_flag, std::string message);
}

#endif // RESOURCE_MANAGEMENT_ERROR_CHECKER_H