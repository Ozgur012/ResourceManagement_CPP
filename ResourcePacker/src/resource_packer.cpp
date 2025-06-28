#include <iostream>
#include "environment_validator.hpp"
#include "error_checker.hpp"
#include "command_line_interface.hpp"
#include "pack_maker.hpp"

#include "resource_loader.hpp"

#define ENTRY_NAME_LENGTH_FIELD_SIZE 1      // 1 byte to store the length of the asset's name (0–255)
#define ENTRY_NAME_MAX_LENGTH        256    // Maximum number of characters in the asset name
#define ENTRY_OFFSET_FIELD_SIZE      4      // 4 bytes to store the offset of the asset data
#define ENTRY_SIZE_FIELD_SIZE        4      // 4 bytes to store the size of the asset data


int main(int argc, char* argv[])
{
    // No arguments: show "no command" notice + available commands
    if (argc < 2)
    {
        ResourceManagement::CLI::show_no_command_given();
        return 0;
    }

    std::string arg1 = argv[1];

    // --help flag: show usage info
    if (arg1 == "--help")
    {
        ResourceManagement::CLI::show_help();
        return 0;
    }

    // --guide flag: show full how-to
    if (arg1 == "--guide")
    {
        ResourceManagement::CLI::show_resource_packing_guide();
        return 0;
    }

    // --pack flag: validate and pack resources
    if (arg1 == "--pack")
    {
        std::string config_path = (argc >= 3) ? argv[2] : "";

        if (!ResourceManagement::Validator::is_valid_environment(config_path))
        {
            return -1; // Validation failed, logs already flushed
        }

        ResourceManagement::ErrorChecker::Utils::_log_success(ResourceManagement::ErrorChecker::SuccessTypes::VALIDATION, "Validation succeeded. Ready to pack resources..." );
        ResourceManagement::PackMaker::make_resource_pack(config_path);

        
        return 0;
    }

    // Unknown flag
    ResourceManagement::CLI::show_unknown_command(arg1);
    return -1;
}
