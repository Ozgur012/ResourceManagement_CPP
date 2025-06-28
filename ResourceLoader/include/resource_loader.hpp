#ifndef RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#define RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#include <vector>

namespace ResourceManagement::ResourceLoader
{
    // namespace Private
    // {
    //     std::string _res_pack_path;
    // }
    void init(std::string res_pack_path);
    std::vector<char> get_image_data(std::string &access_path);
    std::vector<char> load_test_resource();
    void pre_load_all_resources();
}

#endif // RESOURCE_MANAGEMENT_RESOURCE_LOADER_H