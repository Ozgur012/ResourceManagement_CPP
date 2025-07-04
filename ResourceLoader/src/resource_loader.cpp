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
        LoadState current_load_state = LoadState::NONE; // See header for LoadState

        std::string name = "RESOURCE_LOADER:";
        std::string _res_pack_path;
        std::string _encryption_key;

        bool encryption_flag = false;

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

    LoadState open_resource_buffer()
    {
        priv::pack_file.exceptions(std::ios::badbit | std::ios::failbit);

        try
        {
            priv::pack_file.open(priv::_res_pack_path, std::ios::binary);
        }
        catch(std::fstream::failure &e)
        {
            std::cerr << e.what() << ": Could not load file at path \"" << priv::_res_pack_path << "\"" << '\n';
            
            priv::current_load_state = LoadState::Failed;
            return priv::current_load_state;
        }

        // FILE META DATA
        priv::pack_file.read((char*)&priv::encryption_flag, PackFormat::ENCRYPTION_FLAG_SIZE);
        
        // Catch encrytion error
        if (priv::encryption_flag && priv::_encryption_key.empty())
        {
            std::cout << priv::name << "This file is encrypted. Please provide a valid encryption key." << '\n';
            
            priv::current_load_state = LoadState::Failed;
            return priv::current_load_state;
        } else{
            std::cout << priv::name << "File is encrypted. Encryption key is present." << '\n';
        }

        priv::pack_file.read((char*)&priv::entry_count, PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE);
        
        std::cout << '\n';
        std::cout << priv::name << "Buffer open successfully. Ready to extract data..." << '\n';
        std::cout << priv::name << "Call get_buffered_resource_data() to extract individual resources." << '\n';
        std::cout << priv::name << "Once all required data is extracted must call close_resource_buffer()." << '\n';
        std::cout << '\n';

        priv::current_load_state = LoadState::Begin;

        return priv::current_load_state;
    }
    
    std::vector<char> get_buffered_data(std::string access_path)
    {
        using namespace priv;

        std::vector<char> output_data;
        
        uint32_t file_index;

        if (priv::current_load_state == LoadState::Failed)
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
            uint32_t _end_point = 0; // End point initially set to the entry size. Then as the get pointer moves, the amount moved will be deducted from it.
            
            uint32_t _entry_size = 0;
            uint32_t _entry_name_size = 0;
            std::string _entry_name = "";

            // Read size
            pack_file.read((char*)&_entry_size, PackFormat::ENTRY_TOTAL_SIZE_IN_BYTES);
            _end_point = _entry_size;
            _end_point -= PackFormat::ENTRY_TOTAL_SIZE_IN_BYTES;
            std::cout << "entry size = " << _entry_size << '\n';

            // Read entry name size
            pack_file.read((char*)&_entry_name_size, PackFormat::ENTRY_NAME_SIZE_IN_BYTES);
            std::cout << "_entry_name_size = " << _entry_name_size << '\n';
            _end_point -= PackFormat::ENTRY_NAME_SIZE_IN_BYTES;

            // Name size check
            if (access_path.size() != _entry_name_size)
            {
                pack_file.seekg(_end_point, std::ios::cur);
                std::cout << "access_path size invalid.\n";
                continue;
            }
            
            
            // Read Entry name
            _entry_name.resize(_entry_name_size);
            pack_file.read(_entry_name.data(), _entry_name_size);
            _end_point -= _entry_name_size;
            std::cout << "_entry_name = " << _entry_name.data() << '\n';

            // Name check
            if (access_path != _entry_name)
            {
                pack_file.seekg(_end_point, std::ios::cur);
                std::cout << "access_path invalid.\n";
                continue;
            }
            
            std::cout << "_end_point final = " << _end_point << '\n';
            std::cout << "total entry meta data size = " << _entry_size - _end_point << '\n';

            // Read data. By this point the decutions made to the _end_point represent the data size.
            output_data.resize(_end_point);
            pack_file.read(output_data.data(), output_data.size());

            if (encryption_flag == true)
            {
                size_t _key_size = _encryption_key.size();
                for (size_t i = 0; i < output_data.size(); i++)
                {
                    output_data[i] ^= _encryption_key[i % _key_size];
                }
                std::cout << name << "decryption complete\n";
            }
            break;
        }
        return output_data;
    }

    LoadState close_resource_buffer()
    {
        priv::pack_file.close();

        if (priv::current_load_state == LoadState::Failed)
        {
            std::cout << priv::name << "File closed. Process was halted due to earlier errors.\n";
            priv::current_load_state = Failed;   
        }
        else
        {
            std::cout << priv::name << "File closed successfully.\n";
            priv::current_load_state = LoadState::Finished ;
        }

        return priv::current_load_state;
    }
}
