# ResourceManagement_CPP

## Welcome
Resource Management C++ is a lightweight, modular system for packing and loading game assets in binary form. Designed for simplicity, portability, and performance, it provides a CLI-based resource packer and a runtime loader library to streamline asset distribution and access in your C++ game or application.

## How It Works
The system is split into two components:

Resource Packer (CLI tool): Reads a JSON config file, collects files from multiple target folders, optionally encrypts them, and produces one or more binary .pk pack files.

Resource Loader (Library): Loads any of the generated .pk files at runtime, decrypts them if needed, and gives direct access to individual assets via simple API calls.

This lets you manage and load multiple independent resource packs as needed, giving you flexibility over how assets are bundled and accessed.

### Example Json config file.

    {
        "input_targets" : {
            "res_pack1" : "C:/MyProjects/AwesomeGame/Assets/pack1",
            "res_pack2" : "C:/MyProjects/AwesomeGame/Assets/pack2"
        },
        "output_dir_debug": "C:/MyProjects/AwesomeGame/build/Example/Debug",
        "output_dir_release": "C:/MyProjects/AwesomeGame/build/Example/Release",
        "output_sub_dir": "my_sub_directory",
        "encryption_key" : "my-encryption_key"
    }


## Example

    include <iostream>
    include "raylib.h"
    include "resource_loader.hpp"
    
    int main()
    {
        InitWindow(600, 600, "");
        SetTargetFPS(60);
        InitAudioDevice();
        
        std::vector<char> music_data;
        std::vector<char> img_data;
    
        // Pack 1
        {
            rm::rLdr::PackBuffer pack_1 = rm::rLdr::create_pack_buffer("res_pack1.pk", "ioaosdjo");
    
            rm::rLdr::open_pack_buffer(pack_1);
    
            music_data = rm::rLdr::get_pack_data(pack_1, "pack1/Audio/Music/Heavy Riffs - More Gain - Theme #2.wav");
    
            rm::rLdr::close_pack_buffer(pack_1);
        }
    
        // Pack 2
        {
            rm::rLdr::PackBuffer pack_2 = rm::rLdr::create_pack_buffer("res_pack2.pk", "ioaosdjo");
    
            rm::rLdr::open_pack_buffer(pack_2);
    
            img_data = rm::rLdr::get_pack_data(pack_2,"pack2/Textures/ammo/ammo_02.png");
    
            rm::rLdr::close_pack_buffer(pack_2);
        }
        
        Music music = LoadMusicStreamFromMemory(".wav", (unsigned char*)music_data.data(), music_data.size());
    
        Image img = LoadImageFromMemory(".png", (unsigned char*)img_data.data(), img_data.size());
        Texture2D texture = LoadTextureFromImage(img);
        UnloadImage(img);
    
        PlayMusicStream(music);
    
        while (!WindowShouldClose())
        {
            UpdateMusicStream(music);
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawTexture(texture, 0, 0, WHITE);
    
            EndDrawing();
        }
        UnloadTexture(texture);
        UnloadMusicStream(music);
        CloseWindow();
    
        std::cout << "test finished...\n";
        
        return 0;
    }
