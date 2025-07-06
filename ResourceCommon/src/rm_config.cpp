#include "rm_config.hpp"
#include <iostream>

namespace rm::Config
{
    std::vector<std::string> supported_file_types = {".png", ".jpg", ".jpeg", ".ogg", ".wav", ".mp3"};

    bool is_supported_type(std::string extention)
    {
        bool result = false;

        for (auto &sprted_type: supported_file_types)
        {
            if (extention == sprted_type)
            {
                result = true;
                //std::cout << "\nrm::Config : file extention = " << extention << " : is supported\n";
            }
        }

        // if (!result)
        // {
        //     std::cout << "\nrm::Config : file extention = " << extention << " : is NOT supported\n";
        // }

        return result;
    }

    void add_file_type(std::string new_type)
    {
        supported_file_types.push_back(new_type);
    }
}