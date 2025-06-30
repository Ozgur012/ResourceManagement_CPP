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


    ResourceManagement::ResourceLoader::init("resources.pk"); // A path is supplied here. It is relative to your binary (exe).

    // Loading resources

    // Access name example:
    //      Input dir: c:/Projects/MyAwsomegame/Assets
    //      Packed file: c:/Projects/MyAwsomegame/Assets/Textures/Player/player.png
    //      Access name: Assets/Textures/Player/player.png

    // Texture
    std::vector<char> img_data = ResourceManagement::ResourceLoader::get_raw_resource_data("access name of your png");
    Image img = LoadImageFromMemory(".png", (unsigned char*)img_data.data(), img_data.size());
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    // Music
    std::vector<char> music_data = ResourceManagement::ResourceLoader::get_raw_resource_data("access name of your ogg");
    Music music = LoadMusicStreamFromMemory(".ogg", (unsigned char*)music_data.data(), music_data.size());
    PlayMusicStream(music);
    
    // Sound
    std::vector<char> sound_data = ResourceManagement::ResourceLoader::get_raw_resource_data("access name of your wav");
    Wave s_wave = LoadWaveFromMemory(".wav", (unsigned char*)sound_data.data(), sound_data.size());
    Sound sound = LoadSoundFromWave(s_wave);

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