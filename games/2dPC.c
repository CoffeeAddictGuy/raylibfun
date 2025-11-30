#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define MY_YELLOW CLITERAL(Color){255, 140, 130, 200}
#define MY_GREEN CLITERAL(Color){50, 255, 50, 200}
#define GRAYART CLITERAL(Color){50, 255, 50, 200}
#define WHITEART CLITERAL(Color){50, 255, 50, 200}

#define MAX_ROW 32
#define MAX_COLOMN 32

int width = 1920;
int height = 1080;
int DEBUG = 1;
int windowHold = 0;

typedef struct Window {
  Texture2D winTex;
  Vector2 TexSize;
  Vector2 position;
  Vector2 triggerPos;
  float scale;
  bool open;
} Window;

typedef struct Button {
  Texture2D def;
  Texture2D pressed;
  Vector2 texSize;
  Vector2 position;
  Vector2 triggerPosition;
  float scale;
  int type;
  int state;
} Button;

typedef struct Pixel {
  Vector2 position;
  Vector2 size;
  Color color;
  bool colored;
  bool mSelect;
} Pixel;

Window back;
Window app;
Window mascot;
Color debugColor;

Font font;
Pixel pixels[MAX_COLOMN][MAX_ROW] = {0};

static void gameInit();
static void gameDraw();
static void gameUpdate();
static void gameInput();
static void gameDrawAndUpdate();

static void drawWindow();

static void debugDraw();
static void debugLog();

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  SetTargetFPS(144);
  InitWindow(width, height, "2D PC");
  // SetWindowState(FLAG_FULLSCREEN_MODE);

  gameInit();

  while (!WindowShouldClose()) {
    gameDrawAndUpdate();
    gameInput();
  }
}

void gameInit() {
  // Font
  int codepoints[384];
  int count = 0;
  // ASCII
  for (int cp = 0x20; cp < 0x7E; cp++)
    codepoints[count++] = cp;
  // Cir
  for (int cp = 0x0400; cp < 0x04FF; cp++)
    codepoints[count++] = cp;

  font = LoadFontEx("resources/ScienceGothic.ttf", 48, codepoints, count);
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

  // Texture init
  Texture2D backgroundTex = LoadTexture("resources/MainWindowPC.png");
  Texture2D appTex = LoadTexture("resources/AppWindow.png");
  Texture2D mascotTex = LoadTexture("resources/Maskot.png");

  back = (Window){backgroundTex,
                  (Vector2){backgroundTex.width, backgroundTex.height},
                  (Vector2){0, 0},
                  (Vector2){0, 0},
                  6.0f,
                  true};
  app = (Window){appTex,
                 (Vector2){appTex.width, appTex.height},
                 (Vector2){50, 100},
                 (Vector2){appTex.width * 2.9f,
                           appTex.height / 4.0f + (appTex.height / 4.0f)},
                 4.0f,
                 true};
  mascot = (Window){mascotTex,
                    (Vector2){mascotTex.width, mascotTex.height},
                    (Vector2){100, 100},
                    (Vector2){mascotTex.width, mascotTex.height},
                    4.0f,
                    true};
  TraceLog(LOG_DEBUG, "Texture of app - %d x and %d y", appTex.width,
           appTex.height);

  for (int i = 0; i < MAX_COLOMN; i++) {
    for (int j = 0; j < MAX_ROW; j++) {
      pixels[i][j] = (Pixel){
          (Vector2){app.position.x + j * 15 + 30, app.position.y + i * 15 + 30},
          (Vector2){15, 15}, ((i + j) % 2 == 1) ? LIGHTGRAY : GRAY, false,
          false};
    }
  }

  // Debug
  debugColor = MY_YELLOW;
}

void gameDraw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  drawWindow();
  if (DEBUG)
    debugDraw();

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  DrawTextEx(font, TextFormat("%d:%d:%d", t->tm_hour, t->tm_min, t->tm_sec),
             (Vector2){50, 50}, 48, 2, BLACK);
  EndDrawing();
}

void gameUpdate() {}

void gameInput() {
  Vector2 mouseCord = GetMousePosition();
  // TraceLog(LOG_DEBUG, "App pos - %f and %f", app.position.x, app.position.y);
  // TraceLog(LOG_DEBUG, "App triger zone - %f and %f", app.triggerPos.x,
  //          app.triggerPos.y);
  if (mouseCord.x >= app.position.x &&
      mouseCord.x <= app.position.x + app.triggerPos.x &&
      mouseCord.y >= app.position.y &&
      mouseCord.y <= app.position.y + app.triggerPos.y) {
    debugColor = MY_GREEN;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      windowHold = 1;
    } else {
      windowHold = 0;
    }
  } else {
    debugColor = MY_YELLOW;
  }
  if (windowHold == 1) {
    app.position.x += GetMouseDelta().x;
    app.position.y += GetMouseDelta().y;
    for (int i = 0; i < MAX_COLOMN; i++) {
      for (int j = 0; j < MAX_ROW; j++) {
        pixels[i][j].position = (Vector2){app.position.x + j * 15 + 30,
                                          app.position.y + i * 15 + 30};
      }
    }
  }
  for (int i = 0; i < MAX_COLOMN; i++) {
    for (int j = 0; j < MAX_ROW; j++) {
      if (mouseCord.x >= pixels[i][j].position.x &&
          mouseCord.x <= pixels[i][j].position.x + pixels[i][j].size.x &&
          mouseCord.y >= pixels[i][j].position.y &&
          mouseCord.y <= pixels[i][j].position.y + pixels[i][j].size.y) {
        TraceLog(LOG_DEBUG, "TEST #%d%d", i, j);
        pixels[i][j].mSelect = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !pixels[i][j].colored) {
          pixels[i][j].color = BLACK;
          pixels[i][j].colored = true;
        } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) &&
                   pixels[i][j].colored) {
          pixels[i][j].color = ((i + j) % 2 == 1) ? LIGHTGRAY : GRAY;
          pixels[i][j].colored = false;
        }
      } else {
        pixels[i][j].mSelect = false;
      }
    }
  }

  // TraceLog(LOG_DEBUG, "Mouse pos - %f and %f", GetMousePosition().x,
  //          GetMousePosition().y);
}

void gameDrawAndUpdate() {
  gameDraw();
  gameUpdate();
}

void debugDraw() {
  DrawRectangle(app.position.x, app.position.y, app.triggerPos.x,
                app.triggerPos.y, debugColor);
  DrawTextEx(font, "Game have english language и русский ! ^_^",
             (Vector2){20, 20}, 48, 2, BLACK);
}

void drawWindow() {
  // Main backgroud
  DrawTextureEx(back.winTex, back.position, 0.0f, back.scale, WHITE);

  // Mascot
  DrawTextureEx(mascot.winTex, (Vector2){app.position.x, app.position.y - 80},
                0.0f, mascot.scale, WHITE);

  // Draw App
  DrawTextureEx(app.winTex, app.position, 0.0f, app.scale, WHITE);
  for (int i = 0; i < MAX_COLOMN; i++) {
    for (int j = 0; j < MAX_ROW; j++) {
      if (pixels[i][j].mSelect)
        DrawRectangle(pixels[i][j].position.x, pixels[i][j].position.y,
                      pixels[i][j].size.x, pixels[i][j].size.y, BLACK);
      else
        DrawRectangle(pixels[i][j].position.x, pixels[i][j].position.y,
                      pixels[i][j].size.x, pixels[i][j].size.y,
                      pixels[i][j].color);
    }
  }
}
