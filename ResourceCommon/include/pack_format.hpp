#ifndef RESOURCE_MANAGEMENT_PACK_FORMAT_HPP
#define RESOURCE_MANAGEMENT_PACK_FORMAT_HPP

#include <cstdint>
#include <string>

// rm -> ResourceManagement
namespace rm::PackFormat
{
    // Default extension used for packed resource files (used if the user provides none).
    inline constexpr const char* PACK_EXTENTION = ".pk";
    // ===================== FILE DATA LAYOUT =====================
    
    // Number of bytes that store the encrytion flag (bool)
    inline constexpr uint8_t ENCRYPTION_FLAG_SIZE = 1;

    // Number of bytes at the beginning of the file to store the total entry count (uint32_t).
    inline constexpr uint8_t FILE_ENTRY_COUNT_BYTE_SIZE = 4;

    // ===================== ENTRY DATA LAYOUT =====================
    // Number of bytes used to store the entry's id;
    inline constexpr uint8_t ENTRY_ID_SIZE_IN_BYTES = 4;

    // Number of bytes used to store the total size of an entry (uint32_t).
    // This includes all components: metadata, name length, name, and data.
    inline constexpr uint8_t ENTRY_TOTAL_SIZE_IN_BYTES = 4;

    // Number of bytes used to store the length of an asset name (uint8_t).
    // The name string follows this field, and its length is determined by this value.
    inline constexpr uint8_t ENTRY_NAME_SIZE_IN_BYTES = 1;

    // Combined byte size of fixed metadata at the start of each entry.
    // This does NOT include the name or data bytes — only the header fields. 
    // !IMPORTANT! To get the true size of the entry header the entry access name
    // length must be added to the ENTRY_HEADER_FIXED_SIZE. This is not done here
    // because entry name storage is allowed to be dynamic.
    inline constexpr uint8_t ENTRY_CHUNK_FIXED_SIZE = ENTRY_ID_SIZE_IN_BYTES + ENTRY_TOTAL_SIZE_IN_BYTES + ENTRY_NAME_SIZE_IN_BYTES;

    inline constexpr uint8_t FILE_META_DATA_SIZE = ENCRYPTION_FLAG_SIZE + FILE_ENTRY_COUNT_BYTE_SIZE;

    // Off set of where to start encrption from.
    

    // ENTRY_NAME_BYTE_SIZE: dynamic (N bytes)
    // ENTRY_DATA_BYTE_SIZE: dynamic (N bytes)
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
