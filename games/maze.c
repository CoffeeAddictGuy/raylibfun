#include "../../c_cpp/c-cpp-libs/algorithms/searching/bfs/bfs.h"
#include "../../c_cpp/c-cpp-libs/algorithms/searching/dfs/dfs.h"
#include "../../c_cpp/c-cpp-libs/data_structures/graph/graph.h"

#include "C:\raylib\include\raylib.h"
#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>

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
static Graph *graph;
static int start_id = -1;
static int end_id = -1;
static bool show_path = true;
static BFSResult path = {0};
static DFSResult path2 = {0};

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
  SetTraceLogLevel(LOG_DEBUG);
  brush.type = WALL;
  placeSize = (Vector2){(float)GetScreenWidth() / BLOCK_LINE - 50,
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

  // graph
  graph = graph_create(100);
  for (int i = 0; i < BLOCK_LINE; i++) {
    for (int j = 0; j < BLOCK_PER_LINE; j++) {
      int id = i * BLOCK_PER_LINE + j;
      bool is_passable =
          (blocks[i][j].type == ROAD || blocks[i][j].type == START ||
           blocks[i][j].type == END);

      if (blocks[i][j].type == START) {
        start_id = id;
      }
      if (blocks[i][j].type == END) {
        end_id = id;
      }

      if (is_passable) {
        if (i + 1 < BLOCK_LINE) {
          bool neighbor_passable =
              (blocks[i + 1][j].type == ROAD ||
               blocks[i + 1][j].type == START || blocks[i + 1][j].type == END);
          if (neighbor_passable)
            graph_add_edge(graph, id, id + BLOCK_LINE);
        }
        if (i - 1 < BLOCK_LINE) {
          bool neighbor_passable =
              (blocks[i - 1][j].type == ROAD ||
               blocks[i - 1][j].type == START || blocks[i - 1][j].type == END);
          if (neighbor_passable)
            graph_add_edge(graph, id, id - BLOCK_LINE);
        }
        if (j + 1 < BLOCK_PER_LINE) {
          bool neighbor_passable =
              (blocks[i][j + 1].type == ROAD ||
               blocks[i][j + 1].type == START || blocks[i][j + 1].type == END);
          if (neighbor_passable)
            graph_add_edge(graph, id, id + 1);
        }
        if (j - 1 < BLOCK_PER_LINE) {
          bool neighbor_passable =
              (blocks[i][j - 1].type == ROAD ||
               blocks[i][j - 1].type == START || blocks[i][j - 1].type == END);
          if (neighbor_passable)
            graph_add_edge(graph, id, id - 1);
        }
        // dia
        if (i + 1 < BLOCK_LINE && j + 1 < BLOCK_LINE) {
          bool neighbor_passable = (blocks[i + 1][j + 1].type == ROAD ||
                                    blocks[i + 1][j + 1].type == START ||
                                    blocks[i + 1][j + 1].type == END);
          graph_add_edge(graph, id, id + BLOCK_LINE + 1);
        }
        if (i + 1 < BLOCK_LINE && j - 1 < BLOCK_LINE) {
          bool neighbor_passable = (blocks[i + 1][j - 1].type == ROAD ||
                                    blocks[i + 1][j - 1].type == START ||
                                    blocks[i + 1][j - 1].type == END);
          graph_add_edge(graph, id, id + BLOCK_LINE - 1);
        }
        if (i - 1 < BLOCK_LINE && j - 1 < BLOCK_LINE) {
          bool neighbor_passable = (blocks[i - 1][j - 1].type == ROAD ||
                                    blocks[i - 1][j - 1].type == START ||
                                    blocks[i - 1][j - 1].type == END);
          graph_add_edge(graph, id, id - BLOCK_LINE - 1);
        }
        if (i - 1 < BLOCK_LINE && j + 1 < BLOCK_LINE) {
          bool neighbor_passable = (blocks[i - 1][j + 1].type == ROAD ||
                                    blocks[i - 1][j + 1].type == START ||
                                    blocks[i - 1][j + 1].type == END);
          graph_add_edge(graph, id, id - BLOCK_LINE + 1);
        }
      }
    }
  }
}

