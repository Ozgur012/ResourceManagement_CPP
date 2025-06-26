#ifndef RESOURCE_MANAGEMENT_PACKAGE_FORMAT_HPP
#define RESOURCE_MANAGEMENT_PACKAGE_FORMAT_HPP

#include <cstdint>

namespace ResourceManagement::PackFormat
{
    // The number of bytes used to store the length of an asset name (uint8_t)
    inline constexpr uint8_t ENTRY_NAME_SIZE = 1;
    // Where the entire entry ends/start of the next entry.
    inline constexpr uint8_t ENTRY_END = 256;

    // EntryName: dynamic

    // EntryData: dynamic

}

// [][][]

#endif // RESOURCE_MANAGEMENT_PACKAGE_FORMAT_HPP
