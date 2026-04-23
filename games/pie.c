#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIDTH 1200
#define HEIGHT 800

typedef struct Data {
  char *name;
  int data_info;
  float radius;
  float anim_start;
  float anim_end;
  bool is_hovered;
} Data;

typedef struct DataCollector {
  Data *data_array;
  Vector2 *data_array_sector_size;
  float radius;
  int max_size;
  Vector2 circle_center;
} DataCollector;

static DataCollector main_collector;
static Color colors[4] = {RED, GREEN, BLUE, BLACK};

static float anim_end = 3.f;

void rl_init() {
  SetTraceLogLevel(LOG_DEBUG);
  InitWindow(WIDTH, HEIGHT, "Interactive Pie");
}

void init() {
  Vector2 circle_center = (Vector2){.x = WIDTH / 2.f, .y = HEIGHT / 2.f};

  Data *da = malloc(sizeof(Data) * 4);
  da[0].name = "Water";
  da[0].data_info = 500;
  da[0].radius = 300;
  da[1].name = "Sugar";
  da[1].data_info = 50;
  da[1].radius = 300;
  da[2].name = "Oil";
  da[2].data_info = 150;
  da[2].radius = 300;
  da[3].name = "Salt";
  da[3].data_info = 30;
  da[3].radius = 300;

  int sum = 0;
  for (int i = 0; i < 4; i++) {
    sum += da[i].data_info;
  }

  Vector2 *da_ss = malloc(sizeof(Vector2) * 4);
  float start_angle = 0.f;
  for (int i = 0; i < 4; i++) {
    float x = start_angle;
    float proc = (da[i].data_info * 100.f) / sum;
    float y = start_angle + (proc * 360.f) / 100.f;
    start_angle = y;

    da_ss[i].x = x;
    da_ss[i].y = y;
  }

  main_collector = (DataCollector){.data_array = da,
                                   .data_array_sector_size = da_ss,
                                   .max_size = sum,
                                   .radius = 300,
                                   .circle_center = circle_center};
}

void draw() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  for (int i = 0; i < 4; i++) {
    DrawCircleSector(
        main_collector.circle_center, main_collector.data_array[i].radius,
        main_collector.data_array_sector_size[i].x,
        main_collector.data_array_sector_size[i].y, 100, colors[i]);

    if (main_collector.data_array[i].is_hovered) {
      DrawText(TextFormat("%s", main_collector.data_array[i].name), 30, 30, 20,
               GRAY);
    }
  }

  EndDrawing();
}

void update() {
  Vector2 mouse_pos = GetMousePosition();

  float d = sqrtf(pow(mouse_pos.x - main_collector.circle_center.x, 2.0) +
                  pow(mouse_pos.y - main_collector.circle_center.y, 2.0));

  float angle = atan2f(mouse_pos.y - main_collector.circle_center.y,
                       mouse_pos.x - main_collector.circle_center.x);
  float ts = 360.f;
  angle = (angle > 0) ? angle : (2 * PI + angle);

  if (d <= main_collector.radius) {
    for (int i = 0; i < 4; i++) {
      if (GetMouseDelta().x != 0.f || GetMouseDelta().y != 0.f) {
        // TraceLog(LOG_DEBUG, "Mouse angle %.2f", angle * RAD2DEG);
        //
        // TraceLog(LOG_DEBUG,
        //          "Sector %d with name %s start_angle %.2f and end_angle % .2f
        //          ", i, main_collector.data_array[i].name,
        //          main_collector.data_array_sector_size[i].x,
        //          main_collector.data_array_sector_size[i].y);
      }
      if (angle * RAD2DEG > main_collector.data_array_sector_size[i].x &&
          angle * RAD2DEG < main_collector.data_array_sector_size[i].y) {
        main_collector.data_array[i].is_hovered = true;
      } else {
        main_collector.data_array[i].is_hovered = false;
      }
    }
  }
}

void input() {}

void free_collector() {
  free(main_collector.data_array);
  free(main_collector.data_array_sector_size);
}

int main() {
  rl_init();
  init();
  while (!WindowShouldClose()) {
    draw();
    update();
    input();
  }
  free_collector();
}
