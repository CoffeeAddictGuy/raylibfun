#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

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
static void playerMoveX(float dX);
static void playerMoveY(float dY);
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

void updateGame() { inputPlayer(); }

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
    playerMoveY(-GetFrameTime() * player.speed);
  }
  if (IsKeyDown(KEY_S)) {
    playerMoveY(GetFrameTime() * player.speed);
  }
  if (IsKeyDown(KEY_D)) {
    playerMoveX(GetFrameTime() * player.speed);
  }
  if (IsKeyDown(KEY_A)) {
    playerMoveX(-GetFrameTime() * player.speed);
  }
  if (DEBUG && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    TraceLog(LOG_DEBUG, "Mouse cord:(%f,%f)", GetMousePosition());
    player.position = GetMousePosition();
    player.collider.x = GetMousePosition().x - (player.size.x / 10);
    player.collider.y = GetMousePosition().y - (player.size.y / 10);
  }
}

void playerMoveX(float dX) {
  player.position.x += dX;
  player.collider.x = player.position.x - (player.size.x / 10);
  if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
    player.position.x -= dX;
    player.collider.x = player.position.x - (player.size.x / 10);
  }
  if (player.position.x >= width - player.size.x - 5 ||
      player.position.x <= 0 + 5) {
    player.position.x -= dX;
    player.collider.x = player.position.x - (player.size.x / 10);
  }
}

void playerMoveY(float dY) {
  player.position.y += dY;
  player.collider.y = player.position.y - (player.size.y / 10);
  if (CheckCollisionRecs(player.collider, (Rectangle){200, 300, 100, 50})) {
    player.position.y -= dY;
    player.collider.y = player.position.y - (player.size.y / 10);
  }
  if (player.position.y >= height - player.size.y - 5 ||
      player.position.y < 0 + 5) {
    player.position.y -= dY;
    player.collider.y = player.position.y - (player.size.y / 10);
  }
}

void drawDebug() {
  // Player collider
  DrawRectangleLines(player.collider.x, player.collider.y,
                     player.collider.width, player.collider.height, GRAY);
  TraceLog(LOG_DEBUG, "Player:\nx-%f\ny-%f", player.position.x,
           player.position.y);
}
