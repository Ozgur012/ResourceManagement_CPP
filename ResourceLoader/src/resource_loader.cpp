#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "resource_loader.hpp"
#include "error_checker.hpp"
#include "pack_format.hpp"

namespace ResourceManagement::ResourceLoader
{
    namespace Private
    {
        std::string _res_pack_path;
        bool failed_to_open_file = false;
    }
    void init(std::string res_pack_path)
    {
        Private::_res_pack_path = res_pack_path;
    }

    std::vector<char> get_raw_resource_data(std::string access_path)
    {
        std::vector<char> data;
        std::ifstream in(Private::_res_pack_path, std::ios::binary);

        if (!in.is_open())
        {
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, "Could not load resource file at = " + Private::_res_pack_path);
            Private::failed_to_open_file = true;
            return data;    
        } else{
            ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "File opened successfully at = " + Private::_res_pack_path);
        }
        
        uint32_t entry_count;
        in.read((char*)&entry_count, PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE);
        
        
        for (size_t i = 0; i < entry_count; i++)
        {
            
            // Read name entry size
            char entry_name_size;
            in.read(&entry_name_size, PackFormat::ENTRY_NAME_BYTE_SIZE);

            // Read entries total size. Needed in anycase.
            uint32_t entry_total_size;
            in.read((char*)&entry_total_size, PackFormat::ENTRY_TOTAL_SIZE_BYTE_SIZE);

            
            // If no match go to the next entry.
            if (entry_name_size != access_path.size())
            {
                uint32_t next_entry_pos = entry_total_size - PackFormat::ENTRY_HEADER_FIXED_SIZE;
                in.seekg(next_entry_pos, std::ios::cur);
            } else {
                std::string entry_name;
                entry_name.resize(entry_name_size);
                in.read(entry_name.data(), entry_name_size);

                if (access_path == entry_name)
                {
                    uint32_t data_size = entry_total_size - (PackFormat::ENTRY_HEADER_FIXED_SIZE + entry_name_size);
                    data.resize(data_size);
                    in.read(data.data(), data_size);
                    ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "Resource found at access path = " + access_path + ": Data size = " + std::to_string(data.size()));
                    
                    break;
                }
            }
        }

        if (data.empty())
        {
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::PATH, "No data found for access path = " + access_path);
        }

        in.close();
        
        return data;
    }


    void pre_load_all_resources(){}


}