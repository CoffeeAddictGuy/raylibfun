#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define GLSL_VERSION 330
#define DEBUG 1
#define MAX_BALL_COUNT 20
#define MAX_BULLET_COUNT 10

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
  bool active;
  Color color;
} Ball;

typedef struct Bullet {
  Vector2 position;
  Vector2 speed;
  int radius;
  float lifetime;
  bool active;
  Color color;
} Bullet;

static void initGame();
static void drawGame();
static void updateGame();
static void drawupdateGame();
static void drawDebug();
static void printDebug();

static void LookAt();

static Player plr = {0};
static Ball ball[MAX_BALL_COUNT] = {0};
static Bullet bullet[MAX_BULLET_COUNT] = {0};
static Camera2D cmr;
static Vector2 worldMouse;
static bool testBall = false;
static Shader shader;

int main() {
  srand(time(NULL));
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WIDTH, HEIGHT, "Player control");
  // SetTargetFPS(30);
  initGame();

  shader = LoadShader(0, TextFormat("shader.frag", GLSL_VERSION));
  int objectColorLoc = GetShaderLocation(shader, "objectColor");
  int lightColorLoc = GetShaderLocation(shader, "lightColor");

  Vector3 objColor = {1.0f, 1.0f, 1.0f};
  Vector3 lightColor = {1.0f, 1.0f, 0.5f};

  SetShaderValue(shader, objectColorLoc, &objColor, SHADER_UNIFORM_VEC3);
  SetShaderValue(shader, lightColorLoc, &lightColor, SHADER_UNIFORM_VEC3);

  while (!WindowShouldClose()) {
    drawupdateGame();
  }
}

void initGame() {
  // Player
  plr.position = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};
  plr.rotation = 0;
  plr.speed = plrSpeed;

  // Balls (aka planet or something)
  for (int i = 0; i < MAX_BALL_COUNT; i++) {
    ball[i].position =
        (Vector2){rand() % (2001 - 1000), rand() % (2001 - 1000)};
    ball[i].radius = rand() % (200 - 50 + 1) + 50;
    ball[i].color = colorPicker[rand() % 5];
    ball[i].active = false;
  }
  ball[5].active = true;
  ball[6].active = true;

  // Bullet
  for (int i = 0; i < MAX_BULLET_COUNT; i++) {
    bullet[i].position = plr.position;
    bullet[i].speed = (Vector2){0, 0};
    bullet[i].radius = 5;
    bullet[i].lifetime = 0;
    bullet[i].color = (Color){165, 255, 57, 255};
    bullet[i].active = false;
  }

  // Camera
  cmr.zoom = 1;
  cmr.target = plr.position;
}

void drawGame() {
  BeginDrawing();
  BeginMode2D(cmr);
  ClearBackground(RAYWHITE);
  // draw balls
  // for (int i = 0; i < MAX_BALL_COUNT; i++) {
  //   DrawCircle(ball[i].position.x, ball[i].position.y, ball[i].radius,
  //              ball[i].color);
  // }
  if (ball[5].position.x + ball[5].radius > cmr.target.x - ((float)WIDTH / 4) &&
      ball[5].position.x - ball[5].radius < cmr.target.x + ((float)WIDTH / 4) &&
      ball[5].position.y - ball[5].radius <
          cmr.target.y + ((float)HEIGHT / 4) &&
      ball[5].position.y + ball[5].radius >
          cmr.target.y - ((float)HEIGHT / 4)) {
    BeginShaderMode(shader);
    DrawCircle(ball[5].position.x, ball[5].position.y, ball[5].radius,
               ball[5].color);
    EndShaderMode();
  }
  if (testBall) {
    BeginShaderMode(shader);
    DrawCircle(ball[5].position.x, ball[5].position.y, ball[5].radius,
               ball[5].color);
    EndShaderMode();
  }
  DrawCircle(ball[6].position.x, ball[6].position.y, ball[6].radius,
             ball[6].color);
  // draw player
  Vector2 v1 = (Vector2){plr.position.x + sinf(plr.rotation * DEG2RAD) * (30),
                         plr.position.y - cosf(plr.rotation * DEG2RAD) * (30)};
  Vector2 v2 = (Vector2){plr.position.x - cosf(plr.rotation * DEG2RAD) * (20),
                         plr.position.y - sinf(plr.rotation * DEG2RAD) * (20)};
  Vector2 v3 = (Vector2){plr.position.x + cosf(plr.rotation * DEG2RAD) * (20),
                         plr.position.y + sinf(plr.rotation * DEG2RAD) * (20)};
  DrawTriangle(v1, v2, v3, RED);

  // Bullet
  for (int i = 0; i < MAX_BULLET_COUNT; i++) {
    if (bullet[i].active)
      DrawCircle(bullet[i].position.x, bullet[i].position.y, bullet[i].radius,
                 bullet[i].color);
  }

  // draw debug
  if (DEBUG) {
    drawDebug();
    printDebug();
  }

  EndMode2D();
  if (DEBUG)
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 50, BLACK);

  EndDrawing();
}

