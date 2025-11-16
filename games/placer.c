#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define BLOCKS_PER_LINE 20
#define BLOCKS_COL 5

const int WIDTH = 800;
const int HEIGHT = 450;

typedef struct Places {
  Vector2 position;
  bool active;
} Places;

typedef struct Inventory {
  Vector2 position;
};

typedef struct mType {
  int type;
} mType;

static void initGame();
static void drawGame();
static void drawInv();
static void updateGame();
static void duGame();

static Places place[BLOCKS_COL][BLOCKS_PER_LINE] = {0};
static Vector2 placeSize = {0};
static mType type = {0};

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WIDTH, HEIGHT, "Test");
  initGame();
  while (!WindowShouldClose()) {
    duGame();
  }
  return 0;
}

void initGame() {
  placeSize = (Vector2){GetScreenWidth() / BLOCKS_PER_LINE, 40};
  for (int i = 0; i < BLOCKS_COL; i++) {
    for (int j = 0; j < BLOCKS_PER_LINE; j++) {
      place[i][j].position =
          (Vector2){j * placeSize.x + 10, i * placeSize.y + 50};
      place[i][j].active = false;
    }
  }
}

void drawGame() {
  BeginDrawing();
  ClearBackground(BLUE);

  for (int i = 0; i < BLOCKS_COL; i++) {
    for (int j = 0; j < BLOCKS_PER_LINE; j++) {
      DrawRectangleLines(place[i][j].position.x, place[i][j].position.y,
                         placeSize.x / 2, placeSize.y / 2, BLACK);
      if (place[i][j].active) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, GREEN);
      }
    }
  }
  drawInv();
  EndDrawing();
}

void updateGame() {
  // map
  for (int i = 0; i < BLOCKS_COL; i++) {
    for (int j = 0; j < BLOCKS_PER_LINE; j++) {
      if (GetMouseX() >= place[i][j].position.x &&
          GetMouseX() <= place[i][j].position.x + placeSize.x / 2 &&
          GetMouseY() >= place[i][j].position.y &&
          GetMouseY() <= place[i][j].position.y + placeSize.y / 2) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !place[i][j].active)
          place[i][j].active = true;
        else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && place[i][j].active)
          place[i][j].active = false;
      }
    }
  }

  // Inventory
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 4; j++) {
      if (GetMouseX() >= (WIDTH / 2 + 100) - (100 * j) &&
          GetMouseX() <= (WIDTH / 2 + 100) - (100 * j) - 100 &&
          GetMouseY() >= 300 && GetMouseY() <= 300 + 100)
        DrawRectangleLines((WIDTH / 2 + 100) - (100 * j), 300, 100, 100, BLACK);
    }
  }
}

void drawInv() {
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 4; j++) {
      DrawRectangleLines((WIDTH / 2 + 100) - (100 * j), 300, 100, 100, BLACK);
    }
  }
}

void duGame() {
  drawGame();
  updateGame();
}
