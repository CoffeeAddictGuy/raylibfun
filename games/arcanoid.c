#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

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
bool isGame = false;

static void gameInit();
static void drawMenu();
static void drawGame();
static void updateGame();
static void drawnupdateGame();
static void inputGame();
static void restartGame();

static Player player = {0};
static Ball ball = {0};
static Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = {0};
static Vector2 brickSize = {0};

const int screenWidth = 800;
const int screenHeight = 450;
const float playerNormalSpeed = 2.f;
const float playerFastSpeed = 3.5f;

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
  player = (Player){(Vector2){600, 400}, playerNormalSpeed, true};
  ball =
      (Ball){(Vector2){player.pos.x, player.pos.y}, 10, (Vector2){0, 0}, false};
  brickSize = (Vector2){(float)GetScreenWidth() / BRICKS_PER_LINE, 40};
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

void updateGame() {
  if (ball.active) {
    ball.pos.y += ball.speed.y;
    ball.pos.x += ball.speed.x;
  } else {
    ball.pos =
        (Vector2){ball.pos.x = player.pos.x + 70.0 / 2, player.pos.y - 20.0};
  }

  // Collision ball and player
  if (CheckCollisionCircleRec(ball.pos, ball.radius,
                              (Rectangle){player.pos.x, player.pos.y, 70, 20}))
    if (ball.speed.y > 0) {
      ball.speed.y *= -1;
      ball.speed.x = (ball.pos.x - player.pos.x) / (70.f / 2) * 1.2;
    }

  // ball hit wall
  if (((ball.pos.x + ball.radius) >= screenWidth) ||
      ((ball.pos.x - ball.radius) <= 0))
    ball.speed.x *= -1;
  if ((ball.pos.y - ball.radius) <= 0)
    ball.speed.y *= -1;
  if ((ball.pos.y + ball.radius) >= screenHeight) {
    ball.speed = (Vector2){0, 0};
    ball.active = false;
    restartGame();
  }

  // Collision ball and brick
  for (int i = 0; i < LINES_OF_BRICKS; i++) {
    for (int j = 0; j < BRICKS_PER_LINE; j++) {
      if (brick[i][j].active) {
        // col below
        if (((ball.pos.y - ball.radius) <=
             (brick[i][j].pos.y + brickSize.y / 2)) &&
            ((ball.pos.y - ball.radius) >
             (brick[i][j].pos.y + brickSize.y / 2 + ball.speed.y)) &&
            ((fabs(ball.pos.x - brick[i][j].pos.x)) <
             (brickSize.x / 2 + ball.radius * 2.f / 3)) &&
            (ball.speed.y < 0)) {
          brick[i][j].active = false;
          ball.speed.y *= -1;
          TraceLog(LOG_DEBUG, "Hit below");
          TraceLog(LOG_DEBUG, "I hit brick - %d/%d", i, j);
        }

        // col above
        if (((ball.pos.y + ball.radius) >=
             (brick[i][j].pos.y - brickSize.y / 2)) &&
            ((ball.pos.y + ball.radius) <
             (brick[i][j].pos.y - brickSize.y / 2 + ball.speed.y)) &&
            ((fabs(ball.pos.x - brick[i][j].pos.x)) <
             (brickSize.x / 2 + ball.radius * 2.f / 3)) &&
            (ball.speed.y > 0)) {
          brick[i][j].active = false;
          ball.speed.y *= -1;
          TraceLog(LOG_DEBUG, "Hit above");
          TraceLog(LOG_DEBUG, "I hit brick - %d/%d", i, j);
        }

        // col left
        if (((ball.pos.x + ball.radius) >=
             (brick[i][j].pos.x - brickSize.x / 2)) &&
            ((ball.pos.x + ball.radius) <
             (brick[i][j].pos.x - brickSize.x / 2 + ball.speed.x)) &&
            ((fabs(ball.pos.y - brick[i][j].pos.y)) <
             (brickSize.y / 2 + ball.radius * 2.f / 3)) &&
            (ball.speed.x > 0)) {
          brick[i][j].active = false;
          ball.speed.x *= -1;
          TraceLog(LOG_DEBUG, "Hit left");
          TraceLog(LOG_DEBUG, "I hit brick - %d/%d", i, j);
        }

        // col right
        if (((ball.pos.x - ball.radius) <=
             (brick[i][j].pos.x + brickSize.x / 2)) &&
            ((ball.pos.x - ball.radius) >
             (brick[i][j].pos.x + brickSize.x / 2 + ball.speed.x)) &&
            ((fabs(ball.pos.y - brick[i][j].pos.y)) <
             (brickSize.y / 2 + ball.radius * 2.f / 3)) &&
            (ball.speed.x < 0)) {
          brick[i][j].active = false;
          ball.speed.x *= -1;
          TraceLog(LOG_DEBUG, "Hit right");
          TraceLog(LOG_DEBUG, "I hit brick - %d/%d", i, j);
        }
      }
    }
  }
}

