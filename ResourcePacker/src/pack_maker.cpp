#include "pack_maker.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "error_checker.hpp"
#include "nlohmann/json.hpp"
#include "pack_format.hpp"

namespace ResourceManagement::PackMaker
{
    void make_resource_pack(std::string &pack_config_path, std::string build_type)
    {
        std::ifstream _json_file(pack_config_path);
        nlohmann::json pack_config_json = nlohmann::json::parse(_json_file);
        std::filesystem::path _input_dir = pack_config_json["input_dir"];
        std::string _output_dir = (build_type == "Release") ? pack_config_json["output_dir_release"] : pack_config_json["output_dir_debug"];

        std::filesystem::path _resouce_pack_file_name = pack_config_json["resource_pack_file_name"];


        std::string resource_pack_file_path;
        if (_resouce_pack_file_name.has_extension())
        {
            resource_pack_file_path =  _output_dir + "/" + _resouce_pack_file_name.generic_string();
        } else{
            resource_pack_file_path =  _output_dir + "/" + _resouce_pack_file_name.generic_string() + PackFormat::PACK_EXTENTION;
        }
        

        std::vector<Private::PackEntry> pack_entires;

        for (auto &item : std::filesystem::recursive_directory_iterator(_input_dir))
        {
            if (item.is_regular_file())
            {
                std::string _access_path = _input_dir.stem().string() + "/" + item.path().lexically_relative(_input_dir).generic_string(); // Generic string must be used to ensure that the path usses "/" instead of "\\"
                uint8_t _access_path_size = static_cast<uint8_t>(_access_path.size());

                Private::PackEntry _entry;
                _entry.entry_name = _access_path;
                _entry.entry_name_size = _access_path_size;

                
                
                if (Private::_extract_binary_data(_entry.buffer, item.path().string()) == ErrorChecker::ErrorFlags::Flags::FAILURE)
                {
                    continue;
                }
                
                _entry.entry_total_size = PackFormat::ENTRY_HEADER_FIXED_SIZE + _access_path_size + _entry.buffer.size();

                pack_entires.push_back(_entry);

            }
        }

        Private::_pack_binaries_to_resource_file(pack_entires, resource_pack_file_path);

    }
}

namespace ResourceManagement::PackMaker::Private
{
    ErrorChecker::ErrorFlags::Flags _extract_binary_data(std::vector<char> &to, std::filesystem::path file_path)
    {
        ErrorChecker::ErrorFlags::Flags result = ErrorChecker::ErrorFlags::Flags::SUCCESS;

        std::ifstream in(file_path, std::ios::binary | std::ios::ate);


        if (!in.is_open())
        {
            result = ErrorChecker::ErrorFlags::Flags::FAILURE;
        
            ErrorChecker::Packing::_log_packing(ErrorChecker::ErrorFlags::Flags::FAILURE, " Unable to open file at: " + file_path.generic_string());
        } 
        else{
            ErrorChecker::Packing::_log_packing(ErrorChecker::ErrorFlags::Flags::SUCCESS, " File Opened at: " + file_path.generic_string());
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

    void _pack_binaries_to_resource_file(std::vector<PackEntry> &pack_entries, std::string _res_file_path)
    {
        if (pack_entries.empty())
        {
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, ": No valid files to pack. Input directory is empty. Failed to create resource pack.");
            
            return;
        }

        std::vector<char> pack_buffer;
        uint32_t entry_count;

        // Entry count. 4 bytes at the start of the file.
        {
            // small endian
            entry_count = pack_entries.size();
            pack_buffer.push_back(entry_count & 0xff);
            pack_buffer.push_back((entry_count >> 8) & 0xff);
            pack_buffer.push_back((entry_count >> 16) & 0xff);
            pack_buffer.push_back((entry_count >> 24) & 0xff);
        }

        for (auto &item : pack_entries)
        {
            // entry_name_size: 1 byte
            {
                pack_buffer.push_back(item.entry_name_size);
            }

            // entry_total_size: 4 bytes
            {
                // small endian
                
                pack_buffer.push_back(item.entry_total_size & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 8) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 16) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 24) & 0xff);
            }

            // entry_name: Dynamic
            {
                pack_buffer.insert(pack_buffer.end(), item.entry_name.begin(), item.entry_name.end());
            }

            //buffer: Dynamic
            {
                pack_buffer.insert(pack_buffer.end(), item.buffer.begin(), item.buffer.end())    ;
            }
            ErrorChecker::Packing::_log_packing(ErrorChecker::ErrorFlags::Flags::SUCCESS, "Resource packed with access name: " + item.entry_name);
        }

        ErrorChecker::Packing::_log_packing(ErrorChecker::ErrorFlags::Flags::SUCCESS, "Resource file path = " + _res_file_path);
        ErrorChecker::Utils::_log_success(ErrorChecker::SuccessTypes::FILE, "Packed = " + std::to_string(entry_count) + " files");

        // Write to pack file
        pack_buffer.shrink_to_fit();
        std::ofstream out(_res_file_path, std::ios::binary | std::ios::out);
        out.write(pack_buffer.data(), pack_buffer.size());
        out.close();

        std::cout << "FINISHING...\n";
    }
}