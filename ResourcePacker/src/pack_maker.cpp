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
        std::ifstream _json_file(pack_config_path);
        nlohmann::json pack_config_json = nlohmann::json::parse(_json_file);

        
        std::string _output_dir = (build_type == "Release") ? pack_config_json["output_dir_release"] : pack_config_json["output_dir_debug"];
        std::string _encryption_key = pack_config_json["encryption_key"];
        std::cout << "_encryption_key: " << _encryption_key << "\n";

        std::map<std::string, std::string> input_targets = pack_config_json["input_targets"];

        for (auto &kvp : input_targets)
        {
            std::filesystem::path _resource_pack_file_name = kvp.first;
            std::filesystem::path _input_dir = kvp.second;
            std::string resource_pack_file_path;

            if (_resource_pack_file_name.has_extension())
            {
                resource_pack_file_path =  _output_dir + "/" + _resource_pack_file_name.generic_string();
            } else{
                resource_pack_file_path =  _output_dir + "/" + _resource_pack_file_name.generic_string() + PackFormat::PACK_EXTENTION;
            }
            
            std::vector<Private::PackEntry> pack_entires;
            for (auto &item : std::filesystem::recursive_directory_iterator(_input_dir))
            {
                if (item.is_regular_file())
                {
                    // CHECK IF EXTENSION IS SUPPORTED.
                    if (!rm::Config::is_supported_type(item.path().extension().string()))
                    {
                        continue;
                    }

                    std::filesystem::path _access_path = _input_dir.stem().string() + "/" + item.path().lexically_relative(_input_dir).generic_string(); // Generic string must be used to ensure that the path usses "/" instead of "\\"
                    std::cout << "_access_path = " << _access_path << '\n';

                    uint8_t _access_path_size = static_cast<uint8_t>(_access_path.string().size());

                    Private::PackEntry _entry;
                    _entry.file_path =  item.path().string();
                    _entry.entry_name_size = _access_path_size;
                    _entry.entry_name = _access_path.string();
                    
                    
                    if (Private::_parse_binary_data(_entry, _encryption_key) == err::errFlags::Flags::SUCCESS)
                    {
                        _entry.entry_total_size = PackFormat::ENTRY_CHUNK_FIXED_SIZE + _access_path_size + _entry.data.size();
                        pack_entires.push_back(_entry);
                    }
                }
            }
            Private::_pack_binaries_to_resource_file(pack_entires, resource_pack_file_path, _encryption_key);
        }
    }
}

namespace rm::PackMaker::Private
{
    inline err::errFlags::Flags _parse_binary_data(Private::PackEntry &pack_entry, std::string &_encryption_key)
    {
        err::errFlags::Flags result = err::errFlags::Flags::SUCCESS;

        std::ifstream in(pack_entry.file_path, std::ios::binary | std::ios::ate);

        if (!in.is_open())
        {
            result = err::errFlags::Flags::FAILURE;
        
            err::Packing::_log_packing(err::errFlags::Flags::FAILURE, " Unable to open file at: " + pack_entry.file_path.generic_string());
        } 
        else{
            err::Packing::_log_packing(err::errFlags::Flags::SUCCESS, " File Opened at: " + pack_entry.file_path.generic_string());
        }

        if (result == err::errFlags::Flags::SUCCESS)
        {
            size_t size = in.tellg();
            in.seekg(0, std::ios::beg);
            
            pack_entry.data.resize(size);
            
            in.read(pack_entry.data.data(), size);

            if (!_encryption_key.empty())
            {
                err::Utils::_log_success(err::SuccessTypes::FILE, "Applying encryption...");
                size_t key_len = _encryption_key.size();                

                // Meta data is not encrpyted, only the resource data.
                for (size_t i = 0; i < pack_entry.data.size(); ++i)
                {
                    pack_entry.data[i] ^= _encryption_key[i % key_len];
                }
                err::Utils::_log_success(err::SuccessTypes::FILE, "Encryption Complete.");
            }

            in.close();
        }

        return result;
    }

    void _pack_binaries_to_resource_file(std::vector<PackEntry> &pack_entries, std::string _res_file_path, std::string &_encryption_key)
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
            std::cout << "\nRESOURCE PACKER : adding encrpyted flag = " << std::to_string(encrpyted) << "\n";
            pack_buffer.push_back(encrpyted);
            std::cout << "\nRESOURCE PACKER : buffer size = " << pack_buffer.size() << "\n";

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
            err::Packing::_log_packing(err::errFlags::Flags::SUCCESS, "Resource packed with access name: " + item.entry_name);
        }

        pack_buffer.shrink_to_fit();




        err::Packing::_log_packing(err::errFlags::Flags::SUCCESS, "Resource file path at = " + _res_file_path);
        err::Utils::_log_success(err::SuccessTypes::FILE, "Packed = " + std::to_string(entry_count) + " files");

        // Write to pack file
        
        std::ofstream out(_res_file_path, std::ios::binary | std::ios::out);
        out.write(pack_buffer.data(), pack_buffer.size());
        out.close();

        std::cout << "FINISHING...\n";
    }
}