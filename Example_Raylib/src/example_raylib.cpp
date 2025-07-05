#include <iostream>
#include "raylib.h"
#include "resource_loader.hpp"
#include <fstream>
#include <filesystem>

void test();

int main()
{
    test();
    return 0;
}

void test()
{
    InitWindow(600, 600, "");
    SetTargetFPS(60);
    InitAudioDevice();
    std::cout << "running test\n";


    
    rm::rLdr::PackBuffer pack_buff = rm::rLdr::create_pack_buffer("resources.pk", "ioaosdjo");

    // Load Resources
    //================================================================
    rm::rLdr::open_pack_buffer(pack_buff);

    std::vector<char> music_data = rm::rLdr::get_pack_data(pack_buff, "Assets/Audio/Music/Heavy Riffs - More Gain - Theme #2.wav");
    std::vector<char> img_data = rm::rLdr::get_pack_data(pack_buff,"Assets/Factory 64x64/PNG/gear 1.png");

    rm::rLdr::close_pack_buffer(pack_buff);
    //================================================================



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

}

// void run()
// {
//     InitWindow(800, 800, "");
//     InitAudioDevice();
//     SetTargetFPS(60);

//     rm::rLoader::init("resources.pk"); // A path is supplied here. It is relative to your binary (exe).

//     // Loading resources

//     // Access name example:
//     //      Input dir: c:/Projects/MyAwsomegame/Assets
//     //      Packed file: c:/Projects/MyAwsomegame/Assets/Textures/Player/player.png
//     //      Access name: Assets/Textures/Player/player.png

//     // Texture
//     std::vector<char> img_data = rm::rLoader::get_raw_resource_data("access name of your png");
//     Image img = LoadImageFromMemory(".png", (unsigned char*)img_data.data(), img_data.size());
//     Texture2D texture = LoadTextureFromImage(img);
//     UnloadImage(img);

//     // Music
//     std::vector<char> music_data = rm::rLoader::get_raw_resource_data("Assets/Audio/Music/TetrisSongOGG.ogg");
//     Music music = LoadMusicStreamFromMemory(".ogg", (unsigned char*)music_data.data(), music_data.size());
//     PlayMusicStream(music);
    
//     // Sound
//     std::vector<char> sound_data = rm::rLoader::get_raw_resource_data("access name of your wav");
//     Wave s_wave = LoadWaveFromMemory(".wav", (unsigned char*)sound_data.data(), sound_data.size());
//     Sound sound = LoadSoundFromWave(s_wave);

//     while (!WindowShouldClose())
//     {
//         if (IsKeyPressed(KEY_SPACE))
//         {
//             PlaySound(sound);
//         }
//         UpdateMusicStream(music);

//         BeginDrawing();
//         ClearBackground(DARKBLUE);
//         DrawTexture(texture, 0, 0, WHITE);

//         EndDrawing();
//     }
    
//     UnloadTexture(texture);
//     UnloadMusicStream(music);
//     CloseWindow();
// }