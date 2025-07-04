#ifndef RESOURCE_MANAGEMENT_RM_CONFIG_H
#define RESOURCE_MANAGEMENT_RM_CONFIG_H
#include <vector>
#include <string>

// rm -> ResourceManagement
namespace rm::Config
{
    extern std::vector<std::string> supported_file_types;
    bool is_supported_type(std::string extention);
    void add_file_type(std::string new_type);
}

#endif // RESOURCE_MANAGEMENT_RM_CONFIG_H