static Vector2 id_to_pos(int id) {
  int y = id / BLOCK_PER_LINE;
  int x = id % BLOCK_PER_LINE;
  return (Vector2){blocks[y][x].position.x, blocks[y][x].position.y};
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
      if (path.found && show_path) {
        for (int i = 0; i < path.path_length - 1; i++) {
          Vector2 from = id_to_pos(path.path[i]);
          Vector2 to = id_to_pos(path.path[i + 1]);
          Vector2 from_center = {from.x + placeSize.x / 2,
                                 from.y + placeSize.y / 2};
          Vector2 to_center = {to.x + placeSize.x / 2, to.y + placeSize.y / 2};
          DrawLineV(from_center, to_center, BLUE);
          DrawCircleV(from_center, 5, RED);
        }
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

static void graph_rebuild() {
  graph_free(graph);
  graph = graph_create(100);

  start_id = -1;
  end_id = -1;

  for (int i = 0; i < BLOCK_LINE; i++) {
    for (int j = 0; j < BLOCK_PER_LINE; j++) {
      int id = i * BLOCK_PER_LINE + j;

      bool is_passable =
          (blocks[i][j].type == ROAD || blocks[i][j].type == START ||
           blocks[i][j].type == END);

      if (blocks[i][j].type == START)
        start_id = id;
      if (blocks[i][j].type == END)
        end_id = id;

      if (is_passable) {
        if (i + 1 < BLOCK_LINE) {
          bool neigh_passable =
              (blocks[i + 1][j].type == ROAD ||
               blocks[i + 1][j].type == START || blocks[i + 1][j].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id + BLOCK_PER_LINE);
          }
        }

        if (i > 0) {
          bool neigh_passable =
              (blocks[i - 1][j].type == ROAD ||
               blocks[i - 1][j].type == START || blocks[i - 1][j].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id - BLOCK_PER_LINE);
          }
        }

        if (j + 1 < BLOCK_PER_LINE) {
          bool neigh_passable =
              (blocks[i][j + 1].type == ROAD ||
               blocks[i][j + 1].type == START || blocks[i][j + 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id + 1);
          }
        }

        if (j > 0) {
          bool neigh_passable =
              (blocks[i][j - 1].type == ROAD ||
               blocks[i][j - 1].type == START || blocks[i][j - 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id - 1);
          }
        }

        if (i + 1 < BLOCK_LINE && j + 1 < BLOCK_PER_LINE) {
          bool neigh_passable = (blocks[i + 1][j + 1].type == ROAD ||
                                 blocks[i + 1][j + 1].type == START ||
                                 blocks[i + 1][j + 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id + BLOCK_LINE + 1);
          }
        }
        if (i + 1 < BLOCK_LINE && j > 0) {
          bool neigh_passable = (blocks[i + 1][j - 1].type == ROAD ||
                                 blocks[i + 1][j - 1].type == START ||
                                 blocks[i + 1][j - 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id + BLOCK_LINE - 1);
          }
        }
        if (i > 0 && j > 0) {
          bool neigh_passable = (blocks[i - 1][j - 1].type == ROAD ||
                                 blocks[i - 1][j - 1].type == START ||
                                 blocks[i - 1][j - 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id - BLOCK_LINE - 1);
          }
        }
        if (i > 0 && j + 1 < BLOCK_PER_LINE) {
          bool neigh_passable = (blocks[i - 1][j + 1].type == ROAD ||
                                 blocks[i - 1][j + 1].type == START ||
                                 blocks[i - 1][j + 1].type == END);
          if (neigh_passable) {
            graph_add_edge(graph, id, id - BLOCK_LINE + 1);
          }
        }
      }
    }
  }

  TraceLog(LOG_DEBUG, "start_id=%d, end_id=%d", start_id, end_id);
  if (start_id != -1 && end_id != -1) {
    path = bfs(graph, start_id, end_id);
    path2 = dfs(graph, start_id, end_id);
    TraceLog(LOG_DEBUG, path.found ? "FOUND PATH!" : "PATH NOT FOUND");
  }
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
          if (blocks[i][j].type == START) {
            TraceLog(LOG_DEBUG, "Set start!");
            start_id = i * BLOCK_PER_LINE + j;
          } else if (blocks[i][j].type == END) {
            TraceLog(LOG_DEBUG, "Set end!");
            end_id = i * BLOCK_PER_LINE + j;
          }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        }
      }
    }
  }

  if (IsKeyPressed(KEY_SPACE)) {
    TraceLog(LOG_DEBUG, "Rebuild graph");
    graph_rebuild();
  }
}