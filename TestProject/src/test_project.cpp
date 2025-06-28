#include <iostream>
#include "raylib.h"
#include "resource_loader.hpp"
#include <fstream>
#include <filesystem>



int main()
{
    InitWindow(800, 800, "");

    ResourceManagement::ResourceLoader::init("resources.pk");

    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKBLUE);

        EndDrawing();
    }
    
    CloseWindow();



    return 0;
}