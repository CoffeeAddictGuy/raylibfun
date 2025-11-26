#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define MY_YELLOW CLITERAL(Color){255, 140, 130, 200}
#define MY_GREEN CLITERAL(Color){50, 255, 50, 200}

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

Window back;
Window app;
Color debugColor;

Font font;

static void gameInit();
static void gameDraw();
static void gameUpdate();
static void gameInput();
static void gameDrawAndUpdate();

static void debugDraw();
static void debugLog();

int main() {
  SetTraceLogLevel(LOG_DEBUG);
  SetTargetFPS(144);
  InitWindow(width, height, "2D PC");
  SetWindowState(FLAG_FULLSCREEN_MODE);

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

  back = (Window){backgroundTex,
                  (Vector2){backgroundTex.width, backgroundTex.height},
                  (Vector2){0, 0},
                  (Vector2){0, 0},
                  6.0f,
                  true};
  app = (Window){appTex,
                 (Vector2){appTex.width, appTex.height},
                 (Vector2){50, 100},
                 (Vector2){appTex.width * 4.0f,
                           appTex.height / 4.0f + (appTex.height / 4.0f)},
                 4.0f,
                 true};
  TraceLog(LOG_DEBUG, "Texture of app - %d x and %d y", appTex.width,
           appTex.height);

  // Debug
  debugColor = MY_YELLOW;
}

void gameDraw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawTextureEx(back.winTex, back.position, 0.0f, back.scale, WHITE);
  DrawTextureEx(app.winTex, app.position, 0.0f, app.scale, WHITE);

  if (DEBUG)
    debugDraw();

  EndDrawing();
}

void gameUpdate() {}

void gameInput() {
  Vector2 mouseCord = GetMousePosition();
  TraceLog(LOG_DEBUG, "App pos - %f and %f", app.position.x, app.position.y);
  TraceLog(LOG_DEBUG, "App triger zone - %f and %f", app.triggerPos.x,
           app.triggerPos.y);
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
  }
  TraceLog(LOG_DEBUG, "Mouse pos - %f and %f", GetMousePosition().x,
           GetMousePosition().y);
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
