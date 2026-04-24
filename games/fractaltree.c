#include <math.h>
#include <raylib.h>

#define WIDTH 1200
#define HEIGHT 800
#define GEN_LIMIT 20
#define BRANCH_OFF_ANGLE 20 * DEG2RAD

float branch_lenght = 300.f;
float thickness = 20;
Vector2 start_point = (Vector2){WIDTH / 2.f, 750};

int curr_gen = 0;

void DrawBranch(Vector2 point, float lenght, float angle, float thick) {
  if (lenght <= 1) {
    return;
  }
  Vector2 start = {point.x, point.y};
  Vector2 end =
      (Vector2){point.x + sinf(angle) * lenght, point.y - cosf(angle) * lenght};
  DrawLineEx(start, end, thick, BLACK);
  curr_gen++;
  // if (curr_gen >= GEN_LIMIT) {
  //   return;
  // }
  float new_lenght = lenght * 0.7;
  float new_right_angle = angle + BRANCH_OFF_ANGLE;
  float new_left_angle = angle - BRANCH_OFF_ANGLE;
  float new_thick = thick * 0.7;

  DrawBranch(end, new_lenght, new_right_angle, new_thick);
  DrawBranch(end, new_lenght, new_left_angle, new_thick);
}

void draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Fractal tree", WIDTH / 2 - 100, HEIGHT / 8, 20, GRAY);
  curr_gen = 0;
  DrawBranch(start_point, branch_lenght, 0, thickness);
  EndDrawing();
}

void update() {}

int main() {
  InitWindow(WIDTH, HEIGHT, "Fractal tree");
  SetTargetFPS(10);
  while (!WindowShouldClose()) {
    draw();
    update();
  }
}
