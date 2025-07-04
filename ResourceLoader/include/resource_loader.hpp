#ifndef RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#define RESOURCE_MANAGEMENT_RESOURCE_LOADER_H
#include <vector>

// rLoader -> ResourceLoader
namespace rm::rLoader
{
    void init(std::string res_pack_path, std::string encryption_key = "");

    void open_resource_buffer();
    
    std::vector<char> get_buffered_resource_data(std::string access_path);

    void close_resource_buffer();

}

#endif // RESOURCE_MANAGEMENT_RESOURCE_LOADER_H