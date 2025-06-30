#include <iostream>
#include "raylib.h"
#include "resource_loader.hpp"
#include <fstream>
#include <filesystem>



int main()
{
    InitWindow(800, 800, "");
    InitAudioDevice();
    SetTargetFPS(60);


    ResourceManagement::ResourceLoader::init("resources.pk");

    // Texture
    std::vector<char> img_data = ResourceManagement::ResourceLoader::get_raw_resource_data("Assets/Textures/Icons/ammo_01.PNG");
    Image img = LoadImageFromMemory(".png", (unsigned char*)img_data.data(), img_data.size());
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    // Music
    std::vector<char> music_data = ResourceManagement::ResourceLoader::get_raw_resource_data("Assets/Audio/Music/Triumphant Orchestral Theme #4.wav");
    Music music = LoadMusicStreamFromMemory(".wav", (unsigned char*)music_data.data(), music_data.size());
    PlayMusicStream(music);
    
    // Sound
    std::vector<char> sound_data = ResourceManagement::ResourceLoader::get_raw_resource_data("Assets/Audio/Soundfx/chimes_magic_bell_ding_5.wav");
    Wave s_wave = LoadWaveFromMemory(".wav", (unsigned char*)sound_data.data(), sound_data.size());
    Sound sound = LoadSoundFromWave(s_wave);

    std::cout << "Checking if defs\n";

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            PlaySound(sound);
        }
        UpdateMusicStream(music);

        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }
    
    UnloadTexture(texture);
    UnloadMusicStream(music);
    CloseWindow();

    return 0;
}