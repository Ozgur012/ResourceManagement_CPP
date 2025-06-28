#ifndef RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#define RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#include <vector>

namespace ResourceManagement::ResourceLoader
{
    
    void init(std::string res_pack_path);
    std::vector<char> get_resource_data(std::string &access_path);
    std::vector<char> load_test_resource();
    void pre_load_all_resources();
}

#endif // RESOURCE_MANAGEMENT_RESOURCE_LOADER_H