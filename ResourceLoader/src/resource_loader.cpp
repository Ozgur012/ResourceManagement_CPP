#include "resource_loader.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "raylib.h"

namespace ResourceManagement::ResourceLoader
{
    namespace Private
    {
        std::string _res_pack_path;
    }
    void init(std::string res_pack_path)
    {
        Private::_res_pack_path = res_pack_path;
    }

    std::vector<char> get_resource_data(std::string &access_path)
    {
        std::vector<char> data;
        std::ifstream in(Private::_res_pack_path, std::ios::binary);

        if (!in.is_open())
        {
            std::cout << "ResourceLoader: Failed to open resource file = " << Private::_res_pack_path << "\n";
        } else{
            std::cout << "ResourceLoader: Resource file open successfully at = " << Private::_res_pack_path << "\n";
        }

        uint32_t entry_count;
        in.read((char*)entry_count, 4);
        
        for (size_t i = 0; i < entry_count; i++)
        {
            uint16_t entry_name_size;
            in.read((char*)entry_name_size, 1);

            if (entry_name_size != access_path.size()) continue;

            // Search and find the asset.
            // Change the fle format for the entries, [entry name size][access path][total size][data]
        }
        


        in.close();
        return data;
    }


    void pre_load_all_resources(){}

    std::vector<char> load_test_resource()
    {
        
        // std::string resource_loader_name = "RESOURCE LOADER";
        std::vector<char> main_buffer;

        // std::cout << resource_loader_name << "\n";
        
        // std::filesystem::path pack_path = "F:/Projects/Cpp/ResourceManagement_cxx/TestProject/Debug/resources.pk";
        // std::ifstream in(pack_path, std::ios::binary);

        // if (!in.is_open())
        // {
        //     std::cout << resource_loader_name << ": Failed to open file. "<< "\n";
        // } else{
        //     std::cout << resource_loader_name << ": File Opened successfully. "<< "\n";
        // }

        // std::cout << resource_loader_name << ": tellg = " << in.tellg() << "\n";
        // uint16_t name_size;
        // in.read((char*)&name_size, 1);

        // std::cout << resource_loader_name << ": load_test_resource: name_size = " << name_size << "\n";
        // std::cout << resource_loader_name << ": tellg = " << in.tellg() << "\n";

        // uint32_t total_size;
        // in.read((char*)&total_size, 4);

        // std::cout << resource_loader_name << ": load_test_resource: total_size = " << total_size << "\n";
        // std::cout << resource_loader_name << ": tellg = " << in.tellg() << "\n";

        
        // std::vector<char> access_name_buffer;
        // access_name_buffer.resize(name_size);
        // in.read(access_name_buffer.data(), name_size);

        // std::string access_name = reinterpret_cast<char*>(access_name_buffer.data());

        // std::cout << resource_loader_name << ": load_test_resource: access_name = " << access_name << "\n";
        // std::cout << resource_loader_name << ": tellg = " << in.tellg() << "\n";

        
        // uint32_t data_size = total_size - name_size - 4 - 1;
        // std::vector<unsigned char> image_buffer;
        // image_buffer.resize(data_size);

        // in.read((char*)image_buffer.data(), data_size);

        // in.close();

        
        
        // InitWindow(600, 600, "");
        // Image img = LoadImageFromMemory(".png", (unsigned char*)image_buffer.data(), data_size);
        // Texture2D texture = LoadTextureFromImage(img);
        // UnloadImage(img);

        // while (!WindowShouldClose())
        // {
        //     BeginDrawing();
        //     ClearBackground(DARKBLUE);
        //     DrawTexture(texture, 0, 0, WHITE);

        //     EndDrawing();
        // }

        // UnloadTexture(texture);
        // CloseWindow();
        

        return main_buffer;
    }
}