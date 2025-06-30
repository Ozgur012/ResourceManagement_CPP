#include <iostream>
#include "environment_validator.hpp"
#include "error_checker.hpp"
#include "command_line_interface.hpp"
#include "pack_maker.hpp"
#include "resource_loader.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        ResourceManagement::CLI::show_no_command_given();
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
            ResourceManagement::CLI::show_help();
            return 0;
        }
        else if (arg == "--guide")
        {
            ResourceManagement::CLI::show_resource_packing_guide();
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
        else if (arg.ends_with(".json"))
        {
            config_path = arg;
        }
        else
        {
            ResourceManagement::CLI::show_unknown_command(arg);
            ResourceManagement::ErrorChecker::Utils::_flush_logs();
            return -1;
        }
    }

    if (is_release && is_debug)
    {
        ResourceManagement::ErrorChecker::Utils::_log_error(
            ResourceManagement::ErrorChecker::ErrorTypes::CONFIG,
            "Cannot use both --release and --debug flags at the same time."
        );
        ResourceManagement::ErrorChecker::Utils::_flush_logs();
        return -1;
    }

    if (do_pack)
    {
        if (config_path.empty())
        {
            ResourceManagement::ErrorChecker::Utils::_log_error(
                ResourceManagement::ErrorChecker::ErrorTypes::CONFIG,
                "No configuration file provided for --pack."
            );
            ResourceManagement::ErrorChecker::Utils::_flush_logs();
            return -1;
        }

        std::string build_type = is_release ? "Release" : "Debug";
        std::cout << "Packing in " << build_type << " mode.\n";

        if (!ResourceManagement::Validator::is_valid_environment(config_path))
        {
            ResourceManagement::ErrorChecker::Utils::_flush_logs();
            return -1;
        }

        ResourceManagement::ErrorChecker::Utils::_log_success(
            ResourceManagement::ErrorChecker::SuccessTypes::VALIDATION,
            "Validation succeeded. Ready to pack resources..."
        );

        ResourceManagement::PackMaker::make_resource_pack(config_path, build_type);
        return 0;
    }

    ResourceManagement::CLI::show_no_command_given();
    return 0;
}