void drawGame() {
  BeginDrawing();
  ClearBackground(BLUE);
  // Draw bricks
  for (int i = 0; i < LINES_OF_BRICKS; i++) {
    for (int j = 0; j < BRICKS_PER_LINE; j++) {
      if (brick[i][j].active) {
        if ((i + j) % 2 == 0) {
          DrawRectangle(brick[i][j].pos.x - brickSize.x / 2,
                        brick[i][j].pos.y - brickSize.y / 2, brickSize.x,
                        brickSize.y, GREEN);
        } else
          DrawRectangle(brick[i][j].pos.x - brickSize.x / 2,
                        brick[i][j].pos.y - brickSize.y / 2, brickSize.x,
                        brickSize.y, RED);
      } else {
        DrawRectangle(brick[i][j].pos.x - brickSize.x / 2,
                      brick[i][j].pos.y - brickSize.y / 2, brickSize.x,
                      brickSize.y, GRAY);
      }
      int iS = (i == 0) ? 1 : 1;
      DrawText(TextFormat("%i/%i", i, j), brick[i][j].pos.x, brick[i][j].pos.y,
               2, BLACK);
    }
  }
  // Draw player
  DrawRectangle(player.pos.x, player.pos.y, 70, 20, BLACK);

  // Draw ball
  DrawCircle(ball.pos.x, ball.pos.y, ball.radius, YELLOW);
  EndDrawing();
}

void inputGame() {
  if (IsKeyPressed(KEY_ENTER) && !isMenu) {
    TraceLog(LOG_DEBUG, "ENTER PRESSED!");
    isMenu = !isMenu;
  }
  if (IsKeyDown(KEY_A) && player.pos.x > 0) {
    player.pos.x -= player.speed;
    player.active = true;
  } else {
    player.active = false;
  }
  if (IsKeyDown(KEY_D) && player.pos.x < screenWidth - 70) {
    player.pos.x += player.speed;
    player.active = true;
  } else {
    player.active = false;
  }
  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    player.speed = playerFastSpeed;
  }
  if (IsKeyReleased(KEY_LEFT_SHIFT)) {
    player.speed = playerNormalSpeed;
  }
  if (IsKeyPressed(KEY_SPACE) && !isGame && !ball.active) {
    isGame = !isGame;
    if (!player.active) {
      ball.speed = (Vector2){0, -1.2};
      ball.pos = (Vector2){player.pos.x + 70.0 / 2, player.pos.y - 20};
      ball.active = !ball.active;
    }
  }
  if (IsKeyPressed(KEY_R) && isGame) {
    TraceLog(LOG_DEBUG, "GAME RESTARTED!");
    restartGame();
  }
}

void drawnupdateGame() {
  drawGame();
  updateGame();
}

void restartGame() {
  gameInit();
  isGame = !isGame;
}
