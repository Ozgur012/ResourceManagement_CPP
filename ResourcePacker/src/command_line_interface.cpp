#include "command_line_interface.hpp"
#include <iostream>
#include "environment_validator.hpp"
#include "error_checker.hpp"
#include "command_line_interface.hpp"
#include "pack_maker.hpp"
#include "resource_loader.hpp"


namespace rm::CLI
{
    int run(int argc, char* argv[])
    {
        err::Utils::_begin_log_buffer();
        
        if (argc < 2)
        {
            CLI::show_no_command_given();
            return 0;
        }

        bool do_pack = false;
        bool is_release = false;
        bool is_debug = false;
        std::string config_path;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            

            if (arg == "--help")
            {
                rm::CLI::show_help();
                return 0;
            }
            else if (arg == "--guide")
            {
                rm::CLI::show_resource_packing_guide();
                return 0;
            }
            else if (arg == "--pack")
            {
                do_pack = true;
            }
            else if (arg == "--release")
            {
                is_release = true;
            }
            else if (arg == "--debug")
            {
                is_debug = true;
            }
            else if (arg.size() >= 5 && arg.substr(arg.size() - 5) == ".json")
            {
                config_path = arg;
            }
            else
            {
                rm::CLI::show_unknown_command(arg);
                return -1;
            }
        }

        if (is_release && is_debug)
        {
            rm::err::Utils::_log_error(
                rm::err::ErrorTypes::CONFIG,
                "Cannot use both --release and --debug flags at the same time."
            );
            rm::err::Utils::_end_log_bugger();
            return -1;
        }

        if (do_pack)
        {
            if (config_path.empty())
            {
                rm::err::Utils::_log_error(
                    rm::err::ErrorTypes::CONFIG,
                    "No configuration file provided for --pack."
                );
                rm::err::Utils::_end_log_bugger();
                return -1;
            }

            std::string build_type = is_release ? "Release" : "Debug";
            std::cout << "Packing in " << build_type << " mode.\n";

            if (!rm::Validator::is_valid_environment(config_path))
            {
                rm::err::Utils::_log_error(rm::err::ErrorTypes::VALIDATION, "PACKING ABORTED: Validation failed. Check config file at: " + config_path);
                rm::err::Utils::_end_log_bugger();
                return -1;
            }

            rm::err::Utils::_log_success(
                rm::err::SuccessTypes::VALIDATION,
                "Validation succeeded. Ready to pack resources..."
            );

            rm::PackMaker::make_resource_pack(config_path, build_type);
            rm::err::Utils::_end_log_bugger();
            return 0;
        }

        rm::CLI::show_no_command_given();
        rm::err::Utils::_end_log_bugger();

        return -1;
    }
}
    

namespace rm::CLI
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
            "  Target Debug Directory:\n"
            "  ResourcePacker.exe --pack <path_to_json_config>\n\n"

            "  Target Release Directory:\n"
            "  ResourcePacker.exe --pack --release <path_to_json_config>\n\n"

            "Examples:\n"
            "  ResourcePacker.exe --pack C:\\Projects\\MyGame\\pack_config.json\n"
            "  ResourcePacker.exe --pack C:/Projects/MyGame/pack_config.json\n"
            "  .\\ResourcePacker.exe --pack C:/Projects/MyGame/pack_config.json\n\n"

            "NOTES:\n"
            "------\n"
            " - The JSON file is only required during the packing process.\n"
            " - The encryption key is required at runtime if resources are encrypted.\n"
            " - You do not need to ship the JSON file with your final build.\n"
            " - The encryption key must be registered in code. Refer to the ResourceLoader docs.\n"
            " - If --release is not specified, ResourcePacker defaults to Debug mode.\n\n";
    }

}