#ifndef RESOURCE_MANAGEMENT_PACK_FORMAT_HPP
#define RESOURCE_MANAGEMENT_PACK_FORMAT_HPP

#include <cstdint>
#include <string>

namespace ResourceManagement::PackFormat
{
    // Default extension used for packed resource files (used if the user provides none).
    inline constexpr const char* PACK_EXTENTION = ".pk";

    inline constexpr uint8_t ENCRYPTION_FLAG_SIZE = 1;
    // Number of bytes at the beginning of the file to store the total entry count (uint32_t).
    inline constexpr uint8_t FILE_ENTRY_COUNT_BYTE_SIZE = 4;

    // Number of bytes used to store the length of an asset name (uint8_t).
    // The name string follows this field, and its length is determined by this value.
    inline constexpr uint8_t ENTRY_NAME_BYTE_SIZE = 1;

    // Number of bytes used to store the total size of an entry (uint32_t).
    // This includes all components: metadata, name length, name, and data.
    inline constexpr uint8_t ENTRY_TOTAL_SIZE_BYTE_SIZE = 4;

    // Combined byte size of fixed metadata at the start of each entry.
    // This does NOT include the name or data bytes — only the header fields.
    inline constexpr uint8_t ENTRY_HEADER_FIXED_SIZE = ENCRYPTION_FLAG_SIZE + ENTRY_NAME_BYTE_SIZE + ENTRY_TOTAL_SIZE_BYTE_SIZE;

    // ENTRY_NAME_BYTE_SIZE: dynamic (N bytes)
    // ENTRY_DATA_BYTE_SIZE: dynamic (M bytes)
}

/*
PACK FILE STRUCTURE                     INDIVIDUAL ENTRY STRUCTURE
=====================                  =============================
+----------------------------+         +----------------------------+
| [4 bytes] Entry Count     |         | [1 byte] Name Length       |
|   (uint32_t)              |         +----------------------------+
+----------------------------+         | [4 bytes] Total Entry Size |
|         Entry 1           |-------->|   (metadata + name + data) |
+----------------------------+         +----------------------------+
|         Entry 2           |         | [N bytes] Entry Name       |
+----------------------------+         |   (length from above)      |
|         ...               |         +----------------------------+
                                      | [M bytes] Entry Data       |
                                      |   (actual file contents)   |
                                      +----------------------------+

LEGEND:
- Entry Count: total number of entries in the file (uint32_t)
- Name Length: number of bytes in the entry name (uint8_t)
- Total Entry Size: total size in bytes of this entry from name length field to end of data (uint32_t)
- Entry Name: N bytes (value from Name Length field)
- Entry Data: M bytes (computed from Total Entry Size - metadata - name)
*/

#endif // RESOURCE_MANAGEMENT_PACK_FORMAT_HPP
