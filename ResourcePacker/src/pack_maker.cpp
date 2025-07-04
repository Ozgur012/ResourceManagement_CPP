#include "pack_maker.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
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
        std::filesystem::path _input_dir = pack_config_json["input_dir"];
        std::string _output_dir = (build_type == "Release") ? pack_config_json["output_dir_release"] : pack_config_json["output_dir_debug"];
        std::string _encryption_key = pack_config_json["encryption_key"];
        std::cout << "_encryption_key: " << _encryption_key << "\n";

        std::filesystem::path _resouce_pack_file_name = pack_config_json["resource_pack_file_name"];


        std::string resource_pack_file_path;
        if (_resouce_pack_file_name.has_extension())
        {
            resource_pack_file_path =  _output_dir + "/" + _resouce_pack_file_name.generic_string();
        } else{
            resource_pack_file_path =  _output_dir + "/" + _resouce_pack_file_name.generic_string() + PackFormat::PACK_EXTENTION;
        }
        
        std::vector<Private::PackEntry> pack_entires;
        uint32_t _id = 0;
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

                uint8_t _access_path_size = static_cast<uint8_t>(_access_path.string().size());

                Private::PackEntry _entry;
                _entry.entry_id = _id++;
                _entry.entry_name = _access_path.string();
                _entry.entry_name_size = _access_path_size;
                _entry.file_path =  item.path().string();
                
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

                // Offset for file metadata. Name is dynamic so its size is added where needed as it is here.
                size_t _meta_data_offset = PackFormat::ENTRY_CHUNK_FIXED_SIZE + pack_entry.entry_name_size;

                for (size_t i = _meta_data_offset; i < pack_entry.data.size(); ++i)
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
        
        // ENTRY
        for (auto &item : pack_entries)
        {
            // entry_id
            {
                // small endian
                pack_buffer.push_back(item.entry_id & 0xff);
                pack_buffer.push_back((item.entry_id >> 8) & 0xff);
                pack_buffer.push_back((item.entry_id >> 16) & 0xff);
                pack_buffer.push_back((item.entry_id >> 24) & 0xff);
            }

            // entry_total_size: 4 bytes
            {
                // small endian
                pack_buffer.push_back(item.entry_total_size & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 8) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 16) & 0xff);
                pack_buffer.push_back((item.entry_total_size >> 24) & 0xff);
            }

            // entry_name_size: 1 byte
            {
                pack_buffer.push_back(item.entry_name_size);
            }

            // entry_name: Dynamic
            {
                pack_buffer.insert(pack_buffer.end(), item.entry_name.begin(), item.entry_name.end());
            }

            // entry_data.size(): 

            //buffer: Dynamic
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