void updateGame() {
  if (IsKeyPressed(KEY_P))
    testBall = !testBall;
  // Player speed
  if (IsKeyDown(KEY_LEFT_SHIFT))
    plr.speed = plrRushSpeed;
  else
    plr.speed = plrSpeed;

  // Player movement
  if (IsKeyDown(KEY_W))
    plr.position.y -= GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_S))
    plr.position.y += GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_A))
    plr.position.x -= GetFrameTime() * plr.speed;
  if (IsKeyDown(KEY_D))
    plr.position.x += GetFrameTime() * plr.speed;

  // Shoot
  if (IsKeyPressed(KEY_E)) {
    for (int i = 0; i < MAX_BULLET_COUNT; i++) {
      if (!bullet[i].active) {
        bullet[i].position =
            (Vector2){plr.position.x + sin(plr.rotation * DEG2RAD) * (20),
                      plr.position.y - cos(plr.rotation * DEG2RAD) * (20)};
        bullet[i].active = true;
        bullet[i].speed.x = 20 * sin(plr.rotation * DEG2RAD) * plr.speed;
        bullet[i].speed.y = 20 * cos(plr.rotation * DEG2RAD) * plr.speed;
        break;
      }
    }
  }

  for (int i = 0; i < MAX_BULLET_COUNT; i++) {
    if (bullet[i].active) {
      bullet[i].lifetime += GetFrameTime();
    }
  }

  for (int i = 0; i < MAX_BULLET_COUNT; i++) {
    if (bullet[i].active) {
      // Move
      bullet[i].position.x += bullet[i].speed.x * GetFrameTime();
      bullet[i].position.y -= bullet[i].speed.y * GetFrameTime();

      for (int j = 0; j < MAX_BALL_COUNT; j++) {
        if (CheckCollisionCircles(bullet[i].position, bullet[i].radius,
                                  ball[j].position, ball[j].radius) &&
            ball[j].active) {
          TraceLog(LOG_DEBUG, "Bullet hit!");
          bullet[i].lifetime = 1.f;
          break;
        }
      }

      // Lifetime
      if (bullet[i].lifetime >= 0.8f) {
        bullet[i].position = (Vector2){0, 0};
        bullet[i].lifetime = 0;
        bullet[i].speed = (Vector2){0, 0};
        bullet[i].active = false;
      }
    }
  }

  // Camera update
  cmr.target = (Vector2){plr.position.x, plr.position.y};
  cmr.offset = (Vector2){(float)WIDTH / 2, (float)HEIGHT / 2};

  // Camera zoom in/out by mouse wheel
  cmr.zoom += (float)GetMouseWheelMove() * 0.1f;
  if (cmr.zoom > 5.f)
    cmr.zoom = 3.f;
  else if (cmr.zoom < 1.f)
    cmr.zoom = 1.f;

  // Get mouse pos in 2D world
  worldMouse = GetScreenToWorld2D(GetMousePosition(), cmr);

  // Player rotation limit and follow mouse to rotation
  if (plr.rotation > 360 || plr.rotation < -360)
    plr.rotation = 0;
  LookAt();
}

void drawupdateGame() {
  drawGame();
  updateGame();
}

void drawDebug() {
  DrawLineV(plr.position, worldMouse, BLACK);
  DrawRectangleLines(plr.position.x - ((float)WIDTH / 4),
                     plr.position.y - ((float)HEIGHT / 4), WIDTH / 2,
                     HEIGHT / 2, GRAY);
}

void printDebug() {
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
