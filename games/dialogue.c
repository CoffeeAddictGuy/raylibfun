#include <raylib.h>
#include <stdlib.h>

typedef enum DialogueType {
  DefaultType,
  DefaultWithAvatar,
  BubbleType
} DialogueType;

typedef struct DialogueBoxSkin {
  Texture2D center;
  Texture2D leftTopAngle;
  Texture2D leftBotAngle;
  Texture2D rightTopAngle;
  Texture2D rightBotAngle;
  Texture2D centerTopAngle;
  Texture2D centerBotAngle;
  Texture2D centerLeftAngle;
  Texture2D centerRightAngle;
} DialogueBoxSkin;

typedef struct Dialogue {
  Font font;
  DialogueBoxSkin *skin;
  DialogueType type;
} Dialogue;

typedef struct TextBox {
  Vector2 position;
} textBox;

// Dialogue Texture
static DialogueBoxSkin *initDialogueBoxSkin(
    const char *center, const char *leftTopAngle, const char *leftBotAngle,
    const char *rightTopAngle, const char *rightBotAngle, const char *topMiddle,
    const char *botMiddle, const char *leftMiddle, const char *rightMiddle);
static void freeDialogueBoxSkin(DialogueBoxSkin *skin);

// Dialogue main module
static Dialogue *initDialogue(Font font, DialogueType type,
                              DialogueBoxSkin *skin);
static void freeDialogue(Dialogue *dialogue);

// Dialogue Box
static textBox *createDialogueBox(Font font, char *text, Vector2 position,
                                  float fontSize, Color color);
static void drawDialogueBox(textBox textBox);
static void updateDialogueBox(textBox textBox);
static void clearDialogueBox(textBox textBox);

DialogueBoxSkin *initDialogueBoxSkin(
    const char *center, const char *leftTopAngle, const char *leftBotAngle,
    const char *rightTopAngle, const char *rightBotAngle, const char *topMiddle,
    const char *botMiddle, const char *leftMiddle, const char *rightMiddle) {
  DialogueBoxSkin *skin =
      (DialogueBoxSkin *)malloc(sizeof(DialogueBoxSkin) * 1);
  Texture2D centerTexture = LoadTexture(center);
  Texture2D leftTopAngleTexture = LoadTexture(leftTopAngle);
  Texture2D leftBotAngleTexture = LoadTexture(leftBotAngle);
  Texture2D rightTopAngleTexture = LoadTexture(rightTopAngle);
  Texture2D rightBotAngleTexture = LoadTexture(rightBotAngle);
  Texture2D topMiddleTexture = LoadTexture(topMiddle);
  Texture2D botMiddleTexture = LoadTexture(botMiddle);
  Texture2D leftMiddleTexture = LoadTexture(leftMiddle);
  Texture2D rightMiddleTexture = LoadTexture(rightMiddle);

  *skin = (DialogueBoxSkin){
      centerTexture,        leftTopAngleTexture,  leftBotAngleTexture,
      rightTopAngleTexture, rightBotAngleTexture, topMiddleTexture,
      botMiddleTexture,     leftMiddleTexture,    rightMiddleTexture};
  return skin;
}

void freeDialogueBoxSkin(DialogueBoxSkin *skin) {
  UnloadTexture(skin->center);
  UnloadTexture(skin->rightBotAngle);
  UnloadTexture(skin->rightTopAngle);
  UnloadTexture(skin->leftBotAngle);
  UnloadTexture(skin->leftTopAngle);
  UnloadTexture(skin->centerLeftAngle);
  UnloadTexture(skin->centerBotAngle);
  UnloadTexture(skin->centerRightAngle);
  UnloadTexture(skin->centerTopAngle);
  free(skin);
}

Dialogue *initDialogue(Font font, DialogueType type, DialogueBoxSkin *skin) {
  Dialogue *dialogue = (Dialogue *)malloc(sizeof(Dialogue) * 1);
  *dialogue = (Dialogue){font, skin, type};
  return dialogue;
}

void freeDialogue(Dialogue *dialogue) { free(dialogue); }

textBox *createDialogueBox(Font font, char *text, Vector2 position,
                           float fontSize, Color color) {
  textBox *box = (textBox *)malloc(sizeof(textBox) * 1);
  return box;
}
void drawDialogueBox(textBox bubble) {}
void updateDialogueBox(textBox bubble) {}
void clearDialogueBox(textBox bubble) {}
