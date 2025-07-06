#include "pack_maker.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include "rm_config.hpp"
#include "error_checker.hpp"
#include "nlohmann/json.hpp"
#include "pack_format.hpp"

namespace rm::PackMaker
{
    void make_resource_pack(std::string &pack_config_path, std::string build_type)
    {
        
        std::vector<std::string> _unsupported_files;
        std::vector<std::string> _files_failed_to_open;
        
        std::ifstream _json_file(pack_config_path);
        nlohmann::json pack_config_json = nlohmann::json::parse(_json_file);
        
        std::string _output_dir = (build_type == "Release") ? pack_config_json["output_dir_release"] : pack_config_json["output_dir_debug"];
        std::string _encryption_key = pack_config_json["encryption_key"];
        std::string _sub_directory = pack_config_json["output_sub_dir"];

        std::map<std::string, std::string> input_targets = pack_config_json["input_targets"];

        for (auto &kvp : input_targets)
        {
            std::filesystem::path _resource_pack_file_name = kvp.first;
            std::filesystem::path _input_dir = kvp.second;
            std::filesystem::path resource_pack_file_path;

            // Check subdirectory.
            if (!_sub_directory.empty())
            {
                resource_pack_file_path.append(_output_dir + "/" + _sub_directory);
                std::filesystem::create_directories(resource_pack_file_path);
            } else{
                resource_pack_file_path.append(_output_dir);
            }

            // Check extention.
            if (_resource_pack_file_name.has_extension())
            {
                resource_pack_file_path.append(_resource_pack_file_name.generic_string());
            } else{
                resource_pack_file_path.append(_resource_pack_file_name.generic_string() + PackFormat::PACK_EXTENTION);
            }
    
            // Log starting info.
            err::Utils::_log_info("Packing started: " + resource_pack_file_path.stem().generic_string());
            std::vector<Private::PackEntry> pack_entires;
            for (auto &item : std::filesystem::recursive_directory_iterator(_input_dir))
            {
                if (item.is_regular_file())
                {
                    // CHECK IF EXTENSION IS SUPPORTED.
                    if (!rm::Config::is_supported_type(item.path().extension().string()))
                    {
                        _unsupported_files.push_back(item.path().string());
                        continue;
                    }

                    std::filesystem::path _access_path = _input_dir.stem().string() + "/" + item.path().lexically_relative(_input_dir).generic_string(); // Generic string must be used to ensure that the path usses "/" instead of "\\"

                    uint8_t _access_path_size = static_cast<uint8_t>(_access_path.string().size());

                    Private::PackEntry _entry;
                    _entry.file_path =  item.path().string();
                    _entry.entry_name_size = _access_path_size;
                    _entry.entry_name = _access_path.string();
                    
                    
                    if (Private::_parse_binary_data(_entry, _encryption_key, _files_failed_to_open) == err::errFlags::Flags::SUCCESS)
                    {
                        _entry.entry_total_size = PackFormat::ENTRY_CHUNK_FIXED_SIZE + _access_path_size + _entry.data.size();
                        pack_entires.push_back(_entry);
                    }
                }
            }
            Private::_pack_binaries_to_resource_file(pack_entires, resource_pack_file_path, _encryption_key);

            for (auto &item : _files_failed_to_open)
            {
                rm::err::Utils::_log_error(rm::err::ErrorTypes::FILE, "File failed to open at: " + item);
            }

            for (auto &item : _unsupported_files)
            {
                rm::err::Utils::_log_error(rm::err::ErrorTypes::FILE, "Unsuported file type at: " + item);
            }

            std::cout << "FINISHING...\n";
        }
    }
}

namespace rm::PackMaker::Private
{
    inline err::errFlags::Flags _parse_binary_data(Private::PackEntry &pack_entry, std::string &_encryption_key, std::vector<std::string> &_files_failed_to_open)
    {
        err::errFlags::Flags result = err::errFlags::Flags::SUCCESS;

        std::ifstream in(pack_entry.file_path, std::ios::binary | std::ios::ate);

        if (!in.is_open())
        {
            result = err::errFlags::Flags::FAILURE;
            _files_failed_to_open.push_back("Unable to open file at: " + pack_entry.file_path.generic_string());
        }

        if (result == err::errFlags::Flags::SUCCESS)
        {
            size_t size = in.tellg();
            in.seekg(0, std::ios::beg);
            
            pack_entry.data.resize(size);
            
            in.read(pack_entry.data.data(), size);

            if (!_encryption_key.empty())
            {
                size_t key_len = _encryption_key.size();                

                // Meta data is not encrpyted, only the resource data.
                for (size_t i = 0; i < pack_entry.data.size(); ++i)
                {
                    pack_entry.data[i] ^= _encryption_key[i % key_len];
                }
            }

            in.close();
        }

        return result;
    }

    void _pack_binaries_to_resource_file(std::vector<PackEntry> &pack_entries, std::filesystem::path _res_file_path, std::string &_encryption_key)
    {
        if (pack_entries.empty())
        {
            err::Utils::_log_error(err::ErrorTypes::FILE, ": No valid files to pack. Input directory is empty. Failed to create resource pack.");
            
            return;
        }

        std::vector<char> pack_buffer;
        uint32_t entry_count;

        // FILE META DATA
        {
            // Set Encryption flag : 1 byte
            bool encrpyted = true;
            if (_encryption_key.empty()) encrpyted = false;
            pack_buffer.push_back(encrpyted);

            // Entry count: 4 bytes
            // small endian
            entry_count = static_cast<uint32_t>(pack_entries.size());
            pack_buffer.push_back(entry_count & 0xff);
            pack_buffer.push_back((entry_count >> 8) & 0xff);
            pack_buffer.push_back((entry_count >> 16) & 0xff);
            pack_buffer.push_back((entry_count >> 24) & 0xff);
        }
        
        // ENTRY CHUNK
        for (auto &item : pack_entries)
        {
            // Entry total size: 4 bytes
            {
                // small endian
                pack_buffer.push_back(item.entry_total_size & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 8) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 16) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 24) & 0xff);
            }

            // Entry name size: 1 byte
            {
                pack_buffer.push_back(item.entry_name_size);
            }

            // Entry name: N bytes
            {
                pack_buffer.insert(pack_buffer.end(), item.entry_name.begin(), item.entry_name.end());
            }

            // Entry data: N bytes
            {
                pack_buffer.insert(pack_buffer.end(), item.data.begin(), item.data.end())    ;
            }
        }

        pack_buffer.shrink_to_fit();

        err::Utils::_log_success(err::SuccessTypes::FILE, "Packed = " + std::to_string(entry_count) + " files");
        err::Packing::_log_packing(err::errFlags::Flags::SUCCESS, "Resource file path at = " + _res_file_path.string() + '\n');

        // Write to pack file
        
        std::ofstream out(_res_file_path, std::ios::binary | std::ios::out);
        out.write(pack_buffer.data(), pack_buffer.size());
        out.close();
    }
}