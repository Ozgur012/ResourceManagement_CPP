#ifndef RESOURCE_MANAGEMENT_PACK_MAKER_H
#define RESOURCE_MANAGEMENT_PACK_MAKER_H
#include <string>
#include <vector>
#include <filesystem>
#include "error_checker.hpp"

namespace rm::PackMaker
{
    void make_resource_pack(std::string &pack_config_path, std::string build_type);
}

namespace rm::PackMaker::Private
{
    struct PackEntry
    {
        std::filesystem::path file_path;
        uint32_t entry_id;
        uint8_t entry_name_size;
        uint32_t entry_total_size; // relative to ios::beg
        std::string entry_name;
        std::vector<char> data;
        uint32_t entry_data_size;
    };
    
    inline err::errFlags::Flags _parse_binary_data(Private::PackEntry &pack_entry, std::string &_encryption_key);

    void _pack_binaries_to_resource_file(std::vector<PackEntry> &pack_entries, std::string _res_file_path, std::string &_encryption_key);
}

#endif // RESOURCE_MANAGEMENT_PACK_MAKER_H