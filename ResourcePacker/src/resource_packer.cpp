#include <iostream>
#include "environment_validator.hpp"
#include "error_checker.hpp"
#include "command_line_interface.hpp"
#include "pack_maker.hpp"
#include "resource_loader.hpp"

int main(int argc, char* argv[])
{
    rm::err::Utils::_begin_log_buffer();
    if (argc < 2)
    {
        rm::CLI::show_no_command_given();
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
            return -1;
        }

        std::string build_type = is_release ? "Release" : "Debug";
        std::cout << "Packing in " << build_type << " mode.\n";

        if (!rm::Validator::is_valid_environment(config_path))
        {
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
    
    return 0;
}
