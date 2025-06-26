#include "pack_maker.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "error_checker.hpp"
#include "nlohmann/json.hpp"

namespace ResourceManagement::PackMaker
{
    void make_resource_pack(std::string &pack_config_path)
    {
        std::ifstream _json_file(pack_config_path);

        nlohmann::json pack_config_json = nlohmann::json::parse(_json_file);

        std::filesystem::path _input_dir = pack_config_json["input_dir"];

        for (auto &item : std::filesystem::recursive_directory_iterator(_input_dir))
        {
            if (item.is_regular_file())
            {
                std::string _access_path = _input_dir.stem().string() + "/" + item.path().lexically_relative(_input_dir).generic_string(); // Generic string must be used to ensure that the path usses "/" instead of "\\"
                uint32_t _access_path_size = static_cast<uint32_t>(_access_path.size());

                std::vector<char> buffer;
                uint32_t data_size = buffer.size();

                if (Private::_bake_binary_data(buffer, item.path().string()) == ErrorChecker::ErrorFlags::Flags::FAILURE)
                {
                    continue;
                }

                Private::PackEntry _entry{
                    .entry_name = _access_path,
                    .entry_name_size = _access_path_size,
                    .entry_end = 0
                };
                
                
                ErrorChecker::Utils::_log_success(ErrorChecker::SuccessTypes::FILE, ": buffer size = " + std::to_string(data_size));
                std::cout << "_access_path = " << _access_path << ": length = " << _access_path.length() << ": _entry.entry_name_size = " << _entry.entry_name_size << "\n";
            }
        }
        ErrorChecker::Utils::_flush_logs();
        std::cout << "sizeof(Private::PackEntry) = " << sizeof(Private::PackEntry) << "\n";
    }
}

namespace ResourceManagement::PackMaker::Private
{
    ErrorChecker::ErrorFlags::Flags _bake_binary_data(std::vector<char> &to, std::filesystem::path file_path)
    {
        ErrorChecker::ErrorFlags::Flags result = ErrorChecker::ErrorFlags::Flags::SUCCESS;

        std::ifstream in(file_path, std::ios::binary | std::ios::ate);


        if (!in.is_open())
        {
            result = ErrorChecker::ErrorFlags::Flags::FAILURE;
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, " Unable to open file at: " + file_path.generic_string());
        } else{
            ErrorChecker::Utils::_log_success(ErrorChecker::SuccessTypes::FILE, ": File Opened at = " + file_path.generic_string());
        }

        if (result == ErrorChecker::ErrorFlags::Flags::SUCCESS)
        {
            size_t size = in.tellg();
            in.seekg(0, std::ios::beg);

            to.resize(size);

            in.read(to.data(), size);

            in.close();
        }

        return result;
    }
}