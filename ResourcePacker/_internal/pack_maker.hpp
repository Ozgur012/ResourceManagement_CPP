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
        uint8_t entry_name_size;
        uint32_t entry_total_size; // relative to ios::beg
        std::string entry_name;
        std::vector<char> buffer;
    };
    
    ErrorChecker::ErrorFlags::Flags _extract_binary_data(std::vector<char> &to, std::filesystem::path file_path);

    void _pack_binaries_to_resource_file(std::vector<PackEntry> &pack_entries, std::string _res_file_path);
}

#endif // RESOURCE_MANAGEMENT_PACK_MAKER_H