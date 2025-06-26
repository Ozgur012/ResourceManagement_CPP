#ifndef RESOURCE_MANAGEMENT_PACK_MAKER_H
#define RESOURCE_MANAGEMENT_PACK_MAKER_H
#include <string>
#include <vector>
#include <filesystem>
#include "error_checker.hpp"

namespace ResourceManagement::PackMaker
{
    void make_resource_pack(std::string &pack_config_path);
}

namespace ResourceManagement::PackMaker::Private
{
    struct PackEntry
    {
        std::string entry_name;
        uint32_t entry_name_size;
        uint32_t entry_end;
    };
    
    ErrorChecker::ErrorFlags::Flags _bake_binary_data(std::vector<char> &to, std::filesystem::path file_path);
    
}

#endif // RESOURCE_MANAGEMENT_PACK_MAKER_H