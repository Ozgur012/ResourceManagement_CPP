#include "command_line_interface.hpp"
#include <iostream>


namespace ResourceManagement::CLI
{
    void show_unknown_command(const std::string& command)
    {
        std::cout << "\n[ Unknown Command ]\n"
                << "---------------------\n"
                << "Unrecognized command: " << command << "\n\n";
        show_available_commands();
    }


    void show_no_command_given()
    {
        std::cout << "\n[ Command Summary ]\n"
                << "--------------------\n"
                << "No command given.\n\n";
        show_available_commands();
    }

    void show_available_commands()
    {
        std::cout <<
            "Available commands:\n"
            "  --help\n"
            "  --guide\n"
            "  --pack <path_to_json_config>\n\n";
    }


    void show_help()
    {
        std::cout <<
            "\n[ Help - Command Usage ]\n"
            "--------------------------\n\n"

            "Description:\n"
            "  --help  : Displays this help guide with usage instructions.\n"
            "  --guide : Shows a full guide on how to create and use the JSON configuration.\n"
            "  --pack  : Validates and processes a resource pack config file, generating a\n"
            "            binary pack file for use at runtime.\n\n"

            "Usage:\n"
            "  ResourcePacker.exe --help\n"
            "  ResourcePacker.exe --guide\n"
            "  ResourcePacker.exe --pack <path_to_json_config>\n\n";
    }


    void show_resource_packing_guide()
    {
        std::cout <<
            "\n[ Guide - How to Pack Resources ]\n"
            "----------------------------------\n\n"

            "STEP 1: Create the Configuration File\n"
            "-------------------------------------\n"
            "To use the Resource Packer, create a JSON configuration file.\n"
            "It is recommended (but not required) to name it:\n"
            "   <project_name>_pack_config.json\n" 
            "   or\n" 
            "   pack_config.json\n\n"


            "and keep it in your project directory (recommended).\n\n"

            "STEP 2: Define Required Keys\n"
            "-----------------------------\n"
            "The JSON file must contain the following keys:\n"
            "    - input_dir\n"
            "    - output_dir_debug\n"
            "    - output_dir_release\n"
            "    - output_sub_dir          // optional: use \"\" if unused\n"
            "    - resource_pack_file_name\n"
            "    - encryption_key          // optional: use \"\" if unused\n\n"

            "STEP 3: Example JSON Structure\n"
            "------------------------------\n"
            "{\n"
            "    \"input_dir\": \"C:/MyProjects/MyGame/Assets\",\n"
            "    \"output_dir_debug\": \"C:/MyProjects/MyGame/build/Debug\",\n"
            "    \"output_dir_release\": \"C:/MyProjects/MyGame/build/Release\",\n"
            "    \"output_sub_dir\": \"MyGameData/PackedResources\", // optional\n"
            "    \"resource_pack_file_name\": \"mygame_resc_pack\",\n"
            "    \"encryption_key\": \"MySecret_encryption_key123\"\n"
            "}\n\n"

            "STEP 4: Run the ResourcePacker\n"
            "-------------------------------\n"
            "Use the following command to validate and pack resources:\n\n"
            "  ResourcePacker.exe --pack <path_to_json_config>\n\n"

            "Examples:\n"
            "  ResourcePacker.exe --pack C:\\Projects\\MyGame\\pack_config.json\n"
            "  ResourcePacker.exe --pack C:/Projects/MyGame/pack_config.json\n"
            "  .\\ResourcePacker.exe --pack C:/Projects/MyGame/pack_config.json\n\n"

            "NOTES:\n"
            "------\n"
            " - The JSON file is only required during the packing process.\n"
            " - The encryption key is required at runtime if resources are encrypted.\n"
            " - You do not need to ship the JSON file with your final build.\n"
            " - The encryption key must be registered in code. Refer to the ResourceLoader docs.\n";
    }

}