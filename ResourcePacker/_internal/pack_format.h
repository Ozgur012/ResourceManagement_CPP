#ifndef RESOURCE_MANAGEMENT_PACK_FORMAT_HPP
#define RESOURCE_MANAGEMENT_PACK_FORMAT_HPP

#include <cstdint>

#include <string>

namespace ResourceManagement::PackStorageFormat
{
    inline constexpr std::string PACK_EXTENTION = ".pk";
    namespace Private
    {
        // Number of bytes used to store the length of an asset name.
        // The actual name will follow, and its length is stored as a uint8_t.
        inline constexpr uint8_t ENTRY_NAME_STORAGE_SIZE = 1;

        // Number of bytes used to store the total size of this entry (metadata + name + data).
        // This is a uint32_t representing the size in bytes.
        inline constexpr uint8_t ENTRY_TOTAL_SIZE_STORAGE_SIZE = 4;
    }

    // Combined size of all fixed-size metadata fields at the beginning of an entry.
    // This does not include the actual name string or the asset data.
    inline constexpr uint8_t ENTRY_HEADER_FIXED_SIZE = Private::ENTRY_NAME_STORAGE_SIZE + Private::ENTRY_TOTAL_SIZE_STORAGE_SIZE;

    // ENTRY_NAME_STORAGE_SIZE: dynamic
    // ENTRY_DATA_STORAGE_SIZE: dynamic
}

#endif // RESOURCE_MANAGEMENT_PACK_FORMAT_HPP



