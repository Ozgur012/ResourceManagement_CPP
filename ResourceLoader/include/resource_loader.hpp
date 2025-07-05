#ifndef RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#define RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#include <vector>
#include <fstream>

// rLoader -> ResourceLoader
namespace rm::rLdr
{
    enum LoadState{NONE, Failed, Begin, Finished};

    struct PackBuffer
    {
        rm::rLdr::LoadState current_load_state = rm::rLdr::LoadState::NONE; // See header for LoadState
        std::string _res_pack_path;
        std::string _encryption_key;
        bool encryption_flag = false;
        std::ifstream pack_file;
        uint32_t entry_count;
        std::vector<uint32_t> consumed_ids;
    };

    PackBuffer create_pack_buffer(std::string res_pack_path, std::string encryption_key = "");

    LoadState open_pack_buffer(PackBuffer &pb);
    
    std::vector<char> get_pack_data(PackBuffer &pb, std::string access_path);

    LoadState close_pack_buffer(PackBuffer &pb);

}

#endif // RESOURCE_MANAGEMENT_RESOURCE_LOADER_H