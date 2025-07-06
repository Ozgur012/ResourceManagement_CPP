#ifndef RESOURCE_MANAGEMENT_COMMAND_LINE_INTERFACE_H
#define RESOURCE_MANAGEMENT_COMMAND_LINE_INTERFACE_H
#include <string>

namespace rm::CLI
{
    int run(int argc, char* argv[]);
    
    void show_unknown_command(const std::string &command);

    void show_no_command_given();

    // Minimal listing for no-arg fallback
    void show_available_commands();

    // Shows a short help guide (triggered by --help)
    void show_help();

    // Shows full instructions on how to use the packer (default fallback)
    void show_resource_packing_guide();
}

#endif // RESOURCE_MANAGEMENT_COMMAND_LINE_INTERFACE_H
