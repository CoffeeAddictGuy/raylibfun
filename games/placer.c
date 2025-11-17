#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BLOCKS_PER_LINE 20
#define BLOCKS_COL 5
#define INVENTORY_ITEMS 4

const int WIDTH = 800;
const int HEIGHT = 450;

typedef struct Places {
  Vector2 position;
  int type;
  bool active;
} Places;

typedef struct Inventory {
  Vector2 position;
  int type;
} Inventory;

typedef struct Player {
  int mType;
} Player;

static void initGame();
static void drawGame();
static void drawInv();
static void updateGame();
static void duGame();

static Places place[BLOCKS_COL][BLOCKS_PER_LINE] = {0};
static Inventory inventory[INVENTORY_ITEMS][1] = {0};
static Vector2 placeSize = {0};
static Vector2 invSize = {0};
static Player plr = {0};

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
  placeSize = (Vector2){(float)GetScreenWidth() / BLOCKS_PER_LINE, 40};
  invSize = (Vector2){300, 200};

  // map
  for (int i = 0; i < BLOCKS_COL; i++) {
    for (int j = 0; j < BLOCKS_PER_LINE; j++) {
      place[i][j].position =
          (Vector2){j * placeSize.x + 10, i * placeSize.y + 50};
      place[i][j].active = false;
      place[i][j].type = 0;
    }
  }

  // inventory
  for (int i = 0; i < INVENTORY_ITEMS; i++) {
    inventory[i][0].position = (Vector2){i * (invSize.x / 2 + 10) + 70, 300};
    inventory[i][0].type = i + 1;
  }

  // player
  plr.mType = -1;
}

void drawGame() {
  BeginDrawing();
  ClearBackground(BLUE);

  for (int i = 0; i < BLOCKS_COL; i++) {
    for (int j = 0; j < BLOCKS_PER_LINE; j++) {
      DrawRectangleLines(place[i][j].position.x, place[i][j].position.y,
                         placeSize.x / 2, placeSize.y / 2, BLACK);
      if (place[i][j].active && place[i][j].type != 0) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, GRAY);
      }
      if (place[i][j].active && place[i][j].type == 1) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, RED);
      }
      if (place[i][j].active && place[i][j].type == 2) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, GREEN);
      }
      if (place[i][j].active && place[i][j].type == 3) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, YELLOW);
      }
      if (place[i][j].active && place[i][j].type == 4) {
        DrawRectangle(place[i][j].position.x, place[i][j].position.y,
                      placeSize.x / 2, placeSize.y / 2, PINK);
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
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !place[i][j].active) {
          place[i][j].active = true;
          place[i][j].type = plr.mType;
        } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && place[i][j].active)
          place[i][j].active = false;
      }
    }
  }
}

void drawInv() {
  Color clrSelect = (Color){253, 249, 0, 100};
  for (int i = 0; i < INVENTORY_ITEMS; i++) {
    DrawRectangleLines(inventory[i][0].position.x, inventory[0][i].position.y,
                       invSize.x / 2, invSize.y / 2, BLACK);
    if (GetMouseX() >= inventory[i][0].position.x &&
        GetMouseX() <= inventory[i][0].position.x + invSize.x / 2 &&
        GetMouseY() >= inventory[i][0].position.y &&
        GetMouseY() <= inventory[i][0].position.y + invSize.y / 2) {
      DrawRectangle(inventory[i][0].position.x, inventory[0][i].position.y,
                    invSize.x / 2, invSize.y / 2, YELLOW);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        TraceLog(LOG_DEBUG, "You pick - %d", inventory[i][0].type);
        plr.mType = inventory[i][0].type;
      }
    }
    if (plr.mType == inventory[i][0].type) {
      DrawRectangle(inventory[i][0].position.x, inventory[i][0].position.y,
                    invSize.x / 2, invSize.y / 2, clrSelect);
    }
  }
}

void duGame() {
  drawGame();
  updateGame();
}
