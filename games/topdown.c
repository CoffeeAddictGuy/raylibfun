#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#define ROOMWIDTH 20
#define ROOMHEIGHT 20
#define DEBUG 1

int width = 800;
int height = 450;

const float playerNormalSpeed = 50.f;
const float playerFastSpeed = 100.f;

typedef struct Player {
  Vector2 position;
  Vector2 size;
  Rectangle collider;
  float speed;
} Player;

typedef struct roomPlace {
  Vector2 position;
  int type;
} roomPlace;

static Player player = {0};
static Vector2 playerLastPos = {0};
static Rectangle hit = {0};
static bool playerCollide = 0;

static roomPlace room[ROOMWIDTH][ROOMHEIGHT] = {0};
static Vector2 roomSize = {0};

static void gameInit();
static void drawGame();
static void updateGame();
static void drawupdateGame();

static void initPlayer();
static void drawPlayer();
static void inputPlayer();
static void drawDebug();

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  SetTargetFPS(60);
  InitWindow(width, height, "Mini Jam 198");
  gameInit();
  while (!WindowShouldClose()) {
    drawupdateGame();
  }
}

void gameInit() { initPlayer(); }

void drawGame() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawRectangle(200, 300, 100, 50, YELLOW);
  drawPlayer();
  if (DEBUG)
    drawDebug();
  EndDrawing();
}

void updateGame() {
  inputPlayer();
  if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
    TraceLog(LOG_DEBUG, "Collider check");
  }
}

void drawupdateGame() {
  drawGame();
  updateGame();
}

void initPlayer() {
  // Player
  player.position = (Vector2){(float)width / 2, (float)height / 2};
  player.size = (Vector2){30, 30};
  player.collider = (Rectangle){player.position.x - (player.size.x / 10),
                                player.position.y - (player.size.y / 10),
                                player.size.x + player.size.x / 5,
                                player.size.y + player.size.y / 5};
  player.speed = playerNormalSpeed;
}

void drawPlayer() {
  DrawRectangle(player.position.x, player.position.y, player.size.x,
                player.size.y, GREEN);
}

void inputPlayer() {
  if (IsKeyDown(KEY_LEFT_SHIFT))
    player.speed = playerFastSpeed;
  else
    player.speed = playerNormalSpeed;
  if (IsKeyDown(KEY_W)) {
    player.position.y -= GetFrameTime() * player.speed;
    player.collider.y = player.position.y - (player.size.x / 10);
    if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
      player.position.y += GetFrameTime() * player.speed;
      player.collider.y = player.position.y - (player.size.x / 10);
    }
  }
  if (IsKeyDown(KEY_S)) {
    player.position.y += GetFrameTime() * player.speed;
    player.collider.y = player.position.y - (player.size.x / 10);
    if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
      player.position.y -= GetFrameTime() * player.speed;
      player.collider.y = player.position.y - (player.size.x / 10);
    }
  }
  if (IsKeyDown(KEY_D)) {
    player.position.x += GetFrameTime() * player.speed;
    player.collider.x = player.position.x - (player.size.x / 10);
    if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
      player.position.x -= GetFrameTime() * player.speed;
      player.collider.x = player.position.x - (player.size.x / 10);
    }
  }
  if (IsKeyDown(KEY_A)) {
    player.position.x -= GetFrameTime() * player.speed;
    player.collider.x = player.position.x - (player.size.x / 10);
    if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
      player.position.x += GetFrameTime() * player.speed;
      player.collider.x = player.position.x - (player.size.x / 10);
    }
  }
}

void drawDebug() {
  // Player collider
  DrawRectangleLines(player.collider.x, player.collider.y,
                     player.collider.width, player.collider.height, GRAY);
}
