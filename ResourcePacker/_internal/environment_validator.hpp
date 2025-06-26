#ifndef RESOURCE_MANAGEMENT_ENVIRONMENT_VALIDATOR_H
#define RESOURCE_MANAGEMENT_ENVIRONMENT_VALIDATOR_H

#include <string>

namespace ResourceManagement::Validator
{
    // Validates the environment and configuration before packing resources.
    bool is_valid_environment(const std::string& config_path);
}

#endif // RESOURCE_MANAGEMENT_ENVIRONMENT_VALIDATOR_H
