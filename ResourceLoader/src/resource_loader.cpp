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
        std::string _encryption_key;
        std::string _res_pack_path;
        bool failed_to_open_file = false;
        
    }
    
    void init(std::string res_pack_path, std::string encryption_key)
    {
        Private::_res_pack_path = res_pack_path;
        Private::_encryption_key = encryption_key;
    }

    void open_resource_buffer()
    {

    }
    
    std::vector<char> get_buffer_data(std::string access_path);

    void close_resource_buffer()
    {

    }

}

namespace ResourceManagement::ResourceLoader
{

    void test_data()
    {
        std::ifstream in(Private::_res_pack_path, std::ios::binary);
        
        if (!in.is_open())
        {
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, "Could not load resource file at = " + Private::_res_pack_path);
            Private::failed_to_open_file = true;
            return;
        } else{
            ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "File opened successfully at = " + Private::_res_pack_path);
        }

        std::cout << "RESOURCE LOADER: in.tellg = " << in.tellg() << "\n";
        char char_buffer;
        in.read(&char_buffer, 1);

        std::cout << "RESOURCE LOADER: in.tellg = " << in.tellg() << "\n";


        uint32_t uint_buffer = 0;
        in.read((char*)&uint_buffer, 4);
        

        std::cout << "RESOURCE LOADER: char_buffer = " << static_cast<int>(char_buffer) << "\n";
        std::cout << "RESOURCE LOADER: uint_buffer = " << uint_buffer << "\n";
    }


    std::vector<char> get_raw_resource_data(std::string access_path)
    {
        std::cout << "get_raw_resource_data\n";
        std::vector<char> output_data;
        std::ifstream in(Private::_res_pack_path, std::ios::binary | std::ios::ate);

        if (!in.is_open())
        {
            ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, "Could not load resource file at = " + Private::_res_pack_path);
            Private::failed_to_open_file = true;
            return output_data;    
        } else{
            ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "File opened successfully at = " + Private::_res_pack_path);
        }

        return output_data;
    }


    // std::vector<char> get_raw_resource_data(std::string access_path)
    // {
    //     std::vector<char> output_data;
    //     std::ifstream in(Private::_res_pack_path, std::ios::binary | std::ios::ate);

    //     if (!in.is_open())
    //     {
    //         ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::FILE, "Could not load resource file at = " + Private::_res_pack_path);
    //         Private::failed_to_open_file = true;
    //         return output_data;    
    //     } else{
    //         ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "File opened successfully at = " + Private::_res_pack_path);
    //     }

    //     size_t file_data_size = in.tellg();
    //     in.seekg(0, std::ios::beg);

    //     uint32_t file_index = 0;

    //     std::vector<char> raw_file_data;
    //     raw_file_data.resize(file_data_size);
    //     in.read((char*)&raw_file_data, file_data_size);

    //     // Get encryption flag
    //     char encrpyted;
    //     std::memcpy(&encrpyted, &raw_file_data[file_index], PackFormat::ENCRYPTION_FLAG_SIZE);
    //     file_index += PackFormat::ENCRYPTION_FLAG_SIZE;

    //     if (static_cast<bool>(encrpyted))
    //     {
    //         size_t key_len = Private::_encryption_key.size();

    //         // start from index 1 because the first byte are the encrption flag.
    //         for (size_t i = 1; i < raw_file_data.size(); ++i)
    //         {
    //             raw_file_data[i] ^= Private::_encryption_key[i % key_len];
    //         }
    //     }

    //     uint32_t entry_count = raw_file_data[1];
    //     std::memcpy(&entry_count, &raw_file_data[file_index], PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE);
    //     file_index += PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE;
        
    //     for (size_t i = 0; i < entry_count; i++)
    //     {
    //         // Read name entry size
    //         char entry_name_size = raw_file_data[file_index];
    //         std::memcpy(&entry_name_size, &raw_file_data[file_index], PackFormat::ENTRY_NAME_BYTE_SIZE);
    //         file_index += PackFormat::ENTRY_NAME_BYTE_SIZE;
            
    //         // Read entries total size. Needed in anycase.
    //         uint32_t entry_total_size;
    //         std::memcpy(&entry_total_size, &raw_file_data[file_index], PackFormat::ENTRY_TOTAL_SIZE_BYTE_SIZE);
    //         file_index += PackFormat::ENTRY_TOTAL_SIZE_BYTE_SIZE;

            
    //         // If no match go to the next entry.
    //         if (static_cast<size_t>(entry_name_size) != access_path.size())
    //         {
    //             file_index += entry_total_size;
    //             in.seekg(file_index, std::ios::cur);

    //         } else {
    //             std::string entry_name;
    //             entry_name.resize(entry_name_size);
    //             std::memcpy(&entry_name, &raw_file_data[file_index], sizeof(entry_name));
    //             file_index += sizeof(entry_name);

    //             if (access_path == entry_name)
    //             {
    //                 uint32_t data_size = entry_total_size - (PackFormat::ENTRY_HEADER_FIXED_SIZE + entry_name_size);
    //                 output_data.resize(data_size);
    //                 in.read(output_data.data(), data_size);
    //                 ErrorChecker::Utils::_log_success(ErrorChecker::ErrorTypes::FILE, "Resource found at access path = " + access_path + ": Data size = " + std::to_string(output_data.size()));
                    
    //                 break;
    //             }
    //         }
    //     }

    //     if (output_data.empty())
    //     {
    //         ErrorChecker::Utils::_log_error(ErrorChecker::ErrorTypes::PATH, "No data found for access path = " + access_path);
    //     }

                
    //     if (static_cast<int>(encrpyted) == 1)
    //     {
    //         size_t key_len = Private::_encryption_key.size();

    //         // start from index 1 because the first byte are the encrption flag.
    //         for (size_t i = 1; i < output_data.size(); ++i)
    //         {
    //             output_data[i] ^= Private::_encryption_key[i % key_len];
    //         }
    //     }

    //     in.close();
        
    //     return output_data;
    // }


    void pre_load_all_resources(){}
}

