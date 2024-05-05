/// Author : GhostOne
/// Created Date : 5th May, 2024
/// <summary>
/// Entry point of the game
/// </summary>


#include "game-config.h"

#include <raylib.h>

using namespace std;

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);
        DrawText("Hello World", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}