#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define DEBUG 1
#define MAX_BALL_COUNT 20

const int WIDTH = 1920;
const int HEIGHT = 1080;
const Color colorPicker[5] = {BLACK, GREEN, YELLOW, BLUE, PURPLE};
const int plrSpeed = 50;
const int plrRushSpeed = 150;

typedef struct Player {
  Vector2 position;
  float rotation;
  float speed;
} Player;

typedef struct Ball {
  Vector2 position;
  int radius;
  Color color;
} Ball;

static void initGame();
static void drawGame();
static void updateGame();
static void drawupdateGame();
static void drawDebug();

static void LookAt();

static Player plr = {0};
static Ball ball[MAX_BALL_COUNT] = {0};
static Camera2D cmr;
static Vector2 worldMouse;

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WIDTH, HEIGHT, "Player control");
  initGame();
  while (!WindowShouldClose()) {
    drawupdateGame();
  }
}

void initGame() {
  plr.position = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};
  plr.rotation = 0;
  plr.speed = plrSpeed;
  for (int i = 0; i < MAX_BALL_COUNT; i++) {
    ball[i].position =
        (Vector2){rand() % (2001 - 1000), rand() % (2001 - 1000)};
    ball[i].radius = rand() % (200 - 50 + 1) + 50;
    ball[i].color = colorPicker[rand() % 5];
  }
  cmr.zoom = 1;
  cmr.target = plr.position;
}

void drawGame() {
  BeginDrawing();
  BeginMode2D(cmr);
  ClearBackground(RAYWHITE);

  // draw balls
  for (int i = 0; i < MAX_BALL_COUNT; i++) {
    DrawCircle(ball[i].position.x, ball[i].position.y, ball[i].radius,
               ball[i].color);
  }

  // draw player
  Vector2 v1 = (Vector2){plr.position.x + sinf(plr.rotation * DEG2RAD) * (30),
                         plr.position.y - cosf(plr.rotation * DEG2RAD) * (30)};
  Vector2 v2 = (Vector2){plr.position.x - cosf(plr.rotation * DEG2RAD) * (20),
                         plr.position.y - sinf(plr.rotation * DEG2RAD) * (20)};
  Vector2 v3 = (Vector2){plr.position.x + cosf(plr.rotation * DEG2RAD) * (20),
                         plr.position.y + sinf(plr.rotation * DEG2RAD) * (20)};
  DrawTriangle(v1, v2, v3, RED);

  // draw debug
  if (DEBUG)
    drawDebug();

  EndMode2D();
  if (DEBUG)
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 50, BLACK);

  EndDrawing();
}

void updateGame() {
  if (plr.rotation > 360 || plr.rotation < -360)
    plr.rotation = 0;
  if (IsKeyDown(KEY_LEFT_SHIFT))
    plr.speed = plrRushSpeed;
  else
    plr.speed = plrSpeed;
  if (IsKeyDown(KEY_W))
    plr.position.y -= GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_S))
    plr.position.y += GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_A))
    plr.position.x -= GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_D))
    plr.position.x += GetFrameTime() * plr.speed;
  cmr.target = (Vector2){plr.position.x, plr.position.y};
  cmr.offset = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};
  cmr.zoom += (float)GetMouseWheelMove() * 0.1f;
  if (cmr.zoom > 5.f)
    cmr.zoom = 3.f;
  else if (cmr.zoom < 1.f)
    cmr.zoom = 1.f;
  worldMouse = GetScreenToWorld2D(GetMousePosition(), cmr);
  LookAt();
}

void drawupdateGame() {
  drawGame();
  updateGame();
}

void drawDebug() {
  DrawLineV(plr.position, worldMouse, BLACK);
  TraceLog(LOG_DEBUG, "Mouse pos:\nx:%f\ny:%f\n", GetMousePosition().x,
           GetMousePosition().y);
  TraceLog(LOG_DEBUG, "Player pos:\nx:%f\ny:%f\nPlayer rotation:\n%f",
           plr.position.x, plr.position.y, plr.rotation);
}

void LookAt() {
  float resX = worldMouse.x - plr.position.x;
  float resY = worldMouse.y - plr.position.y;
  float res = atan2(resY, resX) * RAD2DEG + 90;
  plr.rotation = res;
}
