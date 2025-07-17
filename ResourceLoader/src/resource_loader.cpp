#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <array>
#include "resource_loader.hpp"
#include "error_checker.hpp"
#include "pack_format.hpp"

namespace rm::rLdr
{
    namespace priv
    {
        std::string name = "RESOURCE_LOADER:";
        uint32_t success_count = 0;
    }
    
    PackBuffer create_pack_buffer(std::string res_pack_path, std::string encryption_key)
    {
        PackBuffer pb;
        pb._res_pack_path = res_pack_path;
        pb._encryption_key = encryption_key;
        return pb;
    }

    LoadState open_pack_buffer(PackBuffer &pb)
    {
        std::cout << '\n' << "==================== RESOURCE LOADER START ====================" << '\n';
        pb.pack_file.exceptions(std::ios::badbit | std::ios::failbit);

        try
        {
            pb.pack_file.open(pb._res_pack_path, std::ios::binary);
        }
        catch(std::fstream::failure &e)
        {
            std::cerr << e.what() << ": Could not load file at path \"" << pb._res_pack_path << "\"" << '\n';
            
            pb.current_load_state = LoadState::Failed;
            return pb.current_load_state;
        }

        // FILE META DATA
        pb.pack_file.read((char*)&pb.encryption_flag, PackFormat::ENCRYPTION_FLAG_SIZE);
        
        // Catch encrytion error
        if (pb.encryption_flag && pb._encryption_key.empty())
        {
            std::cout << priv::name << "This file is encrypted. Please provide a valid encryption key." << '\n';
            
            pb.current_load_state = LoadState::Failed;
            return pb.current_load_state;
        } else{
            std::cout << priv::name << "File is encrypted. Encryption key is present." << '\n';
        }

        pb.pack_file.read((char*)&pb.entry_count, PackFormat::FILE_ENTRY_COUNT_BYTE_SIZE);
        
        std::cout << priv::name << "Buffer open successfully at: " << pb._res_pack_path << '\n';
        std::cout << priv::name << "Ready to extract data..." << '\n';

        pb.current_load_state = LoadState::Begin;

        return pb.current_load_state;
    }
    
    std::vector<char> get_pack_data(PackBuffer &pb, std::string access_path)
    {
        using namespace priv;

        std::vector<char> output_data;
        
        uint32_t file_index;

        if (pb.current_load_state == LoadState::Failed)
        {
            std::cout << priv::name << "Resource extraction aborted.\n"
                    << priv::name << "A previous error (e.g., file open or encryption check failure) has prevented further processing.\n"
                    << priv::name << "Please review earlier log output for details.\n";
            return {};
        }

        // Apply file meta offset
        pb.pack_file.seekg(PackFormat::FILE_META_DATA_SIZE, std::ios::beg);

        //Search for resource
        for (size_t i = 0; i < pb.entry_count; i++)
        {
            uint32_t _end_point = 0; // End point initially set to the entry size. Then as the get pointer moves, the amount moved will be deducted from it.
            
            uint32_t _entry_size = 0;
            uint32_t _entry_name_size = 0;
            std::string _entry_name = "";

            // Read size
            pb.pack_file.read((char*)&_entry_size, PackFormat::ENTRY_TOTAL_SIZE_IN_BYTES);
            _end_point = _entry_size;
            _end_point -= PackFormat::ENTRY_TOTAL_SIZE_IN_BYTES;

            // Read entry name size
            pb.pack_file.read((char*)&_entry_name_size, PackFormat::ENTRY_NAME_SIZE_IN_BYTES);
            _end_point -= PackFormat::ENTRY_NAME_SIZE_IN_BYTES;

            // Name size check
            if (access_path.size() != _entry_name_size)
            {
                pb.pack_file.seekg(_end_point, std::ios::cur);
                continue;
            }
            
            
            // Read Entry name
            _entry_name.resize(_entry_name_size);
            pb.pack_file.read(_entry_name.data(), _entry_name_size);
            _end_point -= _entry_name_size;

            // Name check
            if (access_path != _entry_name)
            {
                pb.pack_file.seekg(_end_point, std::ios::cur);
                
                continue;
            }

            std::cout << priv::name << "VALID access path: " << access_path << '\n';
            // Read data. By this point the decutions made to the _end_point represent the data size.
            priv::success_count++;
            output_data.resize(_end_point);
            pb.pack_file.read(output_data.data(), output_data.size());

            if (pb.encryption_flag == true)
            {
                size_t _key_size = pb._encryption_key.size();
                for (size_t i = 0; i < output_data.size(); i++)
                {
                    output_data[i] ^= pb._encryption_key[i % _key_size];
                }
            }

            break;
        }
        if (output_data.empty())
        {
            std::cout << priv::name << "ERROR: Invalid access path: " << access_path << '\n';
        }
        return output_data;
    }

    LoadState close_pack_buffer(PackBuffer &pb)
    {
        pb.pack_file.close();

        if (pb.current_load_state == LoadState::Failed)
        {
            std::cout << priv::name << "File closed. Process was halted due to earlier errors.\n";
            pb.current_load_state = Failed;   
        }
        else
        {
            std::cout << priv::name << "File closed successfully.\n";
            pb.current_load_state = LoadState::Finished ;
        }

        std::cout << priv::name << "Succesfully loaded: " << std::to_string(priv::success_count) << " file(s).\n";
        std::cout << "=================== RESOURCE LOADER FINISHED ==================" << '\n';
        priv::success_count = 0;
        return pb.current_load_state;
    }
}
