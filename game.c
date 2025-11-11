#include "raylib.h"
#include "raymath.h"

#define MAX_HP 5
#define LINES_OF_BRICKS 5
#define BRICKS_PER_LINE 20

typedef struct Player {
  Vector2 pos;
  int speed;
  bool active;
} Player;

typedef struct Ball {
  Vector2 pos;
  int radius;
  Vector2 speed;
  bool active;
} Ball;

typedef struct Brick {
  Vector2 pos;
  bool active;
} Brick;

bool isMenu = false;

static void gameInit();
static void drawMenu();
static void drawGame();
static void updateGame();
static void drawnupdateGame();
static void inputGame();

static Player player = {0};
static Ball ball = {0};
static Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = {0};
static Vector2 brickSize = {0};

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
  SetTraceLogLevel(LOG_DEBUG);
  int focus = 0;
  InitWindow(screenWidth, screenHeight, "Arcanoid");
  SetTargetFPS(144);

  gameInit();

  while (!WindowShouldClose()) {
    if (!isMenu) {
      inputGame();
      drawMenu();
    } else {
      inputGame();
      drawnupdateGame();
    }
  }
  CloseWindow();
  return 0;
}

void gameInit() {
  player = (Player){(Vector2){600, 400}, 20.f, true};
  ball = (Ball){(Vector2){player.pos.x, player.pos.y}, 10, 0, false};

  brickSize = (Vector2){GetScreenWidth() / BRICKS_PER_LINE, 40};
  int initDownPos = 50;
  for (int i = 0; i < LINES_OF_BRICKS; i++) {
    for (int j = 0; j < BRICKS_PER_LINE; j++) {
      brick[i][j].pos = (Vector2){j * brickSize.x + brickSize.x / 2,
                                  i * brickSize.y + initDownPos};
      brick[i][j].active = true;
    }
  }
}

void drawMenu() {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawText("To start play - press Enter!", 230, 200, 20, LIGHTGRAY);
  EndDrawing();
}

void updateGame() {}

void drawGame() {
  BeginDrawing();
  ClearBackground(BLUE);
  // Draw bricks
  for (int i = 0; i < LINES_OF_BRICKS; i++) {
    for (int j = 0; j < BRICKS_PER_LINE; j++) {
      if (brick[i][j].active) {
        if ((i + j) % 2 == 0)
          DrawRectangle(brick[i][j].pos.x - brickSize.x / 2,
                        brick[i][j].pos.y - brickSize.y / 2, brickSize.x - 10,
                        brickSize.y - 20, GREEN);
        else
          DrawRectangle(brick[i][j].pos.x - brickSize.x / 2,
                        brick[i][j].pos.y - brickSize.y / 2, brickSize.x - 10,
                        brickSize.y - 20, RED);
      }
    }
  }
  // Draw player
  DrawRectangle(player.pos.x, player.pos.y, 70, 20, BLACK);

  // Draw ball
  if (!ball.active)
    DrawCircle(player.pos.x + 70.0 / 2, player.pos.y - 20, ball.radius, YELLOW);
  EndDrawing();
}

void inputGame() {
  if (IsKeyPressed(KEY_ENTER) && !isMenu) {
    TraceLog(LOG_DEBUG, "ENTER PRESSED!");
    isMenu = !isMenu;
  }
  if (IsKeyDown(KEY_A) && player.pos.x > 0)
    player.pos.x -= GetFrameTime() * player.speed;
  if (IsKeyDown(KEY_D) && player.pos.x < screenWidth - 70)
    player.pos.x += GetFrameTime() * player.speed;
}

void drawnupdateGame() {
  drawGame();
  updateGame();
}
