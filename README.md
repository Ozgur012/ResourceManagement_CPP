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
        "encryption_key" : "my-encryption_key123456789"
    }

### Suported file types

    .png, .jpg, .jpeg, .ogg, .wav, .mp3

## Example

    #include "raylib.h"
    #include "resource_loader.hpp"
    
    int main()
    {
        InitWindow(600, 600, "");
        SetTargetFPS(60);
        InitAudioDevice();
        
        std::vector<char> music_data;
        std::vector<char> player_img_data;
        std::vector<char> enemy_img_data;
    
        // Pack 1
        {
            auto pack_1 = rm::rLdr::create_pack_buffer("my_sub_directory/res_pack1.pk", "my-encryption_key123456789");
    
            rm::rLdr::open_pack_buffer(pack_1);
    
            music_data = rm::rLdr::get_pack_data(pack_1, "pack1/Audio/awesome_song.wav");
    
            rm::rLdr::close_pack_buffer(pack_1);
        }
    
        // Pack 2
        {
            auto pack_2 = rm::rLdr::create_pack_buffer("my_sub_directory/res_pack2.pk", "my-encryption_key123456789");
    
            rm::rLdr::open_pack_buffer(pack_2);
    
            player_img_data = rm::rLdr::get_pack_data(pack_2,"pack2/Textures/Player/player_texture.png");
            enemy_img_data = rm::rLdr::get_pack_data(pack_2,"pack2/Textures/Enemy/enemy_texture.png");
    
            rm::rLdr::close_pack_buffer(pack_2);
        }
        
        Music music = LoadMusicStreamFromMemory(".wav", (unsigned char*)music_data.data(), music_data.size());
    
        Image player_img = LoadImageFromMemory(".png", (unsigned char*)player_img_data.data(), player_img_data.size());
        Texture2D player_texture = LoadTextureFromImage(player_img);
        UnloadImage(player_img);

        Image enemy_img = LoadImageFromMemory(".png", (unsigned char*)enemy_img_data.data(), enemy_img_data.size());
        Texture2D enemy_texture = LoadTextureFromImage(enemy_img);
        UnloadImage(enemy_img);
    
        PlayMusicStream(music);
    
        while (!WindowShouldClose())
        {
            UpdateMusicStream(music);
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawTexture(player_texture, 0, 0, WHITE);
            DrawTexture(enemy_texture, 0, 200, WHITE);
            EndDrawing();
        }
        UnloadMusicStream(music);
        UnloadTexture(player_texture);
        UnloadTexture(enemy_texture);
        CloseWindow();
        
        return 0;
    }
