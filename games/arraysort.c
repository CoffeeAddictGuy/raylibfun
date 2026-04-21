#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1200
#define HEIGHT 800

#define MIN_ELEMENTS 10
#define MAX_ELEMENTS 50
#define MIN_VALUE 10
#define MAX_VALUE HEIGHT - 300

#define PADDING 100
#define GAP 10

typedef struct Timer {
  double start;
  double end;
} Timer;

typedef struct Element {
  int val;
  bool is_active;
} Element;

typedef struct Array {
  Element *array;
  size_t size;
  size_t element_width;
  size_t active_element;
} Array;

static Array main_array = {0};
static Timer timer = {0};
static bool swapped = true;
static bool pause = true;

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

bool array_check(Element *array, size_t size) {
  for (int i = 1; i < size; i++) {
    if (array[i].val < array[i - 1].val) {
      return false;
    }
  }
  return true;
}

void init() {
  timer = (Timer){.start = 0.f, .end = 0.05f};

  srand(time(NULL));

  main_array.size = rand() % (MAX_ELEMENTS - MIN_ELEMENTS + 1) + MIN_ELEMENTS;
  main_array.array = malloc(sizeof(Element) * main_array.size);

  main_array.element_width =
      (WIDTH - (PADDING * 2) - (main_array.size * GAP)) / main_array.size;

  main_array.active_element = 1;

  for (int i = 0; i < main_array.size; i++) {
    int new_value = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
    if (new_value == 0) {
      TraceLog(LOG_DEBUG, "ZERO VALUE!");
    }
    main_array.array[i].val = new_value;
    main_array.array[i].is_active = 0;
  }
  TraceLog(LOG_DEBUG, "Main Array info:");
  TraceLog(LOG_DEBUG, "- Element count %d", main_array.size);
  TraceLog(LOG_DEBUG, "- Element width %d", main_array.element_width);
}

void restart() {
  timer.start = 0.f;
  main_array.size = rand() % (MAX_ELEMENTS - MIN_ELEMENTS + 1) + MIN_ELEMENTS;
  main_array.element_width =
      (WIDTH - (PADDING * 2) - (main_array.size * GAP)) / main_array.size;
  main_array.active_element = 1;

  TraceLog(LOG_DEBUG, "Main Array info:");
  TraceLog(LOG_DEBUG, "- Element count %d", main_array.size);
  TraceLog(LOG_DEBUG, "- Element width %d", main_array.element_width);

  Element *tmp = malloc(sizeof(Element) * main_array.size);
  if (tmp != NULL) {
    free(main_array.array);
    main_array.array = tmp;
  }

  for (int i = 0; i < main_array.size; i++) {
    main_array.array[i].val = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
    main_array.array[i].is_active = 0;
  }
}

void draw() {
  BeginDrawing();
  ClearBackground(BLACK);
  float start_cord = PADDING;
  for (int i = 0; i < main_array.size; i++) {
    Color curr_color = YELLOW;
    if (main_array.array[i].is_active) {
      curr_color = RED;
    } else {
      curr_color = YELLOW;
    }

    Vector2 start_element_pos =
        (Vector2){.x = start_cord + (main_array.element_width * i +
                                     main_array.element_width / 2.0),
                  .y = WIDTH - 500};

    Vector2 end_element_pos =
        (Vector2){.x = start_cord + (main_array.element_width * i +
                                     main_array.element_width / 2.0),
                  .y = start_element_pos.y - main_array.array[i].val};
    DrawLineEx(start_element_pos, end_element_pos, main_array.element_width,
               curr_color);
    start_cord += GAP;
  }
  EndDrawing();
}

void update() {
  if (timer.start < timer.end) {
    timer.start += GetFrameTime();
    return;
  } else {
    timer.start = 0;
    main_array.array[main_array.active_element].is_active = false;
    main_array.active_element++;
  }

  if (!array_check(main_array.array, main_array.size)) {
    if (main_array.array[main_array.active_element].val <
        main_array.array[main_array.active_element - 1].val) {
      swap(&main_array.array[main_array.active_element].val,
           &main_array.array[main_array.active_element - 1].val);
    }
  } else {
    pause = true;
  }

  if (main_array.active_element == main_array.size - 1) {
    main_array.active_element = 0;
  }

  if (!main_array.array[main_array.active_element].is_active) {
    main_array.array[main_array.active_element].is_active = true;
  }
}

void input() {
  if (IsKeyPressed(KEY_R)) {
    restart();
    pause = false;
  }
  if (IsKeyPressed(KEY_SPACE)) {
    pause = !pause;
  }
}

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  SetTargetFPS(144);
  InitWindow(WIDTH, HEIGHT, "Sorter");

  init();

  while (!WindowShouldClose()) {
    if (!pause) {
      update();
    }
    draw();

    input();
  }
  return 0;
}
