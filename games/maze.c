#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define BLOCK_PER_LINE 10
#define BLOCK_LINE 10
#define INVENTORY_ITEMS 4

enum MAP_BLOCK_TYPE { ROAD, WALL, START, END };

static void game_init();
static void game_draw();
static void game_update();
static void game_draw_and_update();
static void input_callback();

typedef struct {
  Vector2 position;
  int type;
  int draw_type;
} Blocks;

typedef struct Inventory {
  Vector2 position;
  int type;
  int draw_type;
} Inventory;

typedef struct {
  int type;
} Brush;

static Blocks blocks[BLOCK_LINE][BLOCK_PER_LINE] = {0};
static Inventory inv[INVENTORY_ITEMS] = {0};
static Vector2 placeSize = {0};
static Vector2 invSize = {0};
static Brush brush = {0};

int main() {

  InitWindow(800, 600, "Maze");
  SetTargetFPS(60);
  game_init();
  while (!WindowShouldClose()) {
    game_draw_and_update();
    input_callback();
  }
  CloseWindow();
  return 0;
}

static void game_init() {
  brush.type = WALL;
  placeSize = (Vector2){(float)GetScreenWidth() / BLOCK_PER_LINE - 50,
                        (float)GetScreenWidth() / BLOCK_PER_LINE - 50};
  invSize = (Vector2){300, 200};
  // map init
  for (int i = 0; i < BLOCK_LINE; i++) {
    for (int j = 0; j < BLOCK_PER_LINE; j++) {
      blocks[i][j].position =
          (Vector2){j * placeSize.x + 230, i * placeSize.y + 50};
      blocks[i][j].type = ROAD;
      blocks[i][j].draw_type = blocks[i][j].type;
    }
  }

  // inventory init
  for (int i = 0; i < INVENTORY_ITEMS; i++) {
    inv[i].position = (Vector2){i * (invSize.x / 2 + 10) + 170, 450};
    inv[i].draw_type = i;
    inv[i].type = inv[i].draw_type;
  }
}

static void game_draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("MAZE", (800 / 2) - 5, (600 / 2) - 5, 20, LIGHTGRAY);
  // map
  for (int i = 0; i < BLOCK_LINE; i++) {
    for (int j = 0; j < BLOCK_PER_LINE; j++) {
      DrawRectangleLines(blocks[i][j].position.x, blocks[i][j].position.y,
                         placeSize.x, placeSize.y, BLACK);
      if (blocks[i][j].type == WALL) {
        DrawRectangle(blocks[i][j].position.x, blocks[i][j].position.y,
                      placeSize.x, placeSize.y, BLACK);
      }
      if (blocks[i][j].type == START) {
        DrawRectangle(blocks[i][j].position.x, blocks[i][j].position.y,
                      placeSize.x, placeSize.y, GREEN);
      }
      if (blocks[i][j].type == END) {
        DrawRectangle(blocks[i][j].position.x, blocks[i][j].position.y,
                      placeSize.x, placeSize.y, RED);
      }
    }
  }

  // inventory
  for (int i = 0; i < INVENTORY_ITEMS; i++) {
    if (inv[i].type == ROAD) {
      DrawRectangle(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                    invSize.y / 2, WHITE);
      DrawRectangleLines(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                         invSize.y / 2, BLACK);
    }
    if (inv[i].type == WALL) {
      DrawRectangle(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                    invSize.y / 2, BLACK);
    }
    if (inv[i].type == START) {
      DrawRectangle(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                    invSize.y / 2, GREEN);
    }
    if (inv[i].type == END) {
      DrawRectangle(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                    invSize.y / 2, RED);
    }
  }
  EndDrawing();
}

static void game_update() {}

static void game_draw_and_update() {
  game_draw();
  game_update();
}

static void input_callback() {
  Vector2 mouse_cord = GetMousePosition();
  for (int i = 0; i < INVENTORY_ITEMS; i++) {
    if (mouse_cord.x >= inv[i].position.x &&
        mouse_cord.x <= inv[i].position.x + invSize.x / 2 &&
        mouse_cord.y >= inv[i].position.y &&
        mouse_cord.y <= inv[i].position.y + invSize.y / 2) {
      DrawRectangle(inv[i].position.x, inv[i].position.y, invSize.x / 2,
                    invSize.y / 2, YELLOW);
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        brush.type = inv[i].type;
      }
    }
  }

  for (int i = 0; i < BLOCK_LINE; i++) {
    for (int j = 0; j < BLOCK_PER_LINE; j++) {
      if (mouse_cord.x >= blocks[i][j].position.x &&
          mouse_cord.x <= blocks[i][j].position.x + placeSize.x &&
          mouse_cord.y >= blocks[i][j].position.y &&
          mouse_cord.y <= blocks[i][j].position.y + placeSize.y) {
        DrawRectangle(blocks[i][j].position.x, blocks[i][j].position.y,
                      placeSize.x, placeSize.y, YELLOW);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          blocks[i][j].type = brush.type;
        }
      }
    }
  }
}
