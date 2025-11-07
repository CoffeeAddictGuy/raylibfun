#include "raylib.h"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "RayLib Test");
  SetTargetFPS(144);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLUE);
    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
