#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <array>
#include "resource_loader.hpp"
#include "error_checker.hpp"
#include "pack_format.hpp"


namespace rm::rLoader
{
    namespace priv
    {
        std::string name = "RESOURCE_LOADER:";

        std::string _res_pack_path;
        std::string _encryption_key;

        bool continue_process = true;
        bool encryption_flag = false;
        bool failed_to_open_file = false;

        std::ifstream pack_file;

        uint32_t entry_count;

        std::vector<uint32_t> consumed_ids;
    }
    
    void init(std::string res_pack_path, std::string encryption_key)
    {
        priv::_res_pack_path = res_pack_path;
        priv::_encryption_key = encryption_key;

        std::cout << priv::name << "priv::_encryption_key = " << priv::_encryption_key << '\n';
    }

    void open_resource_buffer()
    {
        priv::pack_file.exceptions(std::ios::badbit | std::ios::failbit);

        try
        {
            priv::pack_file.open(priv::_res_pack_path, std::ios::binary);
        }
        catch(std::fstream::failure &e)
        {
            std::cerr << e.what() << ": Could not load file at path \"" << priv::_res_pack_path << "\"" << '\n';
            priv::continue_process = false;
            return;
        }

        // FILE META DATA
        priv::pack_file.read((char*)&priv::encryption_flag, PackFormat::ENCRYPTION_FLAG_SIZE);
        
        // Catch encrytion error
        if (priv::encryption_flag && priv::_encryption_key.empty())
        {
            std::cout << priv::name << "This file is encrypted. Please provide a valid encryption key." << '\n';
            priv::continue_process = false;
            return;
        } else{
            std::cout << priv::name << "File is encrypted. Encryption key is present." << '\n';
        }

        priv::pack_file.read((char*)&priv::entry_count, PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE);
        
        
        std::cout << '\n';
        std::cout << priv::name << "Buffer open successfully. Ready to extract data..." << '\n';
        std::cout << priv::name << "Call get_buffered_resource_data() to extract individual resources." << '\n';
        std::cout << priv::name << "Once all required data is extracted must call close_resource_buffer()." << '\n';
        std::cout << '\n';
    }
    
    std::vector<char> get_buffered_resource_data(std::string access_path)
    {
        using namespace priv;

        std::vector<char> output_data;
        
        uint32_t file_index;

        if (!priv::continue_process)
        {
            std::cout << priv::name << "Resource extraction aborted.\n"
                    << priv::name << "A previous error (e.g., file open or encryption check failure) has prevented further processing.\n"
                    << priv::name << "Please review earlier log output for details.\n";
            return {};
        }

        // Apply file meta offset
        pack_file.seekg(PackFormat::FILE_META_DATA_SIZE, std::ios::beg);

        //Search for resource
        for (size_t i = 0; i < entry_count; i++)
        {
            uint32_t _id;
            uint32_t _entry_size;
            uint8_t _entry_name_size;
            std::string _entry_name;

            // Read id
            pack_file.read((char*)&_id, PackFormat::ENTRY_ID_SIZE_IN_BYTES);
            std::cout << "res id = " << _id << '\n';

            // Read size
            pack_file.read((char*)&_entry_size, PackFormat::ENTRY_TOTAL_SIZE_IN_BYTES);
            std::cout << "entry size = " << _entry_size << '\n';

            // Read entry name size
            pack_file.read((char*)&_entry_name_size, PackFormat::ENTRY_NAME_SIZE_IN_BYTES);

            // Read Entry name
            
            pack_file.read(_entry_name.data(), _entry_name_size - 1);


            std::cout << _entry_name << '\n';

            if (encryption_flag == true)
            {
                std::cout << "file encrypted = " << access_path << '\n';
            }
        }
        

        return output_data;
    }

    void close_resource_buffer()
    {
        priv::pack_file.close();

        if (priv::continue_process)
        {
            std::cout << priv::name << "File closed successfully.\n";
        }
        else
        {
            std::cout << priv::name << "File closed. Process was halted due to earlier errors.\n";
        }
    }

}
