# ResourceManagement_CPP

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
