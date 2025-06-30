#include "error_checker.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace ResourceManagement::ErrorChecker::EncryptionFlag
{
    bool apply_encryption = true;
}

namespace ResourceManagement::ErrorChecker::Environment
{
    bool _is_json_path_supplied(const std::string& config_path)
    {
        if (config_path.empty())
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::CONFIG,
                "Missing resource configuration file path.\n\n"
                "Pass the JSON config file as an argument to --pack\n"
                "Example: ResourcePacker.exe --pack path/to/config.json"
            );
            return false;
        }

        return true;
    }

    bool _is_path_json_file(std::string& path)
    {
        if (path.size() < 5 || path.substr(path.size() - 5) != ".json")
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::PATH,
                "File does not have a .json extension: " + path
            );
            return false;
        }

        std::ifstream file(path);
        bool result = file.is_open();
        file.close();
        return result;
    }

    bool _is_json_file_openable(const std::string& path)
    {
        std::ifstream file(path);
        bool result = file.is_open();
        file.close();
        if (!result)
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::FILE,
                "Failed to open JSON configuration file at path: " + path
            );
        }
        
        return result;
    }

    bool _is_valid_pack_config_json(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::SYSTEM,
                "Unexpected internal error: could not open json file."
            );
            return false;
        }

        nlohmann::json data;
        try
        {
            data = nlohmann::json::parse(file);
        }
        catch (const std::exception& e)
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::JSON,
                "Failed to parse JSON file at path: " + path + "\n\n"
                "Parse Error:\n" + std::string(e.what()) + "\n\n"
                "Ensure that the JSON file is correctly formatted."
            );
            return false;
        }

        bool result = true;

        auto check_key = [&](const std::string& key) {
            if (!data.contains(key)) {
                result = false;
                ResourceManagement::ErrorChecker::Utils::_log_error(
                    ResourceManagement::ErrorChecker::ErrorTypes::VALIDATION,
                    "JSON is missing required key: \"" + key + "\""
                );
            }
        };

        check_key(ResourceManagement::ErrorChecker::Private::input_dir);
        check_key(ResourceManagement::ErrorChecker::Private::output_dir_debug);
        check_key(ResourceManagement::ErrorChecker::Private::output_dir_release);
        check_key(ResourceManagement::ErrorChecker::Private::resource_pack_file_name);
        check_key(ResourceManagement::ErrorChecker::Private::encryption_key);

        // Encryption
        if (data.contains(ResourceManagement::ErrorChecker::Private::encryption_key) 
                && data[ResourceManagement::ErrorChecker::Private::encryption_key].is_string() 
                && data[ResourceManagement::ErrorChecker::Private::encryption_key].get<std::string>().empty()) {
                    
                    ResourceManagement::ErrorChecker::EncryptionFlag::apply_encryption = false;

                    ResourceManagement::ErrorChecker::Utils::_log_error(
                        ResourceManagement::ErrorChecker::ErrorTypes::CONFIG,
                        "\"encryption_key\" is empty. Resources will not be encrypted.");
        } else {
            ResourceManagement::ErrorChecker::Utils::_log_success(
                ResourceManagement::ErrorChecker::ErrorTypes::CONFIG,
                "\"encryption_key\" is supplied. Resources will be encrypted.");
        }

        if (result)
        {
            auto check_dir = [&](const std::string& key) {
                const std::string& dir = data[key].get<std::string>();
                if (!std::filesystem::exists(dir)) {
                    result = false;
                    ResourceManagement::ErrorChecker::Utils::_log_error(
                        ResourceManagement::ErrorChecker::ErrorTypes::PATH,
                        "Directory path in \"" + key + "\" does not exist: \"" + dir + "\""
                    );
                }
            };

            check_dir(ResourceManagement::ErrorChecker::Private::input_dir);
            check_dir(ResourceManagement::ErrorChecker::Private::output_dir_debug);
            check_dir(ResourceManagement::ErrorChecker::Private::output_dir_release);
        }
        file.close();
        return result;
    }
}
