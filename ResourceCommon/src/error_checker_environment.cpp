#include "error_checker.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace rm::err::EncryptionFlag
{
    bool apply_encryption = true;
}

namespace rm::err::Environment
{
    bool _is_json_path_supplied(const std::string& config_path)
    {
        if (config_path.empty())
        {
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::CONFIG,
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
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::PATH,
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
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::FILE,
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
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::SYSTEM,
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
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::JSON,
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
                rm::err::Utils::_log_error(
                    rm::err::ErrorTypes::VALIDATION,
                    "JSON is missing required key: \"" + key + "\""
                );
            }
        };

        check_key(rm::err::Private::input_dir);
        check_key(rm::err::Private::output_dir_debug);
        check_key(rm::err::Private::output_dir_release);
        check_key(rm::err::Private::resource_pack_file_name);
        check_key(rm::err::Private::encryption_key);

        // Encryption
        if (data.contains(rm::err::Private::encryption_key) 
                && data[rm::err::Private::encryption_key].is_string() 
                && data[rm::err::Private::encryption_key].get<std::string>().empty()) {
                    
                    rm::err::EncryptionFlag::apply_encryption = false;

                    rm::err::Utils::_log_error(
                        rm::err::ErrorTypes::CONFIG,
                        "\"encryption_key\" is empty. Resources will not be encrypted.");
        } else {
            rm::err::Utils::_log_success(
                rm::err::ErrorTypes::CONFIG,
                "\"encryption_key\" is supplied. Resources will be encrypted.");
        }

        if (result)
        {
            auto check_dir = [&](const std::string& key) {
                const std::string& dir = data[key].get<std::string>();
                if (!std::filesystem::exists(dir)) {
                    result = false;
                    rm::err::Utils::_log_error(
                        rm::err::ErrorTypes::PATH,
                        "Directory path in \"" + key + "\" does not exist: \"" + dir + "\""
                    );
                }
            };

            check_dir(rm::err::Private::input_dir);
            check_dir(rm::err::Private::output_dir_debug);
            check_dir(rm::err::Private::output_dir_release);
        }
        file.close();
        return result;
    }
